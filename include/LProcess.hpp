#pragma once

#include <atomic>
#include <iostream>
#include <string>

struct LProcess {
    std::atomic<bool> killed;
    pid_t pid;
    std::atomic<int> status;
    int pipestdinfd[2];
    int pipestdoutfd[2];

    int fd_from_child;
    int fd_to_child;
    FILE* c_p_from_child;
    FILE* c_p_to_child;
    std::istream* p_from_child;
    std::ostream* p_to_child;
    // std::string command;
    // std::string currentDir;


    LProcess() = delete;
    LProcess(const std::string& commmand, const std::string& currentDir = ".");
    virtual ~LProcess();
    
    int getpid();
    void kill();
    int wait();
    std::ostream& stdin();
    std::istream& stdout();
    int getReturnValue(); // return -1024 when getting an error
    void flush();
    bool isRunning();
};
