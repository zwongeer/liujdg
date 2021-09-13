#include "LPerson.hpp"

LPerson LPerson::readFromJSON(const rapidjson::Value& v, bool* isok) {
    LPerson person;
    if (isok != nullptr) *isok = false;
    if (v.IsObject()) {
        if (v.HasMember(NAME) && v[NAME].IsString())
            person.name = v[NAME].GetString();
        else return person;

        if (v.HasMember(ID) && v[ID].IsString())
            person.id = v[ID].GetString();
        else return person;

        if (v.HasMember(BUILD) && v[BUILD].IsString())
            person.build = v[BUILD].GetString();
        else return person;

        if (v.HasMember(RUN) && v[RUN].IsString())
            person.run = v[RUN].GetString();
        else return person;

        if (v.HasMember(FOLDER) && v[FOLDER].IsString()) {
            person.folder = v[FOLDER].GetString();
            if (person.folder == "")
                person.folder = "./";
            else if (person.folder.back() != '/')
                person.folder.push_back('/');
        }
        else return person;
        if (v.HasMember(BASEDIR) && v[BASEDIR].IsString()) {
            person.basedir = v[BASEDIR].GetString();
            if (person.basedir == "")
                person.basedir = "./";
            else if (person.basedir.back() != '/')
                person.basedir.push_back('/');
        }
        else return person;
    } else return person;
    if (isok != nullptr) *isok = true;
    return person;
}