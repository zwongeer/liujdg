#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>

#include "Lfunc.hpp"
#include "LGame.hpp"
#include "LProcess_container.hpp"

using namespace std;

int main(int argc, char const *argv[]) {
    try {
        LProcess_container pro(liujdg::RUN);
        pro.getStdin() << "not a json!" << std::endl;
        pro.closeInPipe();
        cout << "F:" << (pro.isRunning()) << endl;
        char ch;
        cout << "[stdout]:" << endl;
        while (ch = pro.getStdout().get(), ch != EOF)
            cout << ch;
        cout << "[stderr]:" << endl;
        while (ch = pro.getStderr().get(), ch != EOF)
            cout << ch;
        if (pro.getReturnValue().has_value())
            cout << "[return code]:" << pro.getReturnValue().value() << endl;
        else
            cout << "[Error happend]: The program may is still running" << endl, pro.kill();
        pro.wait();
    } catch (exception& err) {
        cerr << "[runtime_error]:" << err.what() << endl;
        LExit(1);
    }
    LExit(0);
}
