#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>

#include <unistd.h>

#include "Lfunc.hpp"
#include "LGame.hpp"

int main(int argc, char const *argv[]) {
    if (argc != 2 && argc != 3) {
        std::cerr << "Syntax Error!" << std::endl;
        LExit(1);
    }
    bool useErrorLogFile;
    std::ofstream errorLogFile;
    if (argc == 3) {
        errorLogFile.open(argv[2], std::ios::ate);
        if (!errorLogFile) {
            std::cerr << "Unable to open the log file" << std::endl;
            LExit(1);
        }
        useErrorLogFile = true;
    } else {
        useErrorLogFile = false;
        errorLogFile.open("/dev/null");
        std::cerr << "[warning]: Not using the log file" << std::endl;
    }

    std::shared_ptr<LGame> pgame;
    // load the config and launch the programs
    try {
        LfuncInit();
        pgame = std::make_shared<LGame>(argv[1]);
        pgame->init();
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
