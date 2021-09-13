#include <filesystem>
namespace fs = std::filesystem;

#include <boost/chrono.hpp>
#include <boost/thread.hpp>

#include "Lapi.hpp"
#include "api_types.hpp"

static liujdg_deamon_config deamon_config;

// check table item
// note the parament(type_name) may cause sql injection, others may not
template <typename T>
inline bool ensure_table_item(soci::session& s, const std::string& table_name,
const std::string& item_name, const std::string& type_name, const T& value) {
    soci::indicator ind;
    T ret;
    s << fmt::format("select {} from pragma_table_info(:table_name) where name = :item_name;", type_name),
        soci::into(ret, ind), soci::use(table_name), soci::use(item_name);
    if (ind == soci::i_ok)
        return ret == value;
    return false;
}

static void init_executor();

void init_api(const liujdg_deamon_config& config) {
    deamon_config = config;
    sql_session.open(backEnd, fmt::format("db=\"{}\" timeout=1", (std::filesystem::path(config.path) / "data.db").string()) );
    sql_session << 
R"(create table if not exists `tasks`(
    id varchar(512) primary key,
    status varchar(32),
    logpath varchar(512)
);)";
    // check table
    soci::indicator ind;
    std::string table_name = "tasks";
    std::string item_name = "id";
    std::string type_name;
    int ret;
    std::string str;
    
    // check primary key
    if (!ensure_table_item(sql_session, table_name = "tasks", item_name = "id", type_name = "pk", ret = 1))
        throw std::runtime_error(LINFO + "`id` is not primary key");
    // check items
    if (!ensure_table_item(sql_session, table_name = "tasks", item_name = "id", type_name = "type", str = "varchar(512)"))
        throw std::runtime_error(LINFO + "please check `id` in table");
    if (!ensure_table_item(sql_session, table_name = "tasks", item_name = "status", type_name = "type", str = "varchar(32)"))
        throw std::runtime_error(LINFO + "please check `status` in table");
    if (!ensure_table_item(sql_session, table_name = "tasks", item_name = "logpath", type_name = "type", str = "varchar(512)"))
        throw std::runtime_error(LINFO + "please check `logpath` in table");
    init_executor();
}

void exe_thread_f() {
    enum struct Task_type{NONE, BUILD, RUN};
    Task_type type = Task_type::NONE;
    LProcess_container pro;
    for (;;) {
        boost::this_thread::sleep_for(boost::chrono::seconds(2));
        {
            lockguard lock(mutex_pro);
            if (processes.empty()) continue;
            if (type == Task_type::NONE) {
                try {
                    soci::session sql(backEnd, fmt::format("db=\"{}\" timeout=1", (std::filesystem::path(deamon_config.path) / "data.db").string()) );
                    sql << "update tasks set status = :status where id = :id;", 
                            soci::use(api_status_BUILDING), soci::use(processes.front().second);
                } catch (std::exception& err) {
                    std::cerr << LINFO << "Database error:" << err.what() << std::endl;
                    std::cerr << "id:" << processes.front().second << std::endl;
                }
                pro = std::move(LProcess_container("liujdg.build -"));
                pro.getStdin() << processes.front().first.toString() << std::endl;
                pro.closeInPipe();
                type = Task_type::BUILD;
                continue;
            }

            // the process should not be nullptr
            assert(pro.process);

            if (pro.isRunning()) {
                continue;
            }

            auto ret = pro.getReturnValue();
            if (!ret.has_value()) {
                try {
                    soci::session sql(backEnd, fmt::format("db=\"{}\" timeout=1", (std::filesystem::path(deamon_config.path) / "data.db").string()) );
                    sql << "update tasks set status = :status where id = :id;", 
                            soci::use(api_status_UNKNOWN), soci::use(processes.front().second);
                } catch (std::exception& err) {
                    std::cerr << LINFO << "Database error:" << err.what() << std::endl;
                    std::cerr << "id:" << processes.front().second << std::endl;
                }
                std::ofstream out(fs::path(deamon_config.path) / "liujdg_error.txt", std::ios::ate);
                out << "liujdg crashed!" << std::endl;
                out << "[stdout]:" << LreadFile(pro.getStdout()) << std::endl;
                out << "[stderr]:" << LreadFile(pro.getStderr()) << std::endl;
                out << "id:" << processes.front().second << std::endl;
                std::cerr << "liujdg crashed!" << std::endl;
                std::cerr << "[stdout]:" << LreadFile(pro.getStdout()) << std::endl;
                std::cerr << "[stderr]:" << LreadFile(pro.getStderr()) << std::endl;
                std::cerr << "id:" << processes.front().second << std::endl;
                processes.pop();
                continue;
            }
            int return_code = ret.value();

            if (return_code == 0) {
                if (type == Task_type::BUILD) {
                    try {
                        soci::session sql(backEnd, fmt::format("db=\"{}\" timeout=1", (std::filesystem::path(deamon_config.path) / "data.db").string()) );
                        sql << "update tasks set status = :status where id = :id;", soci::use(api_status_RUNNING), soci::use(processes.front().second);
                    } catch (std::exception& err) {
                        std::cerr << LINFO << "Database error:" << err.what() << std::endl;
                        std::cerr << "id:" << processes.front().second << std::endl;
                    }
                    pro = std::move(LProcess_container("liujdg.run -"));
                    // pass the json file
                    pro.getStdin() << processes.front().first.toString() << std::endl;
                    // close stdin, so liujdg will finish reading the json file
                    pro.closeInPipe();
                    type = Task_type::RUN;
                } else if (type == Task_type::RUN) {
                    // this means the game is finished
                    try {
                        soci::session sql(backEnd, fmt::format("db=\"{}\" timeout=1", (std::filesystem::path(deamon_config.path) / "data.db").string()) );
                        sql << "update tasks set status = :status where id = :id;", soci::use(api_status_FULFILLED), soci::use(processes.front().second);
                    } catch (std::exception& err) {
                        std::cerr << LINFO << "Database error:" << err.what() << std::endl;
                        std::cerr << "id:" << processes.front().second << std::endl;
                    }
                    type = Task_type::NONE;
                    pro = std::move(LProcess_container());
                    processes.pop();
                }
            }
            else {
                if (type == Task_type::BUILD) {
                    try {
                        soci::session sql(backEnd, fmt::format("db=\"{}\" timeout=1", (std::filesystem::path(deamon_config.path) / "data.db").string()) );
                        std::string status = api_status_BUILDERROR;
                        std::string id = processes.front().second;
                        sql << "update tasks set status = :status where id = :id;", soci::use(status), soci::use(id);
                    } catch (std::exception& err) {
                        std::cerr << LINFO << "Database error:" << err.what() << std::endl;
                        std::cerr << "id:" << processes.front().second << std::endl;
                    }
                    std::error_code ec;
                    fs::create_directories(fs::path(deamon_config.path) / processes.front().second, ec);
                    if (ec)
                        std::cerr << LINFO << "Failed to create directory" << std::endl;
                    std::ofstream out(fs::path(deamon_config.path) / processes.front().second / "build_error.txt", std::ios::out);
                    if (!out) {
                        std::cerr << LINFO << "Failed to open build_error.txt" << std::endl;
                        std::cerr << "id:" << processes.front().second << std::endl;
                    } else {
                        out << "id:" << processes.front().second << std::endl;
                        out << "[build stdout]:";
                        out << LreadFile(pro.getStdout()) << std::endl;
                        out << "[build stderr]:";
                        out << LreadFile(pro.getStderr()) << std::endl;
                    }
                    type = Task_type::NONE;
                    pro = std::move(LProcess_container());
                    processes.pop();
                } else if (type == Task_type::RUN) {
                    try {
                        soci::session sql(backEnd, fmt::format("db=\"{}\" timeout=1", (std::filesystem::path(deamon_config.path) / "data.db").string()) );
                        sql << "update tasks set status = :status where id = :id;", soci::use(api_status_RUNTIME_ERROR), soci::use(processes.front().second);
                    } catch (std::exception& err) {
                        std::cerr << LINFO << "Database error:" << err.what() << std::endl;
                        std::cerr << "id:" << processes.front().second << std::endl;
                    }
                    std::error_code ec;
                    fs::create_directories(fs::path(deamon_config.path) / processes.front().second, ec);
                    if (ec)
                        std::cerr << LINFO << "Failed to create directory" << std::endl;
                    std::ofstream out(fs::path(deamon_config.path) / processes.front().second / "runtime_error.txt", std::ios::out);
                    if (!out) {
                        std::cerr << LINFO << "Failed to open runtime_error.txt" << std::endl;
                        std::cerr << "id:" << processes.front().second << std::endl;
                    } else {
                        out << "id:" << processes.front().second << std::endl;
                        out << "[build stdout]:";
                        out << LreadFile(pro.getStdout()) << std::endl;
                        out << "[build stderr]:";
                        out << LreadFile(pro.getStderr()) << std::endl;
                    }
                    type = Task_type::NONE;
                    pro = std::move(LProcess_container());
                    processes.pop();
                }
            }
        }
    }
}
static boost::thread exe_thread;

void init_executor() {
    exe_thread = std::move(boost::thread(exe_thread_f));
}

void clean_api() {
    exe_thread.interrupt();
}