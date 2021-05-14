#include <bits/stdc++.h>
using namespace std;
int a[19][19];
int me;
pair<int, int> getPosition() {
  int x = rand() % 19;
  int y = rand() % 19;
  while (a[x][y]) {
    x = rand() % 19;
    y = rand() % 19;
  }
  return make_pair(x, y);
}
int main() {
  srand(time(0));
  int playerNum;
  std::string path;
  cin >> playerNum >> me >> path;
  // initial game state is nothing
  while (true) {
    int player;
    cin >> player;
    if (player != me) {
      int x, y;
      cin >> x >> y;
      a[x][y] = 1;
    } else {
      pair<int, int> pr = getPosition();
      a[pr.first][pr.second] = 1;
      cout << pr.first << " " << pr.second << endl;
    }
  }
}