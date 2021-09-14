#pragma once
#include <chrono>
#include <fstream>
#include <optional>
#include <string>
#include <string_view>

#include <fmt/core.h>

#define LINFO fmt::format("In f:{}  file:{} line:{}\n", __func__, __FILE__, __LINE__)
#define LSHOW(a) (std::cout << "[" << #a << "]: " << (a) << std::endl);
#define LH (std::cout << LINFO << std::endl);
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

inline auto Lget_seconds_since_epoch() {
    auto now = std::chrono::system_clock::now().time_since_epoch();
    return std::chrono::duration_cast<std::chrono::seconds>(now).count();
}

// get the location of a file if existed
std::optional<std::string> LgetFilePath(const std::string& file);

namespace liujdg {
    #if defined(WIN32) || defined(_WIN32) || defined(__WIN32)
    constexpr const char* NULLFILE = "c:\\nul";
    constexpr const char* BUILD = "liujdg.build.exe -";
    constexpr const char* RUN = "liujdg.run.exe -";
    #else
    constexpr const char* NULLFILE = "/dev/null";
    constexpr const char* BUILD = "liujdg.build -";
    constexpr const char* RUN = "liujdg.run -";
    #endif
    extern std::ofstream devNull;
}