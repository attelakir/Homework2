//var 23
#include <iostream>
#include <string>
#include <cstdlib>
#include <sstream>
#include <fstream>
#include <vector>
#include <iterator>

using namespace std;

int main(int argc, const char *argv[]) {
    if (argc != 3) {
        cerr << "Error: Use three parameters\n";
        return 5;
    }
    const string mode(argv[1]);           // режим работы
    const string file_name(argv[2]);      // имя файла
    string text, file2;                   // то, что мы ввели в консоль; имя файла, но с ".txt"
    int password, random, i = 0;          // пароль; гамма; счетчик
    unsigned short a, b;                  // здесь будем сохранять байты одного блока
    stringstream file;                    // промежуточный буффер для имени файла
    unsigned char a_char, b_char;         // те же байты, но типа char
    string result;                        // зашифрованный код
    file << file_name << ".txt";
    file >> file2;

    if (mode == "encryption") {
// Режим шифрования
        cout << "Type your text\n";
        getline(cin, text);
        cout << "Type your password\n";
        cin >> password;
        srand(password);
        for (; i < text.size() - 1; i += 2) {
            a = static_cast<short>(text[i]);
            b = static_cast<short>(text[i + 1]);
            a <<= 8;
            a |= b;                              // "склеим" два байта
            random = rand();
            random &= 0xFFFF;                    // размер random() два байта, но наложим маску на всякий случай
            a ^= random;                         // наложим гамму
            b = a;
            a <<= 12;
            b >>= 4;                             // циклический сдвиг на 4 бита вправо
            a |= b;
            b = a;
            a >>= 8;
            a_char = a;
            b_char = b;                          // "расклеим" байты и присвоим им тип char
            result.push_back(a_char);            // сохраним результат
            result.push_back(b_char);
        }
        if (text.size() % 2 != 0 && i == text.size() - 1) {   // частный случай для блока размером 1 байт
            a = static_cast<short>(text[i]);
            random = rand();
            random &= 0xFF;
            a ^= random;
            b = a;
            a >>= 4;
            b <<= 4;
            a |= b;
            a_char = a;
            result.push_back(a_char);;
        }
        ofstream file_encryption(file2);     // создадим файл и сохраним в нем зашифрованный код
        file_encryption << result;
        file_encryption.close();
        cout << "Encryption comlpeted!! Check " << file_name << ".txt." << endl;
    } else if (mode == "decryption") {
// Режим расшифрования
        ifstream file_decryption(file2, ios::binary);                                   // считаваем зашифрованный код
        std::vector<char> text2((istreambuf_iterator<char>(file_decryption)),           // и сохраним его в text2
                                istreambuf_iterator<char>());
        file_decryption.close();
        cout << "Enter password\n";
        cin >> password;
        srand(password);
        for (int k = 0; k < text2.size(); k++) {                         // эта штука нужна на случай, если
            if ((unsigned short) text2[k] == 13 &&                       // зашифрованный код кодержит символ
                (unsigned short) text2[k + 1] == 10) {                   // переноса строки
                text2[k] = text2[k + 1];
                text2.erase(text2.begin() + k + 1);
            }
        }
        for (; i < text2.size() - 1; i += 2) {              //проделываем то же самое, но в обратном порядке
            a = static_cast<unsigned short>(text2[i]);
            b = static_cast<unsigned short>(text2[i + 1]);
            a &= 0xFF;
            a <<= 8;
            b &= 0xFF;
            a |= b;
            b = a;
            a >>= 12;
            b <<= 4;
            a |= b;
            random = rand();
            random &= 0xFFFF;
            a ^= random;
            b = a;
            a >>= 8;
            a_char = a;
            b_char = b;
            result.push_back(a_char);
            result.push_back(b_char);
        }
        if (text2.size() % 2 != 0 && i == text2.size() - 1) {
            a = static_cast<short>(text2[i]);
            a &= 0xFF;
            b = a;
            a <<= 4;
            b >>= 4;
            a |= b;
            random = rand();
            random &= 0xFF;
            a ^= random;
            a_char = a;
            result.push_back(a_char);
        }
        cout << result << endl;   // выводим расшифрованный код в консоль
    }
    system("pause");
    return 0;
}