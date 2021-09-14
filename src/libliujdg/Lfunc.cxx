#include <cstdlib>
#include <cstring>
#include <ctime>
#include <filesystem>

#include <boost/process/search_path.hpp>

#include "Lfunc.hpp"

bool LcheckCommand(const std::string& command) {
    return boost::process::search_path(command).empty();
}

bool LcheckDirectory(const std::string& path, std::string& errMessage) {
    namespace fs = std::filesystem;
    if (fs::exists(path)) {
        if (fs::status(path).type() == fs::file_type::directory) {
            // if (::access(path.c_str(), W_OK) != 0) {
            //     errMessage = '`' + path + '`' + "is not writable";
            //     return false;
            // }
        }
        else {
            errMessage = "`" + path + "` is not a directory";
            return false;
        }
    } else {
        errMessage = "Bad path:" + path;
        return false;
    }
    return true;
}

bool LcheckDirectory(const std::string& path) {
    namespace fs = std::filesystem;
    // return fs::exists(path) && fs::status(path).type() == fs::file_type::directory && ::access(path.c_str(), W_OK) == 0;
    return fs::exists(path) && fs::status(path).type() == fs::file_type::directory;
}

bool LcheckFileExists(const std::string& path) {
    namespace fs = std::filesystem;
    // return ::access(path.c_str(), F_OK) == 0;
    return fs::exists(path) && fs::status(path).type() != fs::file_type::directory;
}

void LfuncInit() {
    srand(time(nullptr));
}

std::string LreadFile(std::istream& in, size_t len) {
    std::string data = "";
    data.reserve(len);
    int ch;
    while (ch = in.get(), ch != EOF)
        data += char(ch);
    return data;
}

std::optional<std::string> LgetFilePath(const std::string& file) {
    namespace fs = std::filesystem;
    if (fs::exists(file) && fs::status(file).type() != fs::file_type::directory)
        return fs::path(file).parent_path().string();
    return {};
}

std::string_view LgetCommand(const char* command) {
    if (command[0] != '#') return {};
    size_t pos;
    size_t len = std::strlen(command);
    for (pos = 0; pos < len; ++pos) {
        if (command[pos] == ' ')
            break;
    }
    if (len == pos) return std::string_view(command);
    return std::string_view(command, pos);
}

namespace liujdg {
    std::ofstream devNull(NULLFILE);
}