// 19.2. Пятница 13 (4)
// Перечислить дни, на которые приходится пятница 13-го числа в заданном диапазоне лет из интервала 1901-2099 г.г.
// Ввод из файла INPUT.TXT. В единственной строке задаются через пробел начальный и конечный годы. 
// Вывод в файл OUTPUT.TXT. Выводится столько строк, сколько найдено пятниц 13-го числа. 
// Пример
// Ввод   
// 1952 1954  
// Вывод 
// 1952 JUNE
// 1953 FEBRUARY
// 1953 MARCH
// 1953 NOVEMBER
// 1954 AUGUST

#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

// Определение високосного года
bool isLeapYear(int year) {
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

// Определение дня недели
int dayOfWeek(int year, int month, int day) {
    if (month < 3) {
        month += 12;
        year -= 1;
    }
    int k = year % 100;
    int j = year / 100;
    int h = (day + 13 * (month + 1) / 5 + k + k / 4 + j / 4 + 5 * j) % 7;
    return (h + 5) % 7 + 1;
}

int main() {
    ifstream inputFile("INPUT.TXT");
    ofstream outputFile("OUTPUT.TXT");

    if (!inputFile.is_open()) {
        cerr << "Не удается открыть input файл." << endl;
        return 1;
    }

    if (!outputFile.is_open()) {
        cerr << "Не удается открыть output файл." << endl;
        return 1;
    }

    int startYear, endYear;
    inputFile >> startYear >> endYear;

    vector<string> months = {"JANUARY", "FEBRUARY", "MARCH", "APRIL", "MAY", "JUNE", 
                             "JULY", "AUGUST", "SEPTEMBER", "OCTOBER", "NOVEMBER", "DECEMBER"};

    for (int year = startYear; year <= endYear; ++year) {
        for (int month = 1; month <= 12; ++month) {
            if (dayOfWeek(year, month, 13) == 5) {
                outputFile << year << " " << months[month - 1] << endl;
            }
        }
    }

    inputFile.close();
    outputFile.close();

    return 0;
}
