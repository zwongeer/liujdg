#include "Lapi.hpp"
#include "api_types.hpp"

void Lapi_test(std::unique_ptr<router_t>& router, const liujdg_deamon_config& config)
{router->http_post("/test", [](auto req, auto params) {
    if (req->header().content_length() > CONTENT_LENGTH_LIMIT)
        return init_response_text(req->create_response(restinio::status_bad_request()))
            .done();
    
    return init_response_text(req->create_response())
            .set_body("Preserved")
            .done();
});}