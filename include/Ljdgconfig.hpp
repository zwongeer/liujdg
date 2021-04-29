#pragma once
#include <string>

#include "rapidjson/document.h"
#include "rapidjson/prettywriter.h"
#include "rapidjson/stringbuffer.h"

struct Ljdgconfig {
    static constexpr const char* LOGDIR = "logdir";
    static constexpr const char* BASEDIR = "basedir";
    static constexpr const char* TIMELIMIT = "timeLimit";
    
    static Ljdgconfig readFromJSON(const rapidjson::Value&);

    std::string logdir; // default "" (not loging the stdout)
    std::string basedir; // default "." (current directory)
    int timeLimit; // default 3 seconds if it is negative or too large or not included in the .json file
    
    Ljdgconfig() = default;
    Ljdgconfig(Ljdgconfig&&) = default;
    Ljdgconfig& operator = (Ljdgconfig&&) = default;
};