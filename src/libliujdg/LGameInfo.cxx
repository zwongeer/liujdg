#include <filesystem>
#include <fstream>
#include <stdexcept>
#include <string>
#include <vector>

#include <unistd.h>

#include "rapidjson/document.h"
#include "rapidjson/prettywriter.h"
#include "rapidjson/stringbuffer.h"

#include "Lfunc.hpp"
#include "LGameInfo.hpp"

LGameInfo::LGameInfo(const std::string& json) {
    std::ifstream in(json.c_str());
    if (!in.good()) throw std::runtime_error("Cannot open the json file `" + json + '`');

    if (std::filesystem::status(json.c_str()).type() == std::filesystem::file_type::directory)
        throw std::runtime_error("`" + json + "`" + "is a directory");

    size_t fileSize = size_t( std::filesystem::file_size(json.c_str()) );
    std::string data;
    data.reserve(fileSize + 8);
    int ch;
    while (ch = in.get(), ch != EOF)
        data += char(ch);
    in.close();
    fromString(data);
}

void LGameInfo::fromString(const std::string& str) {
    rapidjson::Document doc;
    bool flag;
    if (doc.Parse(str.c_str()).HasParseError())
        throw std::runtime_error("Bad JSON file");
    if (doc.HasMember(JUDGER) && doc[JUDGER].IsObject()) {
        persons.push_back(LPerson::readFromJSON(doc[JUDGER], &flag));
        if (!flag) throw std::runtime_error("An error occurred when reading the judger from json");
    } else throw std::runtime_error("Cannot find the judger from json");
    if (doc.HasMember(PLAYERS) && doc[PLAYERS].IsArray()) {
        rapidjson::Value& array = doc[PLAYERS];
        for (int i = 0; i < array.Size(); ++i) {
            persons.push_back(LPerson::readFromJSON(array[i], &flag));
            if (!flag) throw std::runtime_error("An error occurred when reading players from json");
        }
    } else throw std::runtime_error("Cannot find players from json");
    if (doc.HasMember(CONFIG) && doc[CONFIG].IsObject())
        this->config = Ljdgconfig::readFromJSON(doc[CONFIG]);
    else throw std::runtime_error("Cannot find config from json");
}

std::string LGameInfo::toString() {
    // add players
    std::string ret = R"({"players": [)";
    nlohmann::json j;
    for (int i = 1; i < persons.size(); ++i) {
        j.clear();
        to_json(j, persons[i]);
        ret += j.dump();
        if (i != persons.size() - 1) ret += ",";
    }
    ret += R"(],"judger":)";
    j.clear();
    to_json(j, persons[0]);
    ret += j.dump();
    ret += R"(,"config":)";
    j.clear();
    to_json(j, config);
    ret += j.dump() + "}";
    return ret;
}

int LGameInfo::getPeopleNum() {
    return persons.size();
}