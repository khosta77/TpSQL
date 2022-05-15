#include "../include/auxiliary_func.h"


// Разбиения считанной строки на элементы и запись в таблицу
void split_str(const string buf, vector<string>* table) {
    int i = 0;
    string res;
    while (buf[i] != '\r') {     // считываем до конца строки
        if (buf[i] != ';'){
            res = res + buf[i];  // элемент считываем до ;
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

// Создать таблицу заданного размера
Table create_table(size_t row, size_t col) {
    Table table;
    table.set_rows_size(row);
    table.set_cols_size(col);
    table.set_table();
    return table;
}

/* Добавление строк в таблицу
 * Получаем на вход таблицу, которую будем менять, количество добавляемых строк,
 * номер строки, после которого нужно вставить строки
 */
void add_rows(Table* table, size_t row, size_t add_after) {
    if (add_after == table->get_cols() - 1) {
        // Добавление строки в конец
        table->set_rows_size(table->get_rows() + row);
        table->set_empty_row(add_after + 1);
    } else {
        // Добавление строки в середину
        Table new_table(table->get_rows() + row, table->get_cols());

        // Переносим все стоки до места вставки столбца в новую таблицу
        for (size_t i = 0; i <= add_after; i++) {
            for (size_t j = 0; j < new_table.get_cols(); j++) {
                new_table.set_elem(i, j, table->get_elem(i, j));
            }
        }

        // Переносим все строки после места вставки строки в новую таблицу
        for (size_t i = add_after + row + 1; i < new_table.get_rows(); i++) {
            for (size_t j = 0; j < new_table.get_cols(); j++) {
                new_table.set_elem(i, j, table->get_elem(i - row, j));
            }
        }
        *table = new_table;
    }

}

/* Добавление столбцов в таблицу
 * Получаем на вход таблицу, которую будем менять, количество добавляемых столбцов,
 * номер столбца, после которого нужно вставить столбцы
 */
void add_cols(Table* table, size_t col, size_t add_after) {
    if (add_after == table->get_cols() - 1) {
        // Добавление столбца в конец
        table->set_cols_size(table->get_cols() + col);
        table->set_empty_col(add_after + 1, col);
    } else {
        // Добавление столбца в середину
        Table new_table(table->get_rows(), table->get_cols() + col);

        // Переносим все столбцы до места вставки столбца в новую таблицу
        for (size_t i = 0; i < new_table.get_rows(); i++) {
            for (size_t j = 0; j <= add_after; j++) {
                new_table.set_elem(i, j, table->get_elem(i, j));
            }
        }

        // Добавляем заголовок
        for (size_t i = add_after + 1; i < add_after + col+ 1; i++) {
            string heading_num;
            cout << "Введите название столбца" << endl;
            cin >> heading_num;
            new_table.set_elem(0, i, heading_num);
        }

        // Переносим все столбцы после места вставки столбца в новую таблицу
        for (size_t i = 0; i < new_table.get_rows(); i++) {
            for (size_t j = add_after + col + 1; j < new_table.get_cols(); j++) {
                new_table.set_elem(i, j, table->get_elem(i, j - col));
            }
        }
        *table = new_table;
    }
}

// Заполнить столбец
void fill_col(Table* table, size_t col) {
    vector<string> buf_table;
    string buf;
    cout << "Введите столбец" << endl;
    cin.ignore();
    getline(cin, buf);
    buf = buf + '\r';
    split_str(buf, &buf_table);

    for (size_t i = 0; i < table->get_rows(); i++) {
        table->set_elem(i, col, buf_table.at(i));
    }
}

// Заполнить строку пустыми элементами
void fill_row(Table* table, size_t row) {
    vector<string> buf_table;
    string buf;
    cout << "Введите строку" << endl;
    cin.ignore();
    getline(cin, buf);
    buf = buf + '\r';
    split_str(buf, &buf_table);

    for (size_t i = 0; i < table->get_cols(); i++) {
        table->set_elem(row, i, buf_table.at(i));
    }
}

// Печатаем список доступных для выбора функций
void print_choise_func() {
    cout << "Доступны следующие функции:" << endl
         << "1 Вывод таблицы из файла в консоль" << endl
         << "2 Вывести содержимое ячейки" << endl
         << "3 Cоздать таблицу" << endl
         << "4 Добавить строки" << endl
         << "5 Добавить столбцы" << endl
         << "6 Сохранить таблицу в файл" << endl
         << "7 Заполнить пустой столбец" << endl
         << "8 Заполнить пустую строку" << endl
         << "9 Вывод столбцов" << endl
         << "Для завершения работы введите -1" << endl
         << "Что вы хотите сделать?" << endl;
}


/*
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
*/ // Из ветки stepan-2
