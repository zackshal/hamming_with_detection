#include <vector>
#include <cmath>
#include <iostream>
#include <locale>
#include <bitset>
#include <iomanip>
#ifdef _WIN32
#include <windows.h>
#endif
using namespace std;
constexpr int ERR_LABEL_WIDTH = 18;
bool isPowerofTwo(int n) { return (n > 0) && ((n & (n - 1)) == 0); }
void init_locale() {
#ifdef _WIN32
    SetConsoleOutputCP(1251);
    SetConsoleCP(1251);
#else
    setlocale(LC_ALL, "ru_RU.UTF-8");
    locale::global(locale("ru_RU.UTF-8"));
#endif
}
void printStep(const vector<int>& vec,
    const string& label,
    int indent = 0,
    int labelWidth = ERR_LABEL_WIDTH)
{
    cout << string(indent, ' ')
        << setw(labelWidth) << left << (label + ":")
        << " [";
    for (size_t i = 0; i < vec.size(); ++i) {
        cout << vec[i]
            << (i + 1 < vec.size() ? " " : "");
    }
    cout << "]\n";
}
vector <int> hammingEncode(const vector <int>& data) {
    int dataBits = data.size();
    int m = 1;
    while ((1 << m) < (dataBits + m + 1)) { m++; }
    int totalBits = dataBits + m;

    vector <int> encoded(totalBits, 0);

    int dataIndex = 0;
    for (int i = 1; i <= totalBits; ++i) {
        if (!isPowerofTwo(i)) {
            if (dataIndex < dataBits) {
                encoded[i - 1] = data[dataIndex++];
            }
        }
    }
    for (int p = 0; p < m; ++p) {
        int parityPos = (1 << p) - 1;
        int xorResult = 0;
        for (int i = parityPos; i < totalBits; i += (1 << (p + 1))) {
            for (int j = i; j < min(i + (1 << p), totalBits); ++j) {
                if (j != parityPos) {
                    xorResult ^= encoded[j];
                }
            }
        }
        encoded[parityPos] = xorResult;
    }
    return encoded;
}
vector <int> hammingDecode(vector <int>& encoded) {
    int totalBits = encoded.size();
    int m = 0;
    while ((1 << m) < totalBits) m++;

    printStep(encoded, "Начало декодирования");

    vector<int> syndrome(m, 0);
    for (int p = 0; p < m; ++p) {
        int parityPos = (1 << p) - 1;
        syndrome[p] = 0;

        for (int i = parityPos; i < totalBits; i += (1 << (p + 1))) {
            for (int j = i; j < min(i + (1 << p), totalBits); ++j) {
                syndrome[p] ^= encoded[j];
            }
        }

        cout << "  Проверочный бит p" << (1 << p)
            << ": вычислен = " << syndrome[p] << endl;
    }

    int errorPos = 0;
    for (int p = 0; p < m; ++p) {
        if (syndrome[p]) {
            errorPos += (1 << p);
        }
    }

    if (errorPos) {
        cout << "! Обнаружена ошибка в позиции " << errorPos << endl;
        cout << "  До исправления: ";
        printStep(encoded, "");

        encoded[errorPos - 1] ^= 1;

        cout << "  После исправления: ";
        printStep(encoded, "");
    }
    else {
        cout << "Ошибок не обнаружено" << endl;
    }

    vector<int> decoded;
    for (int i = 1; i <= totalBits; ++i) {
        if ((i & (i - 1)) != 0) { 
            decoded.push_back(encoded[i - 1]);
        }
    }

    cout << "Извлечены информационные биты: ";
    printStep(decoded, "");

    return decoded;
}
vector <int> inputData() {

    vector<int> data;
    cout << "Введите биты через пробел, а затем Enter: ";
    int bit;
    while (cin >> bit) {
        if (bit != 0 && bit != 1) {
            cout << "Ошибка, так как бит либо 1, либо 0!" << endl;
            continue;
        }
        data.push_back(bit);
        if (cin.peek() == '\n') break;
    }
    return data;
}
void printVector(const vector<int>& vec, const string& label)
{
    cout << label << ": ";
    for (int bit : vec) {
        cout << bit << " ";
    }
    cout << endl;
}
int main() {
    init_locale();
    vector<int> data = inputData();
    printVector(data, "Исходные данные");

    vector<int> encoded = hammingEncode(data);
    printVector(encoded, "Закодированные данные");

    char choice;
    cout << "Внести ошибку в массив? (y/n):";
    cin >> choice;
    if (choice == 'y' || choice == 'Y') {
        int errorPos;
        cout << "Введите позицию ошибки (1-" << encoded.size() << "): ";
        cin >> errorPos;
        if (errorPos >= 1 && errorPos <= encoded.size()) {
            encoded[errorPos - 1] ^= 1;
            printVector(encoded, "Данные с ошибкой");
        }
        else {
            cout << "Некорректная позиция!" << endl;
        }
    }

    vector<int> decoded = hammingDecode(encoded);
    printVector(decoded, "Декодированные данные");
    return 0;
}
