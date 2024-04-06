#include <fstream>
#include <iostream>
#include <vector>
#include <queue>
#include <climits>
#include <chrono>
#include <conio.h>
#include <algorithm>
#include <vector>

struct Comb
{
    int x, y, pathLength = 0;
    bool isWave = false, isEdge = false;
    char ch;
};

std::vector<Comb> GetStartCombsVector(std::vector<std::vector<Comb>> field, int n)
{
    std::vector<Comb> startCombs;
    for (int i = 0; i < 2 * n - 1; i++)
    {
        for (int j = 0; j <= field[i].size() - 1; j++)
        {
            if (!field[i][j].isEdge)
            {
                continue;
            } else
            {
                startCombs.push_back(field[i][j]);
            }
        }
    }

    return startCombs;
}

int FindOptimalPath(std::vector<std::vector<char>>& field, int combLevel)
{
    int minPath = INT_MAX;

    int dxUnder[6] = {-1, -1, -1, 0, 1, 0};
    int dyUnder[6] = {-1, 0, 1, -1, 0, 1};

    int dxTop[6] = {-1, -1, 0, 0, 1, 1};
    int dyTop[6] = {-1, 0, 1, -1, 0, 1};

    int dxCenter[6] = {-1, -1, -1, 0, 1, 0};
    int dyCenter[6] = {-1, 0, 1, -1, 0, 1};

    Comb startComb;


    for (int i = 0; i < 2 * combLevel - 1; i++)
    {
        for (int j = 0; j < field[i].size(); j++)
        {
            if (field[i][j] == 'M')
            {
                startComb.x = j;
                startComb.y = i;
            }
        }
    }

    std::queue<Comb> combQueue;
    std::vector<std::vector<int>> isWave;

    std::vector<std::vector<char>> tempVector = field;

    combQueue.push(startComb);
    while (!combQueue.empty())
    {
        Comb* prevComb = new Comb;
        *prevComb = combQueue.front();

        combQueue.pop();

        for (int dir = 0; dir < 6; dir++)
        {
            int nx, ny;

            if (prevComb->y < combLevel - 1)
            {
                nx = prevComb->x + dxTop[dir];
                ny = prevComb->y + dyTop[dir];
            }
            if (prevComb->y > combLevel - 1)
            {
                nx = prevComb->x + dxUnder[dir];
                ny = prevComb->y + dyUnder[dir];
            }
            if (prevComb->y == combLevel - 1)
            {
                nx = prevComb->x + dxCenter[dir];
                ny = prevComb->y + dyCenter[dir];
            }

            Comb selectedComb;

            selectedComb.x = nx;
            selectedComb.y = ny;

            if (nx >= 0 && ny >= 0 && nx < field[ny].size() && ny < field.size() && !prevComb->isWave && field[ny][nx] == 'D')
            {
                selectedComb.ch = field[ny][nx];
                selectedComb.isWave = true;

                if (selectedComb.ch == 'D')
                {
                    isWave.push_back({ny, ny});
                    tempVector[ny][nx] = char(prevComb->pathLength + 1 + 48);
                    selectedComb.pathLength = prevComb->pathLength + 1;
                    combQueue.push(selectedComb);
                }

                if (ny == 0 || ny == (2 * combLevel) - 2 || nx == 0 || (ny <= combLevel && nx >= (combLevel + ny) || (ny > combLevel && nx >= (combLevel - (ny - combLevel)))))
                {
                    minPath = std::min(minPath, selectedComb.pathLength);
                }
            }
            for (std::vector<char> askdjfh: tempVector)
            {
                for (char ch: askdjfh)
                {
                    std::cout << ch;
                }
                std::cout << '\n';
            }
        }

        delete prevComb;
    }
    if (minPath == INT_MAX)
    {
        return 0;
    }
    return minPath;
}


bool ValidateInputs(int argc, char *argv[], std::ifstream& inputFile, std::ofstream& outputFile)
{
    if (argc != 3)
    {
        std::cout << "Usage: main.exe <inputFile> <outputFile>" << std::endl;
        return false;
    }

    if (!outputFile.is_open())
    {
        std::cout << "Could not open output file" << std::endl;
        return false;
    }

    if (!inputFile.is_open())
    {
        std::cout << "Could not open input file" << std::endl;
        return false;
    }

    return true;
}

std::vector<std::vector<char>> ReadFile(std::ifstream& inputFile, int& combLevel)
{
    std::string line;
    getline(inputFile, line);
    combLevel = int(line[0]) - 48;

    std::vector<std::vector<char>> field(2 * combLevel - 1);
    while (getline(inputFile, line))
    {
        std::vector<char> tempVector;
        for (char i : line)
        {
            tempVector.push_back(i);
        }
        field.push_back(tempVector);
    }
    return field;
}

int main(int argc, char *argv[])
{
    auto start = std::chrono::high_resolution_clock::now();

    std::ifstream inputFile(argv[1]);
    std::ofstream outputFile(argv[2]);

    int combLevel;

    if (!ValidateInputs(argc, argv, inputFile, outputFile))
    {
        return 1;
    }

    std::vector<std::vector<char>> field = ReadFile(inputFile, combLevel);

    int result = FindOptimalPath(field, combLevel);

    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);

    outputFile << "Time taken by function: " << duration.count() << " milliseconds" << std::endl;
    outputFile << result;

    inputFile.close();
    outputFile.close();

    return 0;
}
