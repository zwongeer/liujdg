#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <ctime>
#include <string>
#include <fstream>

using namespace std;

class TicTacToe {
public:
	static constexpr int BOARD_SIZE = 3;
	TicTacToe();
	void print();
	char getCurrentPlayer();
	char getWinner();
	bool isDone();
	bool isValidMove(int row, int col);
	void makeMove(int row, int col);
	void makeAutoMove();
private:
	char board[BOARD_SIZE][BOARD_SIZE];
	char currentPlayer;
};

TicTacToe::TicTacToe() {
    for (int i=0;i<BOARD_SIZE;++i)
        for (int j=0;j<BOARD_SIZE;++j)
            board[i][j] = '_';
    currentPlayer = 'X';
}

void TicTacToe::print() {
    std::cout << " ";
    for (int i = 1; i <= BOARD_SIZE; ++i)
        std::cout << std::setw(BOARD_SIZE) << i;
    std::cout << std::endl;
    for (int i=1;i<=BOARD_SIZE;++i) {
        std::cout << i;
        for (int j=0;j<BOARD_SIZE;++j)
            std::cout << std::setw(BOARD_SIZE) << board[i-1][j];
        std::cout << std::endl;
    }
}

char TicTacToe::getCurrentPlayer() {
    return currentPlayer;
}

char TicTacToe::getWinner() {
    char ch = board[1][1];
    if (ch != '_') {
        if (board[0][0] == ch && board[2][2] == ch)
            return ch;
        else if (board[0][2] == ch && board[2][0] == ch)
            return ch;
    }
    for (int i = 0; i < BOARD_SIZE; ++i) {
        ch = board[i][0];
        bool flag = true;
        for (int j = 1; j < BOARD_SIZE; ++j) {
            if (ch != board[i][j]) {
                flag = false;
                break;
            }
        }
        if (flag) return ch;
    }
    for (int j = 0; j < BOARD_SIZE; ++j) {
        ch = board[0][j];
        bool flag = true;
        for (int i = 1;i < BOARD_SIZE; ++i) {
            if (ch != board[i][j]) {
                flag = false;
                break;
            }
        }
        if (flag) return ch;
    }
    return '_';
}

bool TicTacToe::isDone() {
    if (getWinner() != '_') return true;
    for (int i = 0; i < BOARD_SIZE; ++i)
        for (int j = 0; j < BOARD_SIZE; ++j)
            if (board[i][j] == '_')
                return false;
    return true;
}

bool TicTacToe::isValidMove(int row, int col) {
    if (board[row][col] != '_')
        return false;
    return true;
}

void TicTacToe::makeMove(int row, int col) {
    if (!isValidMove(row,col)) return;
    board[row][col] = currentPlayer;
    currentPlayer = (currentPlayer == 'X')? 'O' : 'X';
}

void TicTacToe::makeAutoMove() {
    if (isDone()) return;
    int x,y;
    if (board[1][1] == '_'){
        makeMove(1, 1);
        return;
    }
    char player = getCurrentPlayer();

    // Try to win
    int count;
    for (int i = 0; i < BOARD_SIZE; ++i)
    for (int j = 0; j < BOARD_SIZE; ++j)
        if (board[i][j] == player) {
            count = 0;
            for (int w = 0; w < BOARD_SIZE; ++w)
                if (board[w][j] == player)
                    ++count;
            if (count == 2)
            for (int w = 0; w < BOARD_SIZE; ++w)
                if (board[w][j] == '_') return makeMove(w,j);
            count = 0;
            for (int h = 0; h < BOARD_SIZE; ++h)
                if (board[i][h] == player)
                    ++count;
            if (count == 2)
            for (int h = 0; h < BOARD_SIZE; ++h)
                if (board[i][h] == '_') return makeMove(i,h);
            count = 0;
            if (i == j) {
                for (int a = 0; a < BOARD_SIZE; ++a)
                    if (board[a][a] == player)
                        ++count;
                if (count == 2)
                for (int a = 0; a < BOARD_SIZE; ++a)
                    if (board[a][a] == '_') return makeMove(a,a);
            }
            count = 0;
            if (i + j == 2){
                for (int a = 0; a < BOARD_SIZE; ++a)
                    if (board[a][2-a] == player)
                        ++count;
                if (count == 2)
                for (int a = 0; a < BOARD_SIZE; ++a)
                    if (board[a][2-a] == '_')
                        return makeMove(a,2-a);
            }
        }
    // Prevent the opponent from winning
    player = (player == 'X')? 'O' : 'X';
    for (int i = 0; i < BOARD_SIZE; ++i)
    for (int j = 0; j < BOARD_SIZE; ++j)
        if (board[i][j] == player) {
            count = 0;
            for (int w = 0; w < BOARD_SIZE; ++w)
                if (board[w][j] == player)
                    ++count;
            if (count == 2)
            for (int w = 0; w < BOARD_SIZE; ++w)
                if (board[w][j] == '_') return makeMove(w,j);
            count = 0;
            for (int h = 0; h < BOARD_SIZE; ++h)
                if (board[i][h] == player)
                    ++count;
            if (count == 2)
            for (int h = 0; h < BOARD_SIZE; ++h)
                if (board[i][h] == '_') return makeMove(i,h);
            count = 0;
            if (i == j) {
                for (int a = 0; a < BOARD_SIZE; ++a)
                    if (board[a][a] == player)
                        ++count;
                if (count == 2)
                for (int a = 0; a < BOARD_SIZE; ++a)
                    if (board[a][a] == '_') return makeMove(a,a);
            }
            count = 0;
            if (i + j == 2){
                for (int a = 0; a < BOARD_SIZE; ++a)
                    if (board[a][2-a] == player)
                        ++count;
                if (count == 2)
                for (int a = 0; a < BOARD_SIZE; ++a)
                    if (board[a][2-a] == '_') return makeMove(a,2-a);
            }
        }
    if (board[1][1] == player) {
        if (board[0][0] == '_') return makeMove(0,0);
        if (board[0][2] == '_') return makeMove(0,2);
        if (board[2][0] == '_') return makeMove(2,0);
        if (board[2][2] == '_') return makeMove(2,2);
    }

    // Random movement
    do {
        x = rand() % BOARD_SIZE;
        y = rand() % BOARD_SIZE;
    } while (!isValidMove(x, y));
    makeMove(x,y);
}


