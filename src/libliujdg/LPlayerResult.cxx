#include "LPlayerResult.hpp"

LPlayerResult LPlayerResult::readFromJSON(const rapidjson::Value& v, bool* isok) {
    LPlayerResult ret;
    if (isok != nullptr) *isok = false;
    if (v.IsObject()) {
        if (v.HasMember(NAME) && v[NAME].IsString())
            ret.name = v[NAME].GetString();
        else return ret;

        if (v.HasMember(ID) && v[ID].IsString())
            ret.id = v[ID].GetString();
        else return ret;

        if (v.HasMember(SCORES) && v[SCORES].IsString())
            ret.scores = v[SCORES].GetInt64();
        else return ret;

        if (v.HasMember(ISWINNER) && v[ISWINNER].IsString())
            ret.isWinner = v[ISWINNER].GetBool();
        else return ret;
    } else return ret;
    if (isok != nullptr) *isok = true;
    return ret;
}

void LPlayerResult::writeToJSON(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer) {
    writer.StartObject();

    writer.Key(NAME);
    writer.String(name.c_str());

    writer.Key(ID);
    writer.String(id.c_str());

    writer.Key(SCORES);
    writer.Int64(scores);

    writer.Key(ISWINNER);
    writer.Bool(isWinner);
    
    writer.EndObject();
}