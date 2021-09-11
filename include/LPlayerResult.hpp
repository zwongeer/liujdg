#pragma once
#include <cstdint>
#include <string>


#include <nlohmann/json.hpp>

#include <rapidjson/document.h>
#include <rapidjson/prettywriter.h>
#include <rapidjson/stringbuffer.h>

struct LPlayerResult {
    static constexpr const char* NAME = "name";
    static constexpr const char* ID = "id";
    static constexpr const char* SCORES = "scores";
    static constexpr const char* ISWINNER = "isWinner";

    static LPlayerResult readFromJSON(const rapidjson::Value&, bool* isok = nullptr);

    std::string name;
    std::string id;
    int64_t scores;
    bool isWinner;

    LPlayerResult() = default;
    LPlayerResult(LPlayerResult&&) = default;
    LPlayerResult& operator = (LPlayerResult&&) = default;

    void writeToJSON(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer);
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(LPlayerResult, name, id, scores, isWinner)