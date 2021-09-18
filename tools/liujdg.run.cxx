#include <cstdio>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>

#include "Lfunc.hpp"
#include "LGame.hpp"

int main(int argc, char const *argv[]) {
    namespace fs = std::filesystem;
    if (argc != 2) {
        std::cerr << "Syntax Error!" << std::endl;
        LExit(1);
    }
    std::ofstream errorLogFile(liujdg::NULLFILE);
    
    std::error_code ec;
    bool useStdin = strcmp(argv[1], "-") == 0;
    std::ifstream fin;
    if (!useStdin)
        fin.open(argv[1]);
    std::istream& in = useStdin ? std::cin : fin;

    std::shared_ptr<LGame> pgame;
    // load the config and launch the programs
    try {
        LfuncInit();
        if (!useStdin) {
            auto path = LgetFilePath(argv[1]);
            if (path.has_value()) std::filesystem::current_path(path.value().c_str(), ec);
            if (ec) throw std::runtime_error(LINFO + "Unable to set current dir `" + path.value().c_str() + "`");
        }
        std::string str = LreadFile(in);
        LGameInfo info;
        info.fromString(str);
        fs::current_path(info.config.basedir, ec);
        if (ec) throw std::runtime_error(LINFO + "Unable to set current dir `" + info.config.basedir + "`");
        pgame = std::make_shared<LGame>();
        pgame->fromString(str);
        pgame->init();
        std::ofstream out(fs::path(pgame->gameInfo.config.logdir) / "liujdg_run_log.txt", std::ios::out);
        
        if (!out)
            throw std::runtime_error(LINFO + "Unable to open liujdg.run log file");
        else
            errorLogFile = std::move(out);
        pgame->run();
    } catch (std::exception& err) {
        std::cerr << "[fatal error]:" << err.what() << std::endl;
        errorLogFile << "[fatal error]:" << err.what() << std::endl;
        LExit(1);
    }
    
    // run the game
    try {
        while (!pgame->hasDone()) {
            pgame->nextStep();
        }
        pgame->writeResultToFile();
    } catch (std::exception& err) {
        std::cerr << "[runtime_error]:" << err.what() << std::endl;
        errorLogFile << "[runtime_error]:" << err.what() << std::endl;
        pgame->waitAndKill();
        LExit(1);
    }
    pgame->waitAndKill();
    LExit(0);
}
