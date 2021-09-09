#pragma once
#include <string>

#include <rapidjson/document.h>
#include <rapidjson/prettywriter.h>
#include <rapidjson/stringbuffer.h>

struct LPerson {
    static constexpr const char* NAME = "name";
    static constexpr const char* ID = "id";
    static constexpr const char* BUILD = "build";
    static constexpr const char* RUN = "run";
    static constexpr const char* FOLDER = "folder";
    static constexpr const char* BASEDIR = "basedir";

    static LPerson readFromJSON(const rapidjson::Value&, bool* isok = nullptr);

    std::string name;
    std::string id;
    std::string build;
    std::string run;
    std::string folder;
    std::string basedir;

    LPerson() = default;
    LPerson(LPerson&&) = default;
    LPerson& operator = (LPerson&&) = default;
};