//#include "include/csv_reader.h"
//#include "include/auxiliary_func.h"
#include "src/csv_reader.cpp"
#include "src/auxiliary_func.cpp"

#include <iostream>
#include <string>
#include <vector>

using namespace std;

int main() {
    print_choise_func();

    short int user_choise;
    csv_reader csr;
    string path_file;

//    vector<csv_reader> all_tables;  Сюда будем записывать все открытые пользователем таблицы
    cin >> user_choise;
    while (user_choise != -1){
        switch (user_choise) {
            case 1: {
                cout<< "Укажите путь к файлу" <<endl;
                cin >> path_file;
                cin.ignore();

                if (csr.table_empty()) {
                    csr.read_file(path_file);

                    cout << "Таблица из файла " << path_file << endl;
                    csr.out_str();
                } else {
                    cout << "Ошибка";
                }

                break;
            }

            case 2: {
                cout << "Введите номер ячейки по типу <номер строки> <номер столбца>";
                size_t row;
                size_t col;
                cin >> row >> col;
                cin.ignore();

                string res = csr.get_elem(row, col);
                cout << "Содержимое выбранной ячейки: " << endl;
                cout << res <<endl;
                break;
            }

            case 3: {
                cout << "Введите размер таблицы по типу <количество строк> <количество столбцов>";
                size_t row;
                size_t col;
                cin >> row >> col;
                cin.clear();

                csv_reader table = create_table(row, col);
                cout << "Введенная таблица: " << endl;
                table.out_str();
                csr = table;
                break;
            }

            case 4: {
                cout << "Сколько новых строк вы хотите добавить?" << endl;
                size_t row;
                cin >> row;

                add_rows(&csr, row);
                cout << "Теперь таблица выглядит так: " <<endl ;
                csr.out_str();
                break;
            }

            case 5: {
                cout << "Сколько новых столбцов вы хотите добавить?" << endl;
                size_t col;
                cin >> col;

                add_cols(&csr, col);
                cout << "Теперь таблица выглядит так: " <<endl ;
                csr.out_str();
                break;
            }

            case 6: {
                cout << "Введите название файла" << endl;
                string filename;
                cin.ignore();
                getline(cin, filename);
                csr.write_file(filename);
                cout << "Содержимое файла " << filename << endl;
                csr.out_str();
            }

            case -1: {
                cout << "Работа завершена" << endl;
                return 0;
            }

            default: {
                cout << "Ошибка. Неверный выбор" << endl;
                break;
            }
        }

        print_choise_func();
        cin >> user_choise;
    }



//    csv_reader table = create_table(2,2);
//    table.read_file("tst.csv");
//    table.get_elem(3, 2);
//    table.out_str();
    return 0;
}