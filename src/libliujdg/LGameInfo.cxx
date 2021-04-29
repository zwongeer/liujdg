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

    rapidjson::Document doc;
    bool flag;
    if (doc.Parse(data.c_str()).HasParseError())
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
        config = Ljdgconfig::readFromJSON(doc[CONFIG]);
    else throw std::runtime_error("Cannot find config from json");

    // check the build and run command
    for (int i = 0; i < getPeopleNum(); ++i) {
        if (persons[i].build != "" && persons[i].build != "."  && !LcheckCommand(persons[i].build))
            throw std::runtime_error("Invalid build command for user:" + persons[i].name + ",id:" + persons[i].id);
        if (!LcheckCommand(persons[i].run) && !LcheckCommand(persons[i].basedir + persons[i].run))
            throw std::runtime_error("Invalid run command for user:" + persons[i].name + ",id:" + persons[i].id);
    }
}

int LGameInfo::getPeopleNum() {
    return persons.size();
}