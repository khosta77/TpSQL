#ifndef TPSQL_TABLE_H_
#define TPSQL_TABLE_H_

#include <iostream>
#include <string>
#include <vector>
#include <iomanip>

using namespace std;

class Table {
private:
    vector<string> tab; // table
    size_t rows = 0;
    size_t cols = 0;
    //// Test
public:
    // Construct/destruct
//    Table(string path_file);
//    ~Table();


    //// Конструкторы
    // Создаем пустую таблицу заданного размера
    Table(size_t rows, size_t cols);

    // Создаем переменную класса Table
    Table() = default;

    //// Перегруженные операторы
    Table& operator=(const Table& rhs);

    //// Сеттеры
    // Задать количество строк
    void set_rows_size(size_t rows);

    // Задать количество столбцов
    void set_cols_size(size_t cols);

    // Заполнение элементов таблицы пользователем
    void set_table();

    // Заполнение элементов добавленной строки пустыми строками
    void set_empty_row(size_t row);

    // Заполнение элементов добавленного столбца пустыми строками
    void set_empty_col(size_t col, size_t current_col);

    // Редактирование содержимого ячейки
    void set_elem(size_t row, size_t col, string value);

    //// Геттеры
    // Узнать количество строк
    size_t get_rows() const;

    // Узнать количство столбцов
    size_t get_cols() const;

    // Вывод содержимого ячейки
    string get_elem(size_t row, size_t col);

    //// Остальное
    // Вывод таблицы в консоль
    void out_str();

    // Узнаем, пуста ли таблица
    bool table_empty();

    // Запись таблицы в файл
    void write_file(string filename);

    // Записи элементов таблицы из файла в вектор
    void read_file(string path_file);

/*  // Методы реализованы на ветке stepan-2
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


*/
    // !!! dop func
    void push_col(string col_name);
    void set_rows_size();  // Изменение количества строк

private:
    //dop func
};

#endif  // TPSQL_TABLE_H_
