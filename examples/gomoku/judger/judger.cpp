#include <bits/stdc++.h>
int board[19][19];
std::vector<std::pair<int, int>> p[2];
// initialize the game
void init() {
  memset(board, -1, sizeof board);
  p[0].clear();
  p[1].clear();
}
bool validate(int x, int y) {
  if (x < 0 || x >= 19)
    return false;
  if (y < 0 || y >= 19)
    return false;
  return board[x][y] == -1;
}
// 1: player 1 win | 2: player 2 win | -1: nobody win
int checkWin() {
  for (int i = 0; i < 15; ++i) {
    for (int j = 0; j < 15; ++j) {
      int one, two;
      // [-]
      one = two = 0;
      for (int k = 0; k < 5; ++k) {
        one += board[i][j + k] == 1;
        two += board[i][j + k] == 2;
      }
      if (one == 5)
        return 1;
      if (two == 5)
        return 2;
      // [|]
      one = two = 0;
      for (int k = 0; k < 5; ++k) {
        one += board[i + k][j] == 1;
        two += board[i + k][j] == 2;
      }
      if (one == 5)
        return 1;
      if (two == 5)
        return 2;
      // [\]
      one = two = 0;
      for (int k = 0; k < 5; ++k) {
        one += board[i + k][j + k] == 1;
        two += board[i + k][j + k] == 2;
      }
      if (one == 5)
        return 1;
      if (two == 5)
        return 2;
      // [/]
      one = two = 0;
      for (int k = 0; k < 5; ++k) {
        one += board[i + 4 - k][j + k] == 1;
        two += board[i + 4 - k][j + k] == 2;
      }
      if (one == 5)
        return 1;
      if (two == 5)
        return 2;
    }
  }
  return -1;
}

inline int switchPlayer(int p) {
  return p == 1 ? 2 : 1;
}
inline void winAndScore(int thewinner) {
  std::cout << "#win " << thewinner << std::endl;
  // score the players
  for (int i = 1; i <= 2; ++i) {
    if (i == thewinner)
      std::cout << "1 ";
    else
      std::cout << "0 ";
  }
  std::cout << std::endl;
  // give comments(optional)
  std::cout << "Good!" << std::endl;
}
int main(int argc, const char *argv[]) {
  init();
  int n = 0;
  std::string path;
  std::cin >> n;
  std::cin >> path;

  // assert(n == 2);
  std::cout << std::endl;
  int nowplayer = 1;
  std::cout << "#send " << nowplayer << std::endl;
  std::cout << nowplayer << std::endl;
  std::cout << "#continue " << nowplayer << std::endl;
  while (true) {
    // read player operation
    int playerId;
    int x, y;
    std::cin >> playerId >> x >> y;
    // validate the operation
    if (!validate(x, y)) {
      std::cout << "#log" << std::endl;
      std::cout << nowplayer << " " << x << " " << y << std::endl;
      winAndScore(switchPlayer(nowplayer));
      return 0;
    }
    // take the operation
    board[x][y] = nowplayer;

    p[nowplayer - 1].push_back(std::make_pair(x, y));
    // check if somebody wins
    int winner = checkWin();
    if (winner > -1) {
      std::cout << "#log" << std::endl;
      std::cout << nowplayer << " " << x << " " << y << std::endl;
      winAndScore(winner);
      return 0;
    }
    // check if ther e is no place to put chess anymore
    if (p[0].size() + p[1].size() == 19 * 19) {
      std::cout << "#draw" << std::endl;
      std::cout << "0 0" << std::endl;
      return 0;
    }
    // log the step
    std::cout << "#log" << std::endl;
    std::cout << nowplayer << " " << x << " " << y << std::endl;

    // send message to another player
    std::cout << "#send " << switchPlayer(nowplayer) << std::endl;
    std::cout << nowplayer << " " << x << " " << y << std::endl;

    // continue playing
    nowplayer = switchPlayer(nowplayer);
    std::cout << "#send " << nowplayer << std::endl;
    std::cout << nowplayer << std::endl;
    std::cout << "#continue " << nowplayer << std::endl;
  }
}
