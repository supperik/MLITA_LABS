//Пчелы живут в ульях, где есть соты. Соты представляют собой поле правильных шестиугольников, соприкасающихся друг с другом. Правильное поле строится следующим образом:
//сначала имеется всего одна сота (рис. 1) – это правильное поле первого уровня;
//затем вокруг соты появляются соседние (рис. 2) – это правильное поле второго уровня;
//затем строится еще один «ободок» (рис. 3) – это правильное поле третьего уровня, и т. д.
//Пчела Майя возвращается в улей. Она живет в одной из сот правильного поля уровня N (2 ≤ N ≤ 20). Для того, чтобы добраться до своей соты (если она не расположена с краю поля), Майе нужно переместиться через другие соты, в которых могут жить как друзья (перемещаться можно), так и враги (перемещаться нельзя). Майя очень устала и хочет добраться до своей соты, пройдя через минимальное число других сот. Свой путь она может начинать с любой дружественной соты, находящейся с краю поля (то есть такой соты, которая не окружена со всех сторон соседними сотами).

// Попов Никита ПС-23
// CLION 2023

#include <fstream>
#include <iostream>
#include <vector>
#include <queue>
#include <climits>
#include <chrono>
#include <algorithm>
#include <string>

struct Comb {
    int x = -1, y = -1, pathLength = 0;
    bool isWave = false;
    char ch{};
};

void PrintVector(std::vector<std::vector<std::string>> &vector, int combLevel) {
    for (int i = 0; i < 2 * combLevel - 1; i++) {
        for (int j = 0; j < vector[i].size(); j++) {
            std::cout << vector[i][j];
        }
        std::cout << '\n';
    }
}

Comb GetStartComb(std::vector<std::vector<char>> &field, Comb startComb, int combLevel) {
    std::vector<Comb> startCombs;
    for (int i = 0; i < 2 * combLevel - 1; i++) {
        for (int j = 0; j < field[i].size(); j++) {
            if (field[i][j] == 'M') {
                startComb.x = j;
                startComb.y = i;
            }
        }
    }
    return startComb;
}

int FindOptimalPath(std::vector<std::vector<char>> &field, int combLevel) {
    int minPath = INT_MAX;

    int dxUnder[6] = {0, -1, -1, 1, 1, 0};
    int dyUnder[6] = {-1, 0, 1, -1, 0, 1};

    int dxTop[6] = {-1, -1, 0, 0, 1, 1};
    int dyTop[6] = {-1, 0, 1, -1, 0, 1};

    int dxCenter[6] = {-1, -1, -1, 0, 1, 0};
    int dyCenter[6] = {-1, 0, 1, -1, 0, 1};

    Comb startComb;

    startComb = GetStartComb(field, startComb, combLevel);

    std::vector<std::vector<std::string>> tempVector(2 * combLevel - 1, std::vector<std::string>(2 * combLevel - 1));

    for (int i = 0; i < combLevel * 2 - 1; i++)
    {
        for (int j = 0; j < field[i].size(); j++)
        {
            tempVector[i][j] = field[i][j];
        }
    }

    std::queue<Comb> combQueue;
    std::vector<std::vector<Comb>> combField(2 * combLevel - 1, std::vector<Comb>(2 * combLevel - 1));

    combQueue.push(startComb);

    while (!combQueue.empty()) {
        Comb prevComb;
        prevComb = combQueue.front();

        combQueue.pop();

        for (int dir = 0; dir < 6; dir++) {
            int nx, ny;

            if (prevComb.y < combLevel - 1) {
                nx = prevComb.x + dxTop[dir];
                ny = prevComb.y + dyTop[dir];
            }
            if (prevComb.y > combLevel - 1) {
                nx = prevComb.x + dxUnder[dir];
                ny = prevComb.y + dyUnder[dir];
            }
            if (prevComb.y == combLevel - 1) {
                nx = prevComb.x + dxCenter[dir];
                ny = prevComb.y + dyCenter[dir];
            }

            if (nx >= 0 && ny >= 0 && nx < field[ny].size() && ny < field.size()) {
                Comb selectedComb{};

                if (combField[ny][nx].y == -1 && combField[ny][nx].x == -1) {
                    selectedComb.x = nx;
                    selectedComb.y = ny;
                    combField[ny][nx] = selectedComb;
                } else {
                    selectedComb = combField[ny][nx];
                }

                if (!selectedComb.isWave &&
                    field[ny][nx] == 'D') {
                    selectedComb.ch = field[ny][nx];
                    selectedComb.isWave = true;

                    selectedComb.pathLength = prevComb.pathLength + 1;
                    combQueue.push(selectedComb);
                    combField[ny][nx] = selectedComb;

                    tempVector[ny][ny] = std::to_string(selectedComb.pathLength + 48);

                    if (ny == 0 || ny == (2 * combLevel) - 2 || nx == 0 ||
                        ((ny <= combLevel - 1 && nx == combLevel + ny - 1) ||
                        (ny > combLevel - 1 && nx == 2 * combLevel - 2 - (ny - combLevel)))) {
                        minPath = std::min(minPath, selectedComb.pathLength);
                    }
                }
            }
        }
    }

    if (minPath == INT_MAX) {
        return 0;
    }
    return minPath;
}

bool ValidateInputs(int argc, std::ifstream &inputFile, std::ofstream &outputFile) {
    if (argc != 3 && argc != 1) {
        std::cerr << "Usage: main.exe <inputFile> <outputFile>" << std::endl;
        return false;
    }

    if (!outputFile.is_open()) {
        std::cerr << "Could not open output file" << std::endl;
        return false;
    }

    if (!inputFile.is_open()) {
        std::cerr << "Could not open input file" << std::endl;
        return false;
    }

    return true;
}

std::vector<std::vector<char>> ReadFile(std::ifstream &inputFile, int &combLevel) {
    std::string line;
    getline(inputFile, line);
    combLevel = std::stoi(line);
    int lineNum = 0;
    std::vector<std::vector<char>> field(2 * combLevel - 1, std::vector<char>(2 * combLevel - 1));

    while (getline(inputFile, line)) {
        for (int j = 0; j < line.size(); j++) {
            field[lineNum][j] = line[j];
        }
        lineNum++;
    }
    return field;
}

int main(int argc, char *argv[]) {
    auto start = std::chrono::high_resolution_clock::now();

    if (argc == 1) {
        argc = 3;
        std::string str1 = "inputFile2.txt";
        std::string str2 = "outputFile.txt";
        argv[1] = str1.data();
        argv[2] = str2.data();
    }

    std::ifstream inputFile(argv[1]);
    std::ofstream outputFile(argv[2]);

    int combLevel = 0;

    if (!ValidateInputs(argc, inputFile, outputFile)) {
        return 1;
    }

    std::vector<std::vector<char>> field = ReadFile(inputFile, combLevel);

    int result = FindOptimalPath(field, combLevel);

    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);

    outputFile << "Time taken by function: " << duration.count() << " milliseconds" << std::endl;
    outputFile << result + 1;

    inputFile.close();
    outputFile.close();

    return 0;
}
