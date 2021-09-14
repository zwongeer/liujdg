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
        
        getStatus_and_path_fromSql(id, status, logpath);

        nlohmann::json ret = {
            {"status", status},
            {"logpath", logpath}
        };
        return init_response_json(req->create_response())
            .set_body(ret.dump(4))
            .done();
    } catch (std::exception& e) {
        nlohmann::json ret = {{"status", api_status_ERROR}, {"message", e.what()}};
        return init_response_json(req->create_response(restinio::status_bad_request()))
            .set_body(ret.dump(4))
            .done();
    }
});

router->http_get("/get", [](auto req, auto params) {
    try {
        auto qp = restinio::parse_query(req->header().query());
        // std::string id = restinio::utils::unescape_percent_encoding(params["id"]);
        if (!qp.has("id")) throw std::runtime_error(LINFO + "Need `id`");
        std::string id(qp["id"]);
        std::string status, logpath;

        getStatus_and_path_fromSql(id, status, logpath);

        nlohmann::json ret = {
            {"status", status},
            {"logpath", logpath}
        };
        return init_response_json(req->create_response())
            .set_body(ret.dump(4))
            .done();
    } catch (std::exception& e) {
        nlohmann::json ret = {{"status", api_status_ERROR}, {"message", e.what()}};
        return init_response_json(req->create_response(restinio::status_bad_request()))
            .set_body(ret.dump(4))
            .done();
    }
});
}