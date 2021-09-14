#include "Lapi.hpp"

void serve_http(std::unique_ptr<router_t>& router, const liujdg_deamon_config& config) {
    using traits_t = 
        restinio::traits_t<
        restinio::asio_timer_manager_t,
        // #ifdef NDEBUG
        restinio::null_logger_t,
        // #else
        // restinio::single_threaded_ostream_logger_t,
        // #endif
        router_t>;
    restinio::run(
        restinio::on_this_thread<traits_t>()
        .port(config.port)
        .address("localhost")
        .request_handler(std::move(router))
        // .read_next_http_message_timelimit(10s)
        // .write_http_response_timelimit(1s)
        // .handle_request_timeout(1s)
    );
}

void serve_https(std::unique_ptr<router_t>& router, const liujdg_deamon_config& config) {

}