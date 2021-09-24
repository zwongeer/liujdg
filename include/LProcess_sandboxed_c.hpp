#pragma once
#include "LProcess_container.hpp"

namespace liujdg {
    constexpr const char* const sandbox_options =
        "firejail --quiet "
        "--net=none "
        "--private-tmp "
        "--private-dev "
        "--read-only=/ "
        "--rlimit-fsize=16777216 "
        "--rlimit-nofile=32 "
        "--rlimit-as=256m "
        "--rlimit-nproc=4 ";
    extern LProcess_container create_sandboxed_container(const std::string& str, int lim = 3, const std::string& currentDir = ".", const std::string& whitelist = "/tmp");
}
