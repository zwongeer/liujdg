#include <ext/stdio_filebuf.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/prctl.h>
#include <unistd.h>


#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <mutex>
#include <stdexcept>
#include <string>
#include <thread>

#include "Lfunc.hpp"
#include "LProcess.hpp"

static void Attack204_updateKilled(int pid, std::atomic<int>& status, std::atomic<bool>& killed) {
    int ss;
    ::waitpid(pid, &ss, 0);
    status = ss;
    killed = true;
}

LProcess::LProcess (const std::string& command, const std::string& currentDir) {
    killed = false;

    if (!LcheckCommand(command) && !LcheckCommand(currentDir + command)) throw std::runtime_error("the command: `" + command + "` cannot be executed");
    if (!LcheckDirectory(currentDir)) throw std::runtime_error("cannot access the directory:[" + currentDir + "]");
    if (pipe(pipestdinfd)) throw std::runtime_error("failed to create a pipestdin");
    if (pipe(pipestdoutfd)) throw std::runtime_error("failed to create a pipestdout");

    pid = fork();

    if (pid < 0) throw std::runtime_error("failed to create a subprogress");
    if (pid == 0) {
        // run in child
        chdir(currentDir.c_str());

        dup2(pipestdinfd[0], STDIN_FILENO);
        dup2(pipestdoutfd[1], STDOUT_FILENO);
        dup2(pipestdoutfd[1], STDERR_FILENO);
        
        // ask kernel to deliver SIGTERM in case the parent dies
        prctl(PR_SET_PDEATHSIG, SIGTERM); // it is weird that this seems not to work

        // run the program
        execl("/bin/sh", "/bin/sh", "-c", command.c_str(), nullptr);
        
        // execl("/bin/sh", "/bin/sh", "-c", c_command, nullptr);
        
        // Nothing below this line should be executed by child process. If so,
        // it means that the execl function wasn't successfull, so lets throw:
        throw std::runtime_error("failed to run the program");
    }
    // run in parent
    close(pipestdinfd[0]);
    close(pipestdoutfd[1]);

    c_p_from_child = fdopen(pipestdoutfd[0], "r");
    fd_from_child = fileno(c_p_from_child);

    c_p_to_child = fdopen(pipestdinfd[1], "w");
    fd_to_child = fileno(c_p_to_child);

    __gnu_cxx::stdio_filebuf<char>* p_inbuf = new __gnu_cxx::stdio_filebuf<char>(fd_from_child, std::ios::in);
    __gnu_cxx::stdio_filebuf<char>* p_outbuf = new __gnu_cxx::stdio_filebuf<char>(fd_to_child, std::ios::out);
    //__gnu_cxx::stdio_filebuf<char>* p_errbuf = new __gnu_cxx::stdio_filebuf<char>(fileno( fdopen(pipestderrfd[0], "r") ), std::ios::in);

    p_from_child = new std::istream(p_inbuf);
    //p_stderr = new std::istream(p_errbuf);
    p_to_child = new std::ostream(p_outbuf);

    p_from_child->tie(p_to_child);

    std::thread(Attack204_updateKilled, pid, std::ref(status), std::ref(killed)).detach();
}

int LProcess::getpid () {
    return (int)pid;
}

void LProcess::kill() {
    ::kill(pid, SIGKILL); // send SIGKILL signal to the child process
}

LProcess::~LProcess() {
    delete p_from_child;
    delete p_to_child;
    close(pipestdinfd[1]);
    close(pipestdoutfd[0]);
    if (!killed)this->kill();
}

std::ostream& LProcess::stdin() {
    return *p_to_child;
}

std::istream& LProcess::stdout() {
    // this->flush();
    return *p_from_child;
}

void LProcess::flush() {
    const_cast<std::ostream*>(p_to_child)->flush();
}

int LProcess::getReturnValue() {
    if (!killed.load()) return -1024;
    return WEXITSTATUS(status.load());
}

int LProcess::wait() {
    while (!killed.load())
        continue;
    return WEXITSTATUS(status.load());
}

bool LProcess::isRunning() {
    // return ::kill(pid, 0) == 0;
    return !killed;
}