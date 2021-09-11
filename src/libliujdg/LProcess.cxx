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

#include "Lfunc.hpp"
#include "LProcess.hpp"

LProcess::LProcess (const std::string& command, const std::string& currentDir) {
    stdinPipeClosed = false;
    status = Status::NONE;
    if (!LcheckCommand(command) && !LcheckCommand(currentDir + command)) throw std::runtime_error("the command: `" + command + "` cannot be executed");
    if (!LcheckDirectory(currentDir)) throw std::runtime_error("cannot access the directory:[" + currentDir + "]");
    if (pipe(pipestdinfd)) throw std::runtime_error("failed to create a pipestdin");
    if (pipe(pipestdoutfd)) throw std::runtime_error("failed to create a pipestdout");
    if (pipe(pipestderrfd)) throw std::runtime_error("failed to create a pipestderr");

    pid = fork();

    if (pid < 0) throw std::runtime_error("failed to create a subprogress");
    if (pid == 0) {
        close(pipestdinfd[1]);
        close(pipestdoutfd[0]);
        close(pipestderrfd[0]);
        // run in child
        chdir(currentDir.c_str());

        dup2(pipestdinfd[0], STDIN_FILENO);
        dup2(pipestdoutfd[1], STDOUT_FILENO);
        dup2(pipestderrfd[1], STDERR_FILENO);
        
        // ask kernel to deliver SIGKILL in case the parent dies
        prctl(PR_SET_PDEATHSIG, SIGKILL); // it is weird that this seems not to work

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
    close(pipestderrfd[1]);

    c_p_from_child = fdopen(pipestdoutfd[0], "r");
    fd_from_child = fileno(c_p_from_child);

    c_p_stderr = fdopen(pipestderrfd[0], "r");
    fd_stderr_child = fileno(c_p_stderr);

    c_p_to_child = fdopen(pipestdinfd[1], "w");
    fd_to_child = fileno(c_p_to_child);

    p_inbuf = new __gnu_cxx::stdio_filebuf<char>(fd_from_child, std::ios::in); // stdout
    p_outbuf = new __gnu_cxx::stdio_filebuf<char>(fd_to_child, std::ios::out); // stdin
    p_errbuf = new __gnu_cxx::stdio_filebuf<char>(fd_stderr_child, std::ios::in); //stderr

    p_from_child = new std::istream((__gnu_cxx::stdio_filebuf<char>*)p_inbuf); // stdout
    p_stderr = new std::istream((__gnu_cxx::stdio_filebuf<char>*)p_errbuf); //stderr
    p_to_child = new std::ostream((__gnu_cxx::stdio_filebuf<char>*)p_outbuf); // stdin

    p_from_child->tie(p_to_child);
    p_stderr->tie(p_to_child);
}

int LProcess::getpid () {
    return (int)pid;
}

void LProcess::kill() {
    checkStatus();
    if (status.load() == Status::NONE || status.load() == Status::STOPPED || status.load() == Status::CONTINUED)
        ::kill(pid, SIGKILL); // send SIGKILL signal to the child process (like kill -9 pid)
}

LProcess::~LProcess() {
    if (!stdinPipeClosed) {
        fclose(c_p_to_child);
        delete (__gnu_cxx::stdio_filebuf<char>*)p_outbuf;
        delete p_to_child;
        close(pipestdinfd[1]);
    }
    fclose(c_p_from_child);
    fclose(c_p_stderr);

    delete (__gnu_cxx::stdio_filebuf<char>*)p_errbuf;
    delete (__gnu_cxx::stdio_filebuf<char>*)p_inbuf;

    delete p_from_child;
    delete p_stderr;
    
    close(pipestdoutfd[0]);
    close(pipestderrfd[0]);
    
    this->kill();
}

void LProcess::closeInPipe() {
    if (stdinPipeClosed) return;
    p_from_child->tie(nullptr);
    p_stderr->tie(nullptr);
    fclose(c_p_to_child);
    delete (__gnu_cxx::stdio_filebuf<char>*)p_outbuf;
    delete p_to_child;
    close(pipestdinfd[1]);
    stdinPipeClosed = true;
}

std::ostream& LProcess::stdin() {
    return *p_to_child;
}

std::istream& LProcess::stdout() {
    // this->flush();
    return *p_from_child;
}

std::istream& LProcess::stderr() {
    return *p_stderr;
}

void LProcess::flush() {
    const_cast<std::ostream*>(p_to_child)->flush();
}

std::optional<int> LProcess::getReturnValue() {
    checkStatus();
    if (status.load() != Status::EXITED) return {};
    return WEXITSTATUS(status_wait);
}

std::optional<int> LProcess::getSignal() {
    checkStatus();
    if (status.load() == Status::SIGNALED) return WSTOPSIG(status_wait);
    if (status.load() == Status::STOPPED) return WSTOPSIG(status_wait);
    return {};
}

LProcess::Status LProcess::wait() {
    int w = ::waitpid(pid, &status_wait, WUNTRACED | WCONTINUED);
    if (w == -1)
        return status = Status::UNKNOWN;
    
    if (w == pid) {
        if (WIFEXITED(status_wait))
            return status = Status::EXITED;
        else if (WIFSIGNALED(status_wait))
            return status = Status::SIGNALED;
        else if (WIFSTOPPED(status_wait))
            return status = Status::STOPPED;
        else if (WIFCONTINUED(status_wait))
            return status = Status::CONTINUED;
    }
    return status = Status::UNKNOWN; // this may never happen
}

void LProcess::checkStatus() {
    // skip if the child program is waited
    if (status != Status::NONE && status != Status::STOPPED && status != Status::CONTINUED) return;

    // see https://linux.die.net/man/2/waitpid
    int w = ::waitpid(pid, &status_wait, WNOHANG);
    if (w == 0) return;
    if (w == -1) {
        status = Status::UNKNOWN;
        return;
    }
    if (w == pid) {
        if (WIFEXITED(status_wait))
            status = Status::EXITED;
        else if (WIFSIGNALED(status_wait))
            status = Status::SIGNALED;
        else if (WIFSTOPPED(status_wait))
            status = Status::STOPPED;
        else if (WIFCONTINUED(status_wait))
            status = Status::CONTINUED;
    }
}

bool LProcess::isRunning() {
    checkStatus();
    return status.load() != Status::EXITED && status.load() != Status::SIGNALED 
        && status.load() != Status::UNKNOWN && status.load() != Status::STOPPED;
}