#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <fstream>
#include <filesystem>
#include <iostream>
#include <mutex>
#include <stdexcept>
#include <string>

#include "Lfunc.hpp"
#include "LProcess.hpp"

#include <boost/process.hpp>
// #include <boost/process/extend.hpp>

LProcess::LProcess (const std::string& command, const std::string& currentDir) {
    namespace fs = std::filesystem;
    namespace bp = boost::process;
    inPipeClosed = false;
    if (!LcheckCommand(command) && !LcheckCommand(currentDir + command)) throw std::runtime_error("the command: `" + command + "` cannot be executed");
    if (!LcheckDirectory(currentDir)) throw std::runtime_error("cannot access the directory:[" + currentDir + "]");
    
    std::error_code ec;
    fs::path cwd;
    pstdin_ = std::make_unique<boost::process::opstream>();
    cwd = fs::current_path();
    fs::current_path(currentDir);
    c = bp::child(command,
        // boost::process::extend::on_setup = [&](auto & exec)->void{fs::current_path(currentDir);},
        bp::std_in < *pstdin_, bp::std_out > stdout_, bp::std_err > stderr_);
    fs::current_path(cwd);
    
    stdout_.tie(pstdin_.get());
    stderr_.tie(pstdin_.get());
}

LProcess::~LProcess() {
    std::error_code ec;
    c.terminate(ec);
    c.wait(ec);
}

void LProcess::wait() {
    std::error_code ec;
    c.wait(ec);
}

std::ostream& LProcess::getStdin() {
    return *pstdin_;
}

std::istream& LProcess::getStdout() {
    return stdout_;
}

std::istream& LProcess::getStderr() {
    return stderr_;
}

bool LProcess::isRunning() {
    std::error_code ec;
    bool f = c.running(ec);
    if (ec) return false;
    return f;
}

std::optional<int> LProcess::getReturnValue() {
    if (!isRunning()) return c.exit_code();
    return {};
}

void LProcess::kill() {
    std::error_code ec;
    c.terminate(ec);
}

void LProcess::closeInPipe() {
    stderr_.tie(nullptr);
    stdout_.tie(nullptr);
    pstdin_->close();
    pstdin_ = std::make_unique<boost::process::opstream>();
    inPipeClosed = true;
}

int LProcess::getpid () {
    return c.id();
}
