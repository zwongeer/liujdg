#include <new>

#include <boost/process.hpp>
#include <boost/filesystem.hpp>

#include "Lfunc.hpp"
#include "LProcess_sandboxed_c.hpp"

LProcess_sandboxed_c::LProcess_sandboxed_c() : LProcess_container(){

}

LProcess_sandboxed_c::LProcess_sandboxed_c(const std::string& command, int lim, const std::string& currentDir, const std::string& whitelist) {
    namespace bfs = boost::filesystem;
    
    if (!LcheckCommand(command) && !LcheckCommand(currentDir + command)) throw std::runtime_error(LINFO + "the command: `" + command + "` cannot be executed");
    bfs::path white_path = bfs::system_complete(bfs::path(currentDir) / bfs::path(whitelist));
    bfs::path cmd_path = bfs::system_complete(bfs::path(currentDir));
    std::string cmd = std::string(sandbox_options) + "--whitelist=" + white_path.string() + " "
            + "--whitelist=" + cmd_path.string() + " " + command;
    new(this) LProcess_container(cmd, lim, currentDir);
}

LProcess_sandboxed_c::LProcess_sandboxed_c(LProcess_sandboxed_c&& rhs) : LProcess_container(std::move(rhs)) {

}

LProcess_sandboxed_c& LProcess_sandboxed_c::operator = (LProcess_sandboxed_c&& rhs) {
    LProcess_container::operator=(std::move(rhs));
    return *this;
}

LProcess_sandboxed_c::~LProcess_sandboxed_c() {

}