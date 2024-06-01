#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

void generate_spiral_matrix(int N, vector<vector<int>>& matrix) {
    int top = 0, bottom = N - 1;
    int left = 0, right = N - 1;
    int num = 1;

    while (num <= N * N) {
        // Fill top row
        for (int i = left; i <= right; ++i) {
            matrix[top][i] = num++;
        }
        top++;

        // Fill right column
        for (int i = top; i <= bottom; ++i) {
            matrix[i][right] = num++;
        }
        right--;

        // Fill bottom row
        for (int i = right; i >= left; --i) {
            matrix[bottom][i] = num++;
        }
        bottom--;

        // Fill left column
        for (int i = bottom; i >= top; --i) {
            matrix[i][left] = num++;
        }
        left++;
    }
}

int main() {
    // Read the input from the file
    ifstream inputFile("INPUT.TXT");
    int N;
    inputFile >> N;
    inputFile.close();

    // Create an NxN matrix
    vector<vector<int>> matrix(N, vector<int>(N, 0));

    // Generate the spiral matrix
    generate_spiral_matrix(N, matrix);

    // Write the output to the file
    ofstream outputFile("OUTPUT.TXT");
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            outputFile << matrix[i][j] << (j == N - 1 ? "" : " ");
        }
        outputFile << endl;
    }
    outputFile.close();

    return 0;
}
