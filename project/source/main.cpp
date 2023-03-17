#include <iostream>
#include <fstream>
#include <string>

#include "HaffCoder.h"

using namespace std;

int main(int argc, char* argv[]){
    ifstream fin; // Поток входных данных
    ofstream fout; // Поток выходных данных
    ofstream finterdata; // Поток промежуточных данных
    ofstream *p_finterdata = nullptr; // Адрес потока промежуточных данных

    bool decoder; // Использовать ли декодировщик?

    if (argc > 1 && argc < 6){
        if (argv[1] == string("encoder")) decoder = false; // Определение режима работы
        else if (argv[1] == string("decoder")) decoder = true;
        else {
            cout << "Нераспознанный режим работы программы: '" << argv[1] << "'\n";
            return 1;
        }

        if (argc > 2){
            fin.open(argv[2]); // Открытие файла для чтения исходных данных
            if (!fin){
                cout << "Ошибка открытия файла: " << argv[2] << endl;
                return 1;
            }

            if (argc > 3) {
                finterdata.open(argv[3]); // Открытие файла для записи промежуточных данных
                if (!finterdata){
                    cout << "Ошибка открытия файла: " << argv[3] << endl;
                    return 1;
                }
                p_finterdata = &finterdata;

                if (argc == 5) {
                    fout.open(argv[4]); // Открытие файла для записи результата
                    if (!fout){
                        cout << "Ошибка открытия файла: " << argv[4] << endl;
                        return 1;
                    }
                }
            }
        }
    } else {
        cout << "Некорректное число аргументов" << endl;
        return 1;
    }

    InterData interdata(p_finterdata); // Создание объекта вывода промежуточных данных

    char *alphabet = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789.,!?:() "; // Алфавит
    
    HaffCoder coder(alphabet, interdata); // Создание объекта-кодировщика
    string inMessage; // Входная строка
    string outMessage; // Выходная результирующая строка
    if (argc > 2) getline(fin, inMessage); // Чтение входных данных из потока
    else {
        if (decoder) cout << "Введите закодированную последовательность бит: ";
        else cout << "Введите сообщение с символами из заданного алфавита: ";
        getline(cin, inMessage); // Чтение входных данных с консоли
    }
    size_t len = inMessage.length();
    interdata << "\nПромежуточные данные:\n";
    try {
        if (decoder) outMessage = coder.decode(inMessage); // Декодирование входной последовательности бит
        else for (int i = 0; i < len; i++) outMessage += coder.encode(inMessage[i]); // Кодирование входного сообщения
    } catch (invalid_argument ex){ cout << ex.what(); return 1;} // Обработка исключения кодировщика

    if (argc == 5) fout << outMessage << "\n"; // Вывод результата в поток
    else {
        if (decoder) cout << "\nРезультат декодирования:\n";
        else cout << "\nРезультат кодирования:\n";
        cout << outMessage << "\n"; // Вывод результата на консоль
    }
    // Закрытие потоков
    fin.close();
    finterdata.close();
    fout.close();
    
    return 0;
}