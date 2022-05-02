#include "../project/include/Table.h"
#include "../project/include/auxiliary_func.h"

void Table::read_file(string path_file) {
    ifstream fin;
    fin.open(path_file);
    string buf;
    while (!fin.eof()) {
        getline(fin, buf);
        if (buf != "") {
            split_str(buf, &table);  //функция разбиения считанной строки на элементы
            rows++;
        }
    }
    cols = table.size() / rows;
    fin.close();
}

void Table::out_str() {
    int max_elem_size;
    for (size_t i = 0; i < rows; i++) {
        for (size_t j = 0; j < cols; j++) {
            max_elem_size = max_elem_size_in_col(table, rows, cols, j);
            cout << setw(max_elem_size)<< left << table.at(j + i * cols) << "   ";
        }
        cout << endl;
    }
}

string Table::get_elem(size_t row, size_t col) {
    if ((row >= rows) || (col >= cols)) {
        return "Ошибка. Неверно введен номер ячейки";
    }

    if (table.at(col + row * cols) == "") {
        return "Ячейка пуста";
    } else {
        return table.at(col + row * cols);
    }
}

void Table::set_elem(size_t row, size_t col, string& value) {
    table.at(col + row * cols) = value;
}

void Table::set_table() {
    string buf;
    for (size_t i = 0; i < rows; i++) {
        cout << "Введите " << i  << " строку. Значение ячеек разделяйте ;" << endl;
        set_row_elems(i);
    }
}

void Table::set_rows_size(size_t rows) {
    this->rows = rows;
}

void Table::set_cols_size(size_t cols) {
    this->cols = cols;
}

void Table::set_row_elems(size_t pos) {
    string buf;
    if (pos == 0) {
        cin.ignore();
    }
    getline(cin, buf);
    buf = buf + '\r';
    split_str(buf, &table);
}

void Table::set_col_elems(size_t col, size_t current_col) {
    vector<string> new_table;
    vector<string> buf_table;
    string buf;
    if (current_col == 0) {
        cin.ignore();
    }
    getline(cin, buf);
    buf = buf + '\r';
    split_str(buf, &buf_table);

    for (size_t i = 0; i < rows; i++) {
        for(size_t j = 0; j < cols - col + current_col; j++) {
            new_table.push_back(table.at(j + i * (cols - col + current_col)));
        }
        new_table.push_back(buf_table.at(i));
    }

    table = new_table;
}

size_t Table::get_rows() {
    return rows;
}

size_t Table::get_cols() {
    return cols;
}

void Table::write_file(string filename) {
    ofstream fout;
    fout.open(filename);
    for (size_t i = 0; i < rows; i++) {
        for (size_t j = 0; j < cols; j++) {
            fout << table.at(j + i * rows);

            if (j + 1 != cols) {
                fout << ";";
            }
        }
        fout << endl;
    }
    fout.close();
}

bool Table::table_empty() {
    return table.empty();
}