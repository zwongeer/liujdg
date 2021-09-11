#pragma once

#include <map>
#include <string>
#include <queue>
#include <utility>
#include <mutex>

#include <soci/soci.h>
#include <soci/sqlite3/soci-sqlite3.h>

#include "LGameInfo.hpp"

const int CONTENT_LENGTH_LIMIT = 8196;

extern soci::session sql_session;
// store the info and id for running tasks
typedef std::pair<LGameInfo, std::string> task_;
extern std::queue<task_> processes;
// protect processes
extern std::mutex mutex_pro;
typedef std::lock_guard<std::mutex> lockguard;

namespace api_status {
    const std::string ERROR = "error"; // error request
    const std::string BUILDING = "building"; // building
    const std::string BUILDERROR = "buildError"; // compile errror
    const std::string RUNTIME_ERROR = "runtimeError"; // runtime error
    const std::string RECEIVED = "received"; // received json
    const std::string RUNNING = "running"; // game is running
    const std::string FULFILLED = "fulfilled"; // game is funished normally
    const std::string UNKNOWN = "unknown"; //unknown error
}

const auto allMethods = restinio::router::none_of_methods(
    // restinio::http_method_get(),
    // restinio::http_method_head(),
    // restinio::http_method_post(),
    // restinio::http_method_put(),
    // restinio::http_method_delete(),
    // restinio::http_method_connect(),
    // restinio::http_method_options(),
    // restinio::http_method_trace(),
    // restinio::http_method_patch()
);