#pragma once

#include <atomic>
#include <iostream>
#include <string>
#include <optional>

struct LProcess {
    pid_t pid;
    int status_wait; // passed to the function waitpid
    enum struct Status : int {
        NONE, // initial status
        UNKNOWN, // unknow status
        EXITED, // exited
        SIGNALED, // receiced signal and stopped
        STOPPED, // suspended
        CONTINUED // resumed
    };
    std::atomic<Status> status;
    int pipestdinfd[2];
    int pipestdoutfd[2];
    int pipestderrfd[2];
    bool stdinPipeClosed;

    int fd_from_child;
    int fd_to_child;
    int fd_stderr_child;
    FILE* c_p_from_child;
    FILE* c_p_to_child;
    FILE* c_p_stderr;
    std::istream* p_from_child;
    std::ostream* p_to_child;
    std::istream* p_stderr;
    // std::string command;
    // std::string currentDir;

    void* p_inbuf;
    void* p_outbuf;
    void* p_errbuf;

    LProcess() = delete;
    LProcess(const std::string& commmand, const std::string& currentDir = ".");
    virtual ~LProcess();
    
    int getpid();
    void kill();
    Status wait();
    std::ostream& stdin();
    std::istream& stdout();
    std::istream& stderr();
    std::optional<int> getReturnValue();
    std::optional<int> getSignal(); // get the received signal
    void flush();
    bool isRunning();
    void checkStatus();
    void closeInPipe();
};
