// 10.14. Матрица (6)
// В матрице A размером N ×N (3 ≤  N  ≤ 1000) находятся без повторения числа от 1 до N2. Нужно определить, может ли эта матрица получиться из отсортированной матрицы, в которой последовательно по строкам и слева направо в каждой строке записаны в порядке возрастания числа 1, 2, …, N2, путем выполнения следующих двух операций:
// обмен любых двух строк; 
// обмен любых двух столбцов.
// Если это возможно, найти минимальное количество обменов, после которых заданная матрица окажется отсортированной.
// Ввод из файла INPUT.TXT. В первой строке содержатся число N. В следующих N строках находятся элементы матрицы A по N чисел в строке через пробел. 
// Вывод в файл OUTPUT.TXT. В первой строке нужно вывести Yes, если матрицу можно привести к отсортированной, или No, если это невозможно. Если данное преобразование возможно, то во второй строке нужно вывести минимальное число обменов, необходимое для преобразования матрицы.
// Примеры
// Ввод 1    Ввод 2
// 3         3
// 5 6 4     5 6 4
// 8 9 7     8 9 1 
// 2 3 1     2 3 7
// Вывод 1   Вывод 2
// Yes       No
// 4

// Попов Никита ПС-23 
// Visual Studio Code 2023



#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>

void swap_rows(std::vector<std::vector<int>> &matrix, int row1, int row2) {
    std::swap(matrix[row1], matrix[row2]);
}

void swap_columns(std::vector<std::vector<int>> &matrix, int col1, int col2) {
    for (int i = 0; i < matrix.size(); ++i) {
        std::swap(matrix[i][col1], matrix[i][col2]);
    }
}

bool IsCanMatrixSort(const std::vector<std::vector<int>> &matrix) {

    int matrixSize = matrix.size();

    std::vector<int> vectorRow(matrixSize, 0);
    std::vector<int> vectorCol(matrixSize, 0);

    for (int i = 0; i < matrixSize; i++) {
        vectorRow[i] = (matrix[i][0] - 1) / matrixSize;
        vectorCol[i] = (matrix[0][i] - 1) % matrixSize;
    }

    for (int i = 0; i < matrixSize; i++) {
        for (int j = 0; j < matrixSize; j++) {
            if (vectorRow[i] != (matrix[i][j] - 1) / matrixSize) { return false; }

            if (vectorCol[j] != (matrix[i][j] - 1) % matrixSize) { return false; }
        }
    }
    return true;
}

int count_shuffles(std::vector<std::vector<int>> &matrix) {
    int count = 0;
    int matrixSize = matrix.size();
    bool isContinue = true;

    while (isContinue) {
        isContinue = false;
        for (int i = 0; i < matrixSize; i++) {
            if ((matrix[0][i] - 1) % matrixSize != i) {
                swap_columns(matrix, i, (matrix[0][i] - 1) % matrixSize);
                count++;
                if ((matrix[0][i] - 1) % matrixSize != i) { isContinue = true; }
            }

            if ((matrix[i][0] - 1) / matrixSize != i) {
                swap_rows(matrix, i, (matrix[i][0] - 1) / matrixSize);
                count++;
                if ((matrix[i][0] - 1) / matrixSize != i) { isContinue = true; }
            }
        }
    }
    return count;
}


int main(int argc, char* argv[]) {
    auto start = std::chrono::high_resolution_clock::now(); // Начало измерения времени

    if (argc == 1) {
        argc = 3;
        std::string str1 = "INPUT.TXT";
        std::string str2 = "OUTPUT.TXT";
        argv[1] = str1.data();
        argv[2] = str2.data();
    }

    std::ifstream fin(argv[1]);
    std::ofstream fout(argv[2]);

    int n;
    fin >> n;

    std::vector<std::vector<int>> matrix(n, std::vector<int>(n));
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            fin >> matrix[i][j];   
        }
    }
    if (IsCanMatrixSort(matrix)) {
        fout << "Yes\n" << count_shuffles(matrix);
    } else {
        fout << "No";
    }

    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - start); // Вычисляем продолжительность

    fout << '\n' << "Time taken by function: " << duration.count() << " milliseconds" << std::endl;

    fin.close();
    fout.close();

    return 0;
}