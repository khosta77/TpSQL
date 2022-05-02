#include "../include/auxiliary_func.h"

// Разбиения считанной строки на элементы и запись в таблицу
void split_str(const string buf, vector<string>* table) {
    int i = 0;
    string res;
    while (buf[i] != '\r') {     //считываем до конца строки
        if (buf[i] != ';'){
            res = res + buf[i];  //элемент считываем до ;
        } else {
            table->push_back(res);
            res = "";
        }
        i++;
    }

    if (buf[buf.size() - 2 != ';']) {
        table->push_back(res);
    }
}

// Поиск максимального размера элементра в столбце
int max_elem_size_in_col(const vector<string> table, const size_t rows,
                         const size_t cols, const int pos) {
    int max_elem_size = 0;
    int buf;
    max_elem_size = (table.at(pos)).size();
    for (size_t i = 1; i < rows; i++) {
        buf = (table.at(pos + i * cols)).size();

        if (buf > max_elem_size) {
            max_elem_size = buf;
        }
    }

    return max_elem_size;
}

// Создание таблицы
Table create_table(size_t row, size_t col) {
    Table csr;
    csr.set_rows_size(row);
    csr.set_cols_size(col);
    csr.set_table();
    return csr;
}

void add_rows(Table* csr, size_t row) {
    csr->set_rows_size(row + csr->get_rows());

    if (row == 1) {
        cout << "Введите строку";
        csr->set_row_elems(0);
    } else {
        for (size_t i = 0; i < row; i++) {
            cout << "Введите " << i + 1 << " строку";
            csr->set_row_elems(i);
        }
    }
}

void add_cols(Table* csr, size_t col) {
    csr->set_cols_size(col + csr->get_cols());

    if (col == 1) {
        cout << "Введите столбец";
        csr->set_col_elems(col, 0);
    } else {
        for (size_t i = 0; i < col; i++) {
            cout << "Введите " << i + 1 << " столбец";
            csr->set_col_elems(col, i);
        }
    }
}

void print_choise_func() {
    cout << "Доступны следующие функции:" << endl
         << "1 Вывод таблицы из файла в консоль" << endl
         << "2 Вывести содержимое ячейки" << endl
         << "3 Cоздать таблицу" << endl
         << "4 Добавить строки" << endl
         << "5 Добавить столбцы" << endl
         << "6 Сохранить таблицу в файл" << endl
         << "7 Просмотр открытых таблиц" << endl
         << "Для завершения работы введите -1" << endl
         << "Что вы хотите сделать?" << endl;
}

