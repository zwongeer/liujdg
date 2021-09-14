#include <fstream>
#include <string>
#include <vector>

#include "rapidjson/prettywriter.h"
#include "rapidjson/stringbuffer.h"

#include "LGameResult.hpp"

void LGameResult::writeToFile(const std::string& path) {
    rapidjson::StringBuffer strbuf;
    rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(strbuf);
    writer.SetIndent(' ', 2);

    writer.StartObject();

    writer.Key(PLAYERS);
    writer.StartArray();

    // note 0 are reserved for the judger
    for (int i = 1; i < playersResults.size(); ++i)
        playersResults[i].writeToJSON(writer);
    
    writer.EndArray(); // End players
    
    writer.Key(COMMENTS);
    writer.String(comments.c_str());

    writer.EndObject(); // End of json
    
    std::ofstream out(path.c_str());
    if (!out) throw std::runtime_error("Cannot write GameResult to `" + path + "`");
    out << strbuf.GetString() << std::endl;
}