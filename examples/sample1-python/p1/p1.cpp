#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <ctime>
#include <string>
#include <fstream>

using namespace std;

int main() {
    srand(time(nullptr));
    std::string path;
    int playerNum;
    int myid;

    cin >> playerNum >> myid >> path;
    int g;
    cin >> g;
    cout << "ducker:" << __FILE__ << ":" << g << endl;
    return 0;
}
