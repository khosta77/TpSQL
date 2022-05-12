#ifndef TPSQL_TABLE_H_
#define TPSQL_TABLE_H_

#include <iostream>
#include <string>
#include <vector>
#include <iomanip>

using namespace std;

class Table {
private:
    vector<string> table;
    size_t rows = 0;
    size_t cols = 0;

public:
    // Construct/destruct
//    Table(string path_file);
//    ~Table();

    // read/write table
    void read_file(string path_file);
    void write_file(string filename);
    void out_table();  // Вывод таблицы в консоль

    // info
    size_t get_rows();  // Узнать количество строк
    size_t get_cols();  // Узнать количство столбцов
    bool table_empty();
    string get_elem(size_t row, size_t col);  // Вывод содержимого ячейки

    // redact
    void set_rows_size(size_t rows);  // Изменение количества строк
    void set_cols_size(size_t cols);  // Изменение количества столбцов
    void set_table();  // Заполнение элементов таблицы пользователем
    void set_row_elems(size_t pos);  // Заполнение элементов добавленной строки
    void set_col_elems(size_t col, size_t current_col);  // Заполнение элементов добавленного столбца
    void set_elem(size_t row, size_t col, string value);  // Редактирование содержимого ячейки



    // !!! dop func
    void push_col(string col_name);
    void set_rows_size();  // Изменение количества строк

private:
    //dop func
};

#endif  // TPSQL_TABLE_H_
