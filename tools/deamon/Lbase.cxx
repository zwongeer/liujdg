#include "Lapi.hpp"
#include "api_types.hpp"

#include "liujdg_version.hpp"

void Lapi_base(std::unique_ptr<router_t>& router, const liujdg_deamon_config& config)
{(void)config;router->add_handler(allMethods, "/", [](auto req, auto params) {
    std::string str_header;
    req->header().for_each_field([&](auto field) {
        str_header += fmt::format("{}: {}\n", field.name(), field.value());
    });
    return init_response_text(req->create_response())
        .set_body( fmt::format("Hello {}\nYou sent: \n{}\nPost:\n{}", liujdg::NAME + "-" + liujdg::VERSION, str_header, req->body()) )
        .done();
});}
