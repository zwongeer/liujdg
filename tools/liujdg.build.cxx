#include <cstdio>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>

#include <unistd.h>

#include "Lfunc.hpp"
#include "LGame.hpp"
#include "LGameInfo.hpp"
#include "LProcess_container.hpp"


int main(int argc, char const *argv[]) {
    using namespace std;
    namespace fs = std::filesystem;
    if (argc != 2) {
        std::cerr << "Syntax Error!" << std::endl;
        return -1;
    }
    bool useStdin = strcmp("-", argv[1]) == 0;
    if (!useStdin && !LcheckFileExists(argv[1])) {
        std::cerr << "File unaccessiable" << std::endl;
        return -1;
    }
    ifstream fin;
    if (!useStdin)
        fin.open(argv[1]);
    istream& in = useStdin ? cin : fin;
    if (!in) {
        cerr << "Cannot open file" << endl;
        return -1;
    }
    std::string data = useStdin ? LreadFile(in) : LreadFile(in, (size_t)fs::file_size(argv[1]));
    LGameInfo info;
    
    try {
        info.fromString(data); // no check command

        // set current path to the config.basedir in json
        fs::current_path(info.config.basedir);
    } catch (std::exception& err) {
        cerr << LINFO + "Error when init what()=" << err.what() << "\n" << endl;
        return -1;
    }

    std::error_code ec;
    std::string path = fs::current_path().string();
    try {
        fs::create_directories(info.config.logdir);
    } catch (std::exception& err) {
        cerr << "Failed to create log dir. what()=" << err.what() << "\n" << endl;
        return -1;
    }

    // the function will return rc
    int rc = 0;
    for (auto & person : info.persons) {
        fs::current_path(path, ec);
        try {
            fs::current_path(person.basedir);

            // the path for player to store files
            fs::create_directories(person.folder);

            // skip build if there is no need
            if (person.build == "." || person.build == "") continue;

            // build
            LProcess_container pro(person.build);
            if (!pro.wait_or_kill(info.config.buildTimeLimit)) throw std::runtime_error("Build time limit exceed");
            if (!pro.getReturnValue().has_value() || pro.getReturnValue().value() != 0)
                throw std::runtime_error( "Build error:\n\033[;33m[stdout]:" + LreadFile( pro.stdout() ) + "\033[0m" + 
                                        "\n\033[;34m[stderr]:" + LreadFile( pro.stderr() ) + "\033[0m" );
        } catch(std::exception& err) {
            // \033[$BACKGROUND$;$FONT$m$str$\033[0m
            //   30:black
            //   31:red
            //   32:green 
            //   33:yellow
            //   34:blue
            //   35:purple  
            //   36:dark green
            //   37:white
            cerr << "\033[;32mError happened\nid = " << person.id << endl;
            cerr << "name = " << person.name << "\033[0m" << endl;
            cerr << "err.what() = " << err.what() << "\n" << endl;
            rc = -1;
        }
    }
    return rc;
}
