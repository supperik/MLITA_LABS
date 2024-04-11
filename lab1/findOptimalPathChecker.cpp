// Попов Никита Андреевич ПС-23
// Шашки1 - 1.7(5)
// CLION 2023

// На шахматном поле расположены N черных и одна белая шашка. Требуется написать программу, которая по заданному расположению шашек определяет, какое максимальное количество шашек может взять белая шашка за один ход. Черная шашка сразу после взятия снимается с доски.
// Ввод из файла INPUT.TXT. Задается 8 строк, каждая содержит 8 символов. Символом ‘0’ обозначается пустая клетка, символом ‘1’ положение черной шашки и символом ‘2’ - положение белой шашки.
// Вывод в файл OUTPUT.TXT. Вывести единственное число – максимальное количество черных шашек, которых можно взять за один ход.

#include <fstream>
#include <vector>
#include <algorithm>
#include <string>
#include <iostream>
#include <chrono>

using namespace std;

int maxTaken = 0;

void findingTheOptimalPath(int x, int y, int taken, vector<vector<int>> &board, vector<vector<int>> &default_board) {
    maxTaken = max(maxTaken, taken);
    const int dx[4] = {-1, -1, 1, 1};
    const int dy[4] = {-1, 1, -1, 1};
    for (int direction = 0; direction < 4; ++direction) {
        int nx = x + dx[direction];
        int ny = y + dy[direction];
        int jumpX = nx + dx[direction];
        int jumpY = ny + dy[direction];
        if (nx >= 0 && nx < 8 && ny >= 0 && ny < 8 && jumpX >= 0 && jumpX < 8 && jumpY >= 0 && jumpY < 8 &&
            board[nx][ny] == 1 && board[jumpX][jumpY] == 0) {
            board[nx][ny] = 0;
            board[jumpX][jumpY] = 2;
            board[x][y] = 0;

            findingTheOptimalPath(jumpX, jumpY, taken + 1, board, default_board);

            board[jumpX][jumpY] = 0;
            board[x][y] = 2;
            board[nx][ny] = 1;
        }

    }

}

int main(int argc, char* argv[]) {
    auto start = chrono::high_resolution_clock::now();

    if (argc == 1)
    {
        argc = 3;
        std::string str1 = "input9.txt";
        std::string str2 = "output.txt";
        argv[1] = str1.data();
        argv[2] = str2.data();
    }

    ifstream input(argv[1]);
    ofstream output(argv[2]);

    if (!input.is_open()) {
        output << "Error opening file." << endl;
        return 1;
    }

    vector<vector<int>> board(8, vector<int>(8));
    vector<vector<int>> default_board(8, vector<int>(8));

    string line;
    int linesCount = 0;
    int whiteX = -1, whiteY = -1;
    for (int i = 0; i < 8 && getline(input, line); ++i) {
        if (line.length() != 8) {
            output << "ERROR: length of line != 8\n";
            return 0;
        }
        else linesCount += 1;
        for (int j = 0; j < 8; ++j) {
            board[i][j] = line[j] - '0';
            if (board[i][j] == 2) {
                whiteX = i;
                whiteY = j;
            }
        }
    }

    if (linesCount != 8) {
        output << "ERROR: count of lines != 8\n";
        return 0;
    }

    default_board = board;

    findingTheOptimalPath(whiteX, whiteY, 0, board, default_board);

    output << maxTaken << endl;

    auto stop = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(stop - start);

    output << "Time taken by function: " << duration.count() << " milliseconds" << endl;

    input.close();
    output.close();

    return 0;
}