static void twoRobots() {
    TicTacToe tictactoe;
    do {
        tictactoe.print();
        // Random movement
        int x,y;
        do {
            x = rand() % TicTacToe::BOARD_SIZE;
            y = rand() % TicTacToe::BOARD_SIZE;
        } while (!tictactoe.isValidMove(x, y));
        tictactoe.makeMove(x,y);
    } while (!tictactoe.isDone());
    tictactoe.print();
    char winner = tictactoe.getWinner();
    if (winner != '_')
        cout << "The winner is " << winner << std::endl;
    else
        cout << "Draw!" << endl;
}



inline int getCurrentPlayer(char ch) {
	return ch == 'X' ? 1 : 2;
}
inline int switchPlayer(int p) {
	return p == 1 ? 2 : 1;
}

// 'O' : 'X';
int main() {
    srand(time(nullptr));
    TicTacToe tictactoe;
    std::string path;
    int playerNum;
    int currentPlayer = 1;

    cin >> playerNum >> path;
    // open saved data
    ofstream out((path + "fk.txt").c_str());
    
    out << "giao" << endl;
    out << playerNum << endl << path << endl;
    cout << "#continue 1" << endl;

    int id, x, y;
    while (cin.peek() != '#' && cin >> id >> x >> y) {
    	out << "giao" << endl;
    	if (tictactoe.isValidMove(x, y)) {
    		tictactoe.makeMove(x, y);
    		// log the step
    		cout << "#log" << endl;
    		cout << currentPlayer << " " << x << " " << y << endl;
    		currentPlayer = switchPlayer(currentPlayer);
    		cout << "#send " << currentPlayer << endl;
    		cout << x << " " << y << endl;
            if (tictactoe.isDone()) {
                if (tictactoe.getWinner() != '_') {
                    cout << "#win " << getCurrentPlayer(tictactoe.getWinner()) << endl;
                    // give scores
                    for (int i = 1; i <= 2; ++i) {
                        if (i == getCurrentPlayer(tictactoe.getWinner()))
                            cout << "1 ";
                        else cout << "0 ";
                    }
                    cout << endl;

                    // give comments(optional)
                    cout << "Good!" << endl;
                    break;
                } else {
                    cout << "#draw" << endl;

                    // give scores
                    for (int i = 1; i <= 2; ++i)
                        cout << "0 ";
                    cout << endl;

                    // comments(optional)
                    cout << "MLG pro" << endl;
                    out << "MLG pro" << endl;

                    break;
                }
            }
    		cout << "#continue " << currentPlayer << endl;
    	} else {
    		currentPlayer = switchPlayer(currentPlayer);
    		cout << "#win " << currentPlayer << endl;
    		
            // give scores
            for (int i = 1; i <= 2; ++i) {
                if (i == currentPlayer)
                    cout << "1 ";
                else cout << "0 ";
            }

            // comments(optional)
    		cout << "\nWrong operation by one player" << endl;
            break;
    	}
    }
    // do some clean
    out << "end" << endl;
    out.close();
    return 0;
}
