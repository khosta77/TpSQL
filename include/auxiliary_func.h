#ifndef TPSQL_AUXILIARY_FUNC_H
#define TPSQL_AUXILIARY_FUNC_H

#include "csv_reader.h"

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>

using namespace std;

void split_str(const string buf, vector<string>* table);
int max_elem_size_in_col(const vector<string> table, const size_t rows,
                         const size_t cols, const int pos);
csv_reader create_table(size_t row, size_t col);
void add_rows(csv_reader* csr, size_t row);
void split_col(vector<string>* bif_table, const string buf);

#endif //TPSQL_AUXILIARY_FUNC_H
