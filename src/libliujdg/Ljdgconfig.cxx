#include <climits>
#include <stdexcept>

#include "Ljdgconfig.hpp"

Ljdgconfig Ljdgconfig::readFromJSON(const rapidjson::Value& v) {
    if (!v.IsObject()) throw std::runtime_error("Syntax error happened when reading config from json");
    Ljdgconfig config;
    if (v.HasMember(LOGDIR) && v[LOGDIR].IsString())
        config.logdir = v[LOGDIR].GetString();
    
    if (v.HasMember(BASEDIR) && v[BASEDIR].IsString())
        config.basedir = v[BASEDIR].GetString();
    
    if (config.logdir != "" && config.logdir.back() != '/')
        config.logdir.push_back('/');
    
    if (config.basedir.back() != '/')
        config.basedir.push_back('/');

    if (config.basedir == "") config.basedir = defaults::BASEDIR;

    if (v.HasMember(TIMELIMIT) && v[TIMELIMIT].IsUint() && v[TIMELIMIT].GetUint() <= unsigned(INT_MAX))
        config.timeLimit = v[TIMELIMIT].GetUint();

    if (v.HasMember(BUILDTIMELIMIT) && v[BUILDTIMELIMIT].IsUint() && v[BUILDTIMELIMIT].GetUint() <= unsigned(INT_MAX))
        config.buildTimeLimit = v[BUILDTIMELIMIT].GetUint();

    if (v.HasMember(ERRORLOG) && v[ERRORLOG].IsString())
        config.errorLog = v[ERRORLOG].GetString();

    return config;
}