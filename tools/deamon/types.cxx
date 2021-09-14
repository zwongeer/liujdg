#include "Lapi.hpp"
#include "api_types.hpp"

soci::session sql_session;
std::queue<task_> processes;
std::mutex mutex_pro;
soci::backend_factory const &backEnd = *soci::factory_sqlite3();

void to_json(nlohmann::json& j, const liujdg_deamon_config& cfg) {
    j = {
        {"port", cfg.port},
        {"path", cfg.path},
        {"https", cfg.https},
        {"cert", cfg.cert},
        {"key", cfg.key}
    };
}

void from_json(const nlohmann::json& j, liujdg_deamon_config& cfg) {
    cfg.port = j.value("port", liujdg_deamon_config::defaults::PORT);
    j.at("path").get_to<std::string>(cfg.path);
    cfg.https = j.value("https", liujdg_deamon_config::defaults::HTTPS);
    if (cfg.https) {
        j.at("cert").get_to<std::string>(cfg.cert);
        j.at("key").get_to<std::string>(cfg.key);
    }
}