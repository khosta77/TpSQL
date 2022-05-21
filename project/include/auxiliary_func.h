#ifndef TPSQL_AUXILIARY_FUNC_H
#define TPSQL_AUXILIARY_FUNC_H

#include "Table.h"

#include <fstream>

using namespace std;

// Разбиения считанной строки на элементы и запись в таблицу
void split_str(const string buf, vector<string>* table);

// Поиск максимального размера элементра в столбце
int max_elem_size_in_col(const vector<string> table, const size_t rows,
                         const size_t cols, const int pos);

// Добавление сторок в таблицу
void add_rows(Table* table, size_t row, size_t add_after);

// Добавление столбцов в таблицу
void add_cols(Table* table, size_t col, size_t add_after);

// Заполнить столбец
void fill_col(Table* table, size_t col);

// Заполнить строку пустыми элементами
void fill_row(Table* table, size_t row);

#endif  // TPSQL_AUXILIARY_FUNC_H
