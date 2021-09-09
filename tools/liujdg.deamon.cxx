#include <memory>

#include <liujdg_version.hpp>

#include <nlohmann/json.hpp>
#include <restinio/all.hpp>

namespace rr = restinio::router;
using router_t = restinio::router::express_router_t<>;
auto router = std::make_unique<router_t>();

template <typename RESPONSE>
RESPONSE init_response(RESPONSE response) {
    response.append_header("Server", "Potatoes & Apples");
    response.append_header("Who-is-txdy", "Attack204");
    response.append_header_date_field()
        .append_header( "Content-Type", "text/plain; charset=utf-8" );
    return response;
}

auto server_handler() {
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

    router->non_matched_request_handler([](auto req) {
        return init_response(req->create_response())
                .set_body("Hello " + liujdg::NAME + "\n" + "You sent:" + req->body())
                .done();
    });
    return router;
}

int main() {
    using namespace std::chrono;
    try {
        using traits_t = restinio::traits_t<
                restinio::asio_timer_manager_t,
                #ifdef NDEBUG
                restinio::null_logger_t,
                #else
                restinio::single_threaded_ostream_logger_t,
                #endif
                router_t >;
        restinio::run(
            restinio::on_this_thread<traits_t>()
            .port(1984)
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