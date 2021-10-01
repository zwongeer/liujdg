#pragma once
#include "LProcess_container.hpp"

namespace liujdg {
    constexpr const char* const sandbox_options =
        "firejail --quiet "
        "--net=none "
        "--private-tmp "
        "--private-dev "
        // "--read-only=/ "
        "--rlimit-fsize=16777216 "
        "--rlimit-nofile=32 "
        "--rlimit-as=1073741824 "
        "--rlimit-nproc=16 ";
    extern LProcess_container create_sandboxed_container(const std::string& str, int lim = 3, const std::string& currentDir = ".", const std::string& whitelist = "/tmp");
}
