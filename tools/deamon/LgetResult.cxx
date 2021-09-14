#include "Lapi.hpp"
#include "api_types.hpp"


void Lapi_getResult(std::unique_ptr<router_t>& router, const liujdg_deamon_config& config)
{router->http_get("/getResult", [](auto req, auto params) {
    try {
        
        return init_response_text(req->create_response())
            .set_body("getResult")
            .done();
    } catch (std::exception& e) {
        nlohmann::json ret = {{"status", api_status_ERROR}, {"message", e.what()}};
        return init_response_json(req->create_response(restinio::status_bad_request()))
            .set_body(ret.dump(4))
            .done();
    }
});
}