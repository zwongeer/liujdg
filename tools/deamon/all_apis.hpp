#include <memory>


#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <fmt/core.h>
#include <nlohmann/json.hpp>
#include <restinio/all.hpp>

#include "Lfunc.hpp"
#include "liujdg_version.hpp"

using json = nlohmann::json;
restinio::http_status_line_t censoredByAttack204 = { restinio::http_status_code_t(451), "Censored By Attack204" };
using router_t = restinio::router::express_router_t<>;

inline std::string get_unique_id() {
    return fmt::format("{}-{}",
        boost::uuids::to_string(boost::uuids::random_generator()()),
        Lget_seconds_since_epoch());
}

template <typename RESPONSE>
RESPONSE init_response(RESPONSE response) {
    response.append_header("Server", "Attack204's Potatoes & Apples");
    response.append_header("Who-is-txdy", "Attack204");
    response.append_header_date_field()
        .append_header( "Content-Type", "text/plain; charset=utf-8" );
    return response;
}

inline void use_all_apis(std::unique_ptr<router_t>& router) {
    
}