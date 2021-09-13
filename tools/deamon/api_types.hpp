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
extern soci::backend_factory const &backEnd;
// store the info and id for running tasks
typedef std::pair<LGameInfo, std::string> task_;
extern std::queue<task_> processes;
// protect processes
extern std::mutex mutex_pro;
typedef std::lock_guard<std::mutex> lockguard;

void getStatus_and_path_fromSql(const std::string& id, std::string& status, std::string& path);
std::string url_encode(const char* msg);


const std::string api_status_ERROR = "error"; // error request
const std::string api_status_BUILDING = "building"; // building
const std::string api_status_BUILDERROR = "buildError"; // compile errror
const std::string api_status_RUNTIME_ERROR = "runtimeError"; // runtime error
const std::string api_status_RECEIVED = "received"; // received json
const std::string api_status_RUNNING = "running"; // game is running
const std::string api_status_FULFILLED = "fulfilled"; // game is funished normally
const std::string api_status_UNKNOWN = "unknown"; //unknown error

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