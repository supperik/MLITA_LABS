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
    int x = -1, y = -1, pathLength = 0;
    bool isWave = false, isEdge = false;
    char ch;
};

void printVector(std::vector<std::vector<char>>& vector, int combLevel)
{
    for (int i = 0; i < 2 * combLevel - 1; i++)
    {
        for (int j = 0; j < vector[i].size(); j++)
        {
            std::cout << vector[i][j];
        }
        std::cout << '\n';
    }
}

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

    int dxUnder[6] = {0, -1, -1, 1, 1, 0};
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
    std::vector<std::vector<Comb>> combField(2 * combLevel - 1, std::vector<Comb>(2 * combLevel - 1));

    std::vector<std::vector<char>> tempVector(2 * combLevel - 1, std::vector<char>(2 * combLevel - 1));

    for (int i = 0; i < 2 * combLevel - 1; i++)
    {
        for (int j = 0; j < field[i].size(); j++)
        {
            tempVector[i][j] = field[i][j];
        }
    }

    combQueue.push(startComb);

    while (!combQueue.empty())
    {
        Comb prevComb;
        prevComb = combQueue.front();

        combQueue.pop();

        for (int dir = 0; dir < 6; dir++)
        {
            int nx, ny;

            if (prevComb.y < combLevel - 1)
            {
                nx = prevComb.x + dxTop[dir];
                ny = prevComb.y + dyTop[dir];
            }
            if (prevComb.y > combLevel - 1)
            {
                nx = prevComb.x + dxUnder[dir];
                ny = prevComb.y + dyUnder[dir];
            }
            if (prevComb.y == combLevel - 1)
            {
                nx = prevComb.x + dxCenter[dir];
                ny = prevComb.y + dyCenter[dir];
            }

            if (nx >= 0 && ny >= 0 && nx < field[ny].size() && ny < field.size())
            {
                Comb selectedComb{};

                if (combField[ny][nx].y == -1 && combField[ny][nx].x == -1)
                {
                    selectedComb.x = nx;
                    selectedComb.y = ny;
                    combField[ny][nx] = selectedComb;
                } else
                {
                    selectedComb = combField[ny][nx];
                }


                if (!selectedComb.isWave &&
                    field[ny][nx] == 'D')
                {
                    selectedComb.ch = field[ny][nx];
                    selectedComb.isWave = true;

                    selectedComb.pathLength = prevComb.pathLength + 1;
                    combQueue.push(selectedComb);
                    combField[ny][nx] = selectedComb;

                    tempVector[selectedComb.y][selectedComb.x] = char(selectedComb.pathLength + 48);

                    if (ny == 0 || ny == (2 * combLevel) - 2 || nx == 0 || (ny <= combLevel && nx >= (combLevel + ny) ||
                                                                            (ny > combLevel &&
                                                                             nx >= (combLevel - (ny - combLevel)))))
                    {
                        minPath = std::min(minPath, selectedComb.pathLength);
                    }
                }
            }
            //printVector(tempVector, combLevel);
        }
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
    combLevel = std::stoi(line);
    int lineNum = 0;
    std::vector<std::vector<char>> field(2 * combLevel - 1, std::vector<char>(2 * combLevel - 1));

    while (getline(inputFile, line))
    {
        for (int j = 0; j < line.size(); j++)
        {
            field[lineNum][j] = line[j];
        }
        lineNum++;
    }
    return field;
}

int main(int argc, char *argv[])
{
    auto start = std::chrono::high_resolution_clock::now();

    if (argc == 1)
    {
        argc = 3;
        std::string str1 = "inputFile2.txt";
        std::string str2 = "outputFile.txt";
        argv[1] = str1.data();
        argv[2] = str2.data();
    }

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
