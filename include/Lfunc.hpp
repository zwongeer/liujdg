#pragma once
#include <fstream>
#include <string>
#include <string_view>

// #define LExit(v) return (v)

#define LSHOW(a) (std::cout << "[" << #a << "]: " << (a) << std::endl);
#define LH (std::cout << "In f:" << __func__ << "\tfile:" << __FILE__ << "\tline:" << __LINE__ << std::endl);
#define LXD(a) (std::cout << #a << std::endl);

[[noreturn]] inline void LExit(int exit_code) noexcept { std::quick_exit(exit_code); }

// check whether a command can be excuted
bool LcheckCommand(const std::string& command);

// check whether a directory is writable
bool LcheckDirectory(const std::string& path);
bool LcheckDirectory(const std::string& path, std::string& errMessage);

// check whether a file exists
bool LcheckFileExists(const std::string& path);

void LfuncInit();

std::string_view LgetCommand(const char* command);

namespace liujdg {
    extern std::ofstream devNull;
}