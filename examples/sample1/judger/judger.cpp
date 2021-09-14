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
    int currentPlayer = 1;
    
    cin >> playerNum >> path;
    // open saved data
    ofstream out((path + "fk.txt").c_str());

    out << playerNum << endl;
    out << path << endl;
    
    cout << "#send 1" << endl;
    cout << "1" << endl;
    cout << "#continue 1" << endl;

    int p;
    string ret;
    
    cin >> p >> ret;

    cout << "#send 2" << endl;
    cout << "2" << endl;
    cout << "#continue 2" << endl;
    
    cout << "#log" << endl;
    cout << "test log" << endl;
    
    cin >> p >> ret;
    
    cout << "#win 1" << endl;
    cout << "1 0" << endl; // scores
    cout << "MLG pro" << endl; // comments (optional)

    return 0;
}
