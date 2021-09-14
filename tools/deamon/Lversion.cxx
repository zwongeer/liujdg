#include "Lapi.hpp"
#include "api_types.hpp"
#include "liujdg_version.hpp"


void Lapi_version(std::unique_ptr<router_t>& router, const liujdg_deamon_config& config)
{(void)config;router->add_handler(allMethods, "/version", [](auto req, auto params) {
    return init_response_text(req->create_response())
            .set_body(std::string(liujdg::NAME) + " " + liujdg::VERSION)
            .done();
});}