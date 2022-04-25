#ifndef TPSQL_CSV_READER_H
#define TPSQL_CSV_READER_H

#include <iostream>
#include <string>
#include <vector>

using namespace std;

class csv_reader {
private:
    vector<string> table;
    size_t rows = 0;
    size_t cols = 0;

public:
    size_t get_rows();  // Узнать количество строк
    size_t get_cols();  // Узнать количство столбцов
    void set_rows_size(size_t rows);  // Изменение количества строк
    void set_cols_size(size_t cols);  // Изменение количества столбцов
    void set_table();  // Заполнение элементов таблицы пользователем
    void set_row_elems(size_t pos);  // Заполнение элементов добавленной строки
    void set_col_elems(size_t col, size_t current_col);  // Заполнение элементов добавленного столбца
    void read_file(string path_file);       // Записи элементов таблицы из файла в вектор
    void out_str();                         // Вывод таблицы в консоль
    string get_elem(size_t row, size_t col);  // Вывод содержимого ячейки
    void set_elem(size_t row, size_t col, string& value);  // Редактирование содержимого ячейки
    void write_file(string filename);
    bool table_empty();
};

#endif //TPSQL_CSV_READER_H
