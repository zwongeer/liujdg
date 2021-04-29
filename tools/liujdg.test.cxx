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
#include "LProcess_container.hpp"

using namespace std;

int main(int argc, char const *argv[]) {
    try {
        LProcess_container pro("./attack204");
        string str;
        pro.stdout() >> str;
    } catch (exception& err) {
        cerr << "[runtime_error]:" << err.what() << endl;
        LExit(1);
    }
    LExit(0);
}
