#pragma once

#include <string>
#include <vector>

#include <nlohmann/json.hpp>

#include <rapidjson/document.h>
#include <rapidjson/prettywriter.h>
#include <rapidjson/stringbuffer.h>

#include "LPlayerResult.hpp"

struct LGameResult {
    static constexpr const char* PLAYERS = "players";
    static constexpr const char* COMMENTS = "comments";

    std::vector<LPlayerResult> playersResults;
    std::string comments;

    LGameResult() = default;
    LGameResult(LGameResult&&) = default;
    LGameResult& operator = (LGameResult&&) = default;

    void writeToFile(const std::string& path);
};