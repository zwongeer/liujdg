#pragma once

#include <string>
#include <vector>

#include "Ljdgconfig.hpp"
#include "LPerson.hpp"
#include "LProcess_container.hpp"

struct LGameInfo {
    static constexpr const char* PLAYERS = "players";
    static constexpr const char* JUDGER = "judger";
    static constexpr const char* CONFIG = "config";

    std::vector<LPerson> persons; // note that 0 are reserved for the judger
    Ljdgconfig config;
    
    LGameInfo(const std::string& json, bool noCheck = false); // check build command and run command
    LGameInfo() = default;
    LGameInfo(LGameInfo&&) = default;
    LGameInfo& operator = (LGameInfo&&) = default;

    int getPeopleNum();
    void fromString(const std::string& str, bool noCheck = false);
};