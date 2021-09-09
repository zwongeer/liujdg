#include "deamon/all_apis.hpp"

std::unique_ptr<router_t> server_handler() {
    auto router = std::make_unique<router_t>();
    auto allMethods = restinio::router::none_of_methods(
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

    router->add_handler(allMethods, "/version", [](auto req, auto params) {
        return init_response(req->create_response())
                .set_body(liujdg::NAME + " " + liujdg::VERSION)
                .done();
    });

    router->add_handler(allMethods, "/", [](auto req, auto params) {
        std::string str_header;
        req->header().for_each_field([&](auto field) {
            str_header += fmt::format("{}: {}\n", field.name(), field.value());
        });
        return init_response(req->create_response())
                .set_body( fmt::format("Hello {}\nYou sent: \n{}\nPost:\n{}", liujdg::NAME + "-" + liujdg::VERSION, str_header, req->body()) )
                .done();
    });

    use_all_apis(router);

    router->non_matched_request_handler([](auto req) {
        return init_response(req->create_response(censoredByAttack204))
                .set_body(fmt::format("{}<br><del>Your ip({}) had been reported to Attack204!</del>",
                    censoredByAttack204.reason_phrase(), req->remote_endpoint().address().to_string()))
                .append_header( restinio::http_field::content_type, "text/html; charset=utf-8")
                .done();
    });
    return router;
}

struct liujdg_deamon_config {
    int port;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(liujdg_deamon_config, port)

int main(int argc, const char** args) {
    using namespace std::chrono;
    json configFile;
    if (!LcheckFileExists(args[1]))
        throw std::runtime_error(fmt::format("File {} not exists.", args[1]));
    std::ifstream in(args[1]);
    if (!in)
        throw std::runtime_error(fmt::format("File {} unaccessible.", args[1]));
    
    liujdg_deamon_config config;
    try {
        configFile = json::parse(LreadFile(in));
        configFile.get_to(config);
    } catch (std::exception& err) {
        std::cerr << "Error: " << err.what() << std::endl;
        return -1;
    }
    
    try {
        using traits_t = restinio::traits_t<
                restinio::asio_timer_manager_t,
                #ifdef NDEBUG
                restinio::null_logger_t,
                #else
                restinio::single_threaded_ostream_logger_t,
                #endif
                router_t>;
        restinio::run(
            restinio::on_this_thread<traits_t>()
            .port(config.port)
            .address("localhost")
            .request_handler(server_handler())
            // .read_next_http_message_timelimit(10s)
            // .write_http_response_timelimit(1s)
            // .handle_request_timeout(1s)
        );
    } catch (std::exception& err) {
        std::cerr << "Error: " << err.what() << std::endl;
        return 1;
    }
    return 0;
}