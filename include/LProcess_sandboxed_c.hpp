#pragma once
#include "LProcess_container.hpp"

struct LProcess_sandboxed_c : LProcess_container {
    static constexpr const char* const sandbox_options =
        "firejail --quiet "
        "--net=none "
        "--private-tmp "
        "--private-dev "
        "--read-only=/ "
        "--rlimit-fsize=16m "
        "--rlimit-nofile=32 "
        "--rlimit-as=256m "
        "--rlimit-nproc=4 ";
    virtual ~LProcess_sandboxed_c();
    LProcess_sandboxed_c();
    LProcess_sandboxed_c(const std::string& str, int lim = 3, const std::string& currentDir = ".", const std::string& whitelist = "/tmp");
    LProcess_sandboxed_c(const LProcess_sandboxed_c&) = delete;
    LProcess_sandboxed_c(LProcess_sandboxed_c&&);
    LProcess_sandboxed_c& operator = (LProcess_sandboxed_c&&);
};
