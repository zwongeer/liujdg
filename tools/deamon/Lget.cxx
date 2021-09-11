#include "Lapi.hpp"
#include "api_types.hpp"

#include "LGameInfo.hpp"

void Lapi_get(std::unique_ptr<router_t>& router, const liujdg_deamon_config& config)
{router->http_post("/get", [](auto req, auto params) {
    if (req->header().content_length() > CONTENT_LENGTH_LIMIT)
        return init_response_text(req->create_response(restinio::status_bad_request()))
            .done();
    
    try {
        nlohmann::json j = nlohmann::json::parse(req->body());
        if (!j.at("id").is_string()) throw std::runtime_error("`id` is not string");
        std::string id = j.at("id").get<std::string>();
        std::string status, logpath;
        soci::indicator ind;

        sql_session << "select status from tasks where id = :id;", soci::into(status, ind), soci::use(id);
        if (ind != soci::i_ok) throw std::runtime_error("unable to find `status` by `id`");

        sql_session << "select logpath from tasks where id = :id;", soci::into(logpath, ind), soci::use(id);
        if (ind != soci::i_ok) throw std::runtime_error("unable to find `logpath` by `id`");

        nlohmann::json ret = {
            {"status", status},
            {"logpath", logpath}
        };
        return init_response_json(req->create_response())
            .set_body(ret.dump(4))
            .done();
    } catch (std::exception& e) {
        nlohmann::json ret = {{"status", api_status::ERROR}, {"message", e.what()}};
        return init_response_json(req->create_response(restinio::status_bad_request()))
            .set_body(ret.dump(4))
            .done();
    }
    // this may never happen
    return init_response_text(req->create_response())
            .done();
});}