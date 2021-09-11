#include <filesystem>
namespace fs = std::filesystem;
#include "Lapi.hpp"
#include "api_types.hpp"

void Lapi_new(std::unique_ptr<router_t>& router, const liujdg_deamon_config& config)
{router->http_post("/new", [](auto req, auto params) {
    if (req->header().content_length() > CONTENT_LENGTH_LIMIT)
        return init_response_text(req->create_response(restinio::status_bad_request()))
            .done();
    LGameInfo info;
    // game id
    std::string id = get_unique_id();

    // the absolute path
    std::string realLogdir;
    try {
        info.fromString(req->body());
        realLogdir = (fs::path(info.config.basedir) / fs::path(info.config.logdir)).string();
        lockguard lock(mutex_pro);
        sql_session << "insert into tasks (id, status, logpath) values (:id, :status, :logpath);"
            , soci::use(id), soci::use(api_status::RECEIVED), soci::use(realLogdir);
        task_ task = { std::move(info), id };
        processes.push(std::move(task));
        // processes.emplace(info, id);
    } catch (std::exception& e) {
        nlohmann::json ret = {{"status", api_status::ERROR}, {"message", e.what()}};
        return init_response_json(req->create_response(restinio::status_bad_request()))
            .set_body(ret.dump(4))
            .done();
    }
    nlohmann::json ret = {{"status", api_status::RECEIVED}, {"id", id} };
    return init_response_text(req->create_response())
            .set_body(ret.dump(4))
            .done();
});}