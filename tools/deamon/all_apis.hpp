// This file is auto generated. Do not edit.
// you are expected not to include `Lapi.hpp` outside the `deamon` folder, use `all_apis.hpp` instead, or compile error may happen
// you should not include `all_apis.hpp` inside the `deamon` folder, or compilation time would increase significantly.
#pragma once
#include <memory>

#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <fmt/core.h>
#include <restinio/all.hpp>
#include <nlohmann/json.hpp>

#include "Lfunc.hpp"

const restinio::http_status_line_t censoredByAttack204 = { restinio::http_status_code_t(451), "Censored By Attack204" };
using router_t = restinio::router::express_router_t<>;

struct liujdg_deamon_config {
    struct defaults {
        static constexpr int PORT = 1984;
        static constexpr bool HTTPS = false;
    };
    int port; // listening port
    std::string path; // the path to store data
    bool https; // use https
    std::string cert; // https cert
    std::string key; // key
};

void to_json(nlohmann::json& j, const liujdg_deamon_config& cfg);
void from_json(const nlohmann::json& j, liujdg_deamon_config& cfg);

inline std::string get_unique_id() {
    return fmt::format("{}-{}",
        boost::uuids::to_string(boost::uuids::random_generator()()),
        Lget_seconds_since_epoch());
}

void init_api(const liujdg_deamon_config& config);
// do some clean job
void clean_api();

void serve_http(std::unique_ptr<router_t>& router, const liujdg_deamon_config& config);
void serve_https(std::unique_ptr<router_t>& router, const liujdg_deamon_config& config);

template <typename RESPONSE>
RESPONSE init_response(RESPONSE&& response) {
    response.append_header("Server", "Attack204's Potatoes & Apples");
    response.append_header("Who-is-txdy", "Attack204");
    return std::move(response);
}
template <typename RESPONSE>
inline RESPONSE init_response_text(RESPONSE&& response) {
    return std::move(init_response(std::move(response))
        .append_header("Content-Type", "text/plain; charset=utf-8" ));
}
template <typename RESPONSE>
inline RESPONSE init_response_json(RESPONSE&& response) {
    return std::move(init_response(std::move(response))
        .append_header("Content-Type", "application/json; charset=utf-8" ));
}
void Lapi_test(std::unique_ptr<router_t>& router, const liujdg_deamon_config& config);
void Lapi_version(std::unique_ptr<router_t>& router, const liujdg_deamon_config& config);
void Lapi_get(std::unique_ptr<router_t>& router, const liujdg_deamon_config& config);
void Lapi_base(std::unique_ptr<router_t>& router, const liujdg_deamon_config& config);
void Lapi_new(std::unique_ptr<router_t>& router, const liujdg_deamon_config& config);

inline void use_all_apis(std::unique_ptr<router_t>& router, const liujdg_deamon_config& config) {
Lapi_test(router, config);
Lapi_version(router, config);
Lapi_get(router, config);
Lapi_base(router, config);
Lapi_new(router, config);
}