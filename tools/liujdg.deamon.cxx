#include "deamon/all_apis.hpp"

std::unique_ptr<router_t> server_handler(const liujdg_deamon_config& config) {
    auto router = std::make_unique<router_t>();
    use_all_apis(router, config);

    router->non_matched_request_handler([](auto req) {
        return init_response(req->create_response(censoredByAttack204))
                .set_body(fmt::format("{}<br><del>Your ip({}) had been reported to Attack204!</del>",
                    censoredByAttack204.reason_phrase(), req->remote_endpoint().address().to_string()))
                .append_header( restinio::http_field::content_type, "text/html; charset=utf-8")
                .done();
    });
    return router;
}

int main(int argc, const char** args) {
    using namespace std::chrono;
    try {
        if (argc < 2)
            throw std::runtime_error(LINFO + "Need Config File!");
        liujdg_deamon_config config;
        nlohmann::json configFile;
        if (!LcheckFileExists(args[1]))
            throw std::runtime_error(LINFO + fmt::format("File `{}` not exists.", args[1]));
        std::ifstream in(args[1]);
        if (!in)
            throw std::runtime_error(LINFO + fmt::format("File `{}` unaccessible.", args[1]));
        
        configFile = nlohmann::json::parse(LreadFile(in));
        configFile.get_to(config);
        
        if (!LcheckDirectory(config.path))
            throw std::runtime_error(LINFO + fmt::format("The path `{}` is invalid", config.path));

        init_api(config);
        auto router = server_handler(config);
        
        if (config.https)
            serve_https(router, config);
        else
            serve_http(router, config);
    } catch (std::exception& err) {
        std::cerr << "Error: " << err.what() << std::endl;
        clean_api();
        return 1;
    }
    clean_api();
    return 0;
}