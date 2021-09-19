#include <boost/filesystem.hpp>

#include "Lfunc.hpp"
#include "LProcess_sandboxed_c.hpp"

namespace liujdg {
LProcess_container create_sandboxed_container(const std::string& command, int lim, const std::string& currentDir, const std::string& whitelist) {
    namespace bfs = boost::filesystem;
    
    if (!LcheckCommand(command) && !LcheckCommand(currentDir + command)) throw std::runtime_error(LINFO + "the command: `" + command + "` cannot be executed");
    bfs::path white_path = bfs::system_complete(bfs::path(currentDir) / bfs::path(whitelist));
    bfs::path cmd_path = bfs::system_complete(bfs::path(currentDir));
    std::string cmd = std::string(sandbox_options) + "--whitelist=" + white_path.string() + " "
            + "--whitelist=" + cmd_path.string() + " " + command;
    return LProcess_container(cmd, lim, currentDir);
}

}