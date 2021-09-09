#pragma once
#include <string>

#include <rapidjson/document.h>
#include <rapidjson/prettywriter.h>
#include <rapidjson/stringbuffer.h>

struct Ljdgconfig {
    static constexpr const char* LOGDIR = "logdir";
    static constexpr const char* BASEDIR = "basedir";
    static constexpr const char* TIMELIMIT = "timeLimit";
    static constexpr const char* BUILDTIMELIMIT = "buildTimeLimit";
    static constexpr const char* ERRORLOG = "errorLog";

    struct defaults {
        static constexpr const char* LOGDIR = "";  // default "" (not loging the stdout)
        static constexpr const char* BASEDIR = "."; // default "." (current directory)
        static constexpr int TIMELIMIT = 3; // default 3 seconds if it is negative or too large or not included in the .json file
        static constexpr int BUILDTIMELIMIT = 10;
        static constexpr const char* ERRORLOG = "";
    };
    
    static Ljdgconfig readFromJSON(const rapidjson::Value&);

    std::string logdir = defaults::LOGDIR; // sava the data for a game
    std::string basedir = defaults::BASEDIR;
    int timeLimit = defaults::TIMELIMIT;
    int buildTimeLimit = defaults::BUILDTIMELIMIT;
    std::string errorLog = defaults::ERRORLOG; // path for logging the error messages 
    
    Ljdgconfig() = default;
    Ljdgconfig(Ljdgconfig&&) = default;
    Ljdgconfig& operator = (Ljdgconfig&&) = default;
};