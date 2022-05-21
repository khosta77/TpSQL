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
