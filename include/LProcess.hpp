#pragma once

#include <atomic>
#include <iostream>
#include <memory>
#include <string>
#include <optional>

#include <boost/process.hpp>

struct LProcess {
    bool inPipeClosed;
    boost::process::child c;
    boost::process::ipstream stdout_;
    boost::process::ipstream stderr_;
    std::unique_ptr<boost::process::opstream> pstdin_;

    LProcess() = delete;
    LProcess(const std::string& commmand, const std::string& currentDir = ".");
    virtual ~LProcess();
    
    int getpid();
    void kill();
    void wait();
    std::ostream& getStdin();
    std::istream& getStdout();
    std::istream& getStderr();
    std::optional<int> getReturnValue();
    bool isRunning();
    void closeInPipe();
};
