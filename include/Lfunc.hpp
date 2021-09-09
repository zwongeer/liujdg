#pragma once
#include <fstream>
#include <optional>
#include <string>
#include <string_view>

#define LSHOW(a) (std::cout << "[" << #a << "]: " << (a) << std::endl);
#define LH (std::cout << "In f:" << __func__ << "\tfile:" << __FILE__ << "\tline:" << __LINE__ << std::endl);
#define LXD(a) (std::cout << #a << std::endl);
#define LExit(v) return (v)
// [[noreturn]] inline void LExit(int exit_code) noexcept { std::quick_exit(exit_code); }

// do nothing
inline void lpass(...){}

// check whether a command can be excuted
// note this function check the prefix first
bool LcheckCommand(const std::string& command);

// check whether a directory is writable
bool LcheckDirectory(const std::string& path);
bool LcheckDirectory(const std::string& path, std::string& errMessage);

// check whether a file exists
bool LcheckFileExists(const std::string& path);

void LfuncInit();

std::string LreadFile(std::istream& in, size_t len = 512);

std::string_view LgetCommand(const char* command);

// get the location of a file if existed
std::optional<std::string> LgetFilePath(const std::string& file);

namespace liujdg {
    extern std::ofstream devNull;
}