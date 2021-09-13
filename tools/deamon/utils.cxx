#include "Lapi.hpp"
#include "api_types.hpp"

#include <curl/curl.h>

void getStatus_and_path_fromSql(const std::string& id, std::string& status, std::string& path) {
    soci::indicator ind;
    sql_session << "select status from tasks where id = :id;", soci::into(status, ind), soci::use(id);
    if (ind != soci::i_ok) throw std::runtime_error("unable to find `status` by `id`");

    sql_session << "select logpath from tasks where id = :id;", soci::into(path, ind), soci::use(id);
    if (ind != soci::i_ok) throw std::runtime_error("unable to find `logpath` by `id`");
}

std::string url_encode(const char* msg) {
    CURL* c = curl_easy_init();
    char* encoded = curl_easy_escape(c, msg, 0);
    std::string retval = encoded;
    curl_free(encoded);
    curl_easy_cleanup(c);
    return retval;
}
