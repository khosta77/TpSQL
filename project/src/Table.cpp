#include "../include/Table.h"
#include "../include/auxiliary_func.h"

/*
 *
 * Не знаю почему но таблица не корректно считывалась, в ней были не читаемые символы
 * */
static void split_str_2(string str, vector<string> &array) {
    string buf = "";
    for (size_t i = 0; i <= str.size(); i++) {
        if (str[i] == ';' || str[i] == '\r' || str[i] == '\0' || i == (str.size())) {
            array.push_back(buf);
            buf = "";
        } else {
            buf += str[i];
        }
    }
}


// Construct/destruct
//Table::Table(string path_file);
//Table::~Table();

// Создаем пустую таблицу заданного размера
Table::Table(size_t row, size_t col) {
    rows = row;
    cols = col;
    for (size_t i = 0; i < rows * cols; i++) {
        tab.push_back("");
    }
}

//// Перегруженные операторы

// Опреатор присваивания
Table& Table::operator=(const Table& rhs) {
    rows = rhs.get_rows();
    cols = rhs.get_cols();
    tab = rhs.tab;
    return *this;
}

// Функция записи элементов таблицы из файла в вектор
void Table::read_file(string path_file) {
    ifstream fin;
    fin.open(path_file);
    string buf;
    while (!fin.eof()) {
        getline(fin, buf);
        if (buf != "") {
            split_str_2(buf, tab);  // функция разбиения считанной строки на элементы
            rows++;
        }
    }
    cols = tab.size() / rows;
    fin.close();
}

// Вывод таблицы в консоль
void Table::out_str() {
    int max_elem_size;
    for (size_t i = 0; i < rows; i++) {
        for (size_t j = 0; j < cols; j++) {
            // Ищем максимальный элемент в столбце и выводим в консоль ячейки с шириной,
            // равной ширине максимального жлемента в столце
            max_elem_size = max_elem_size_in_col(tab, rows, cols, j);
            cout << setw(max_elem_size)<< left << tab.at(j + i * cols) << "   ";
        }
        cout << endl;
    }
}

// Вывод содержимого ячейки
string Table::get_elem(size_t row, size_t col) {
    if ((row >= rows) || (col >= cols)) {
        return "Ошибка. Неверно введен номер ячейки";
    }

    if (tab.at(col + row * cols) == "") {
        return "";
    } else {
        return tab.at(col + row * cols);
    }
}

// Редактирование содержимого ячейки
void Table::set_elem(size_t row, size_t col, string value) {
    tab.at(col + row * cols) = value;
}

// Задание содержимого ячеек
void Table::set_table() {
    string buf;
    for (size_t i = 0; i < rows; i++) {
        cout << "Введите " << i  << " строку. Значение ячеек разделяйте ;" << endl;
        fill_row(this, i);
    }
}

// Задать количество строк
void Table::set_rows_size(size_t rows) {
    this->rows = rows;
}

// Задать количество столбцов
void Table::set_cols_size(size_t cols) {
    this->cols = cols;
}

// Ввести элементы строки
void Table::set_empty_row(size_t row) {
    for (size_t i = this->rows; i < (this->rows + row); i++) {
        for (size_t j = 0; j < cols; j++) {
            tab.push_back("");
        }
    }
    this->rows = this->rows + row;
}

// Заполнение элементов добавленного столбца пустыми строками
void Table::set_empty_col(size_t col, size_t new_col) {
    vector<string> new_table;
    // Заполняем построчно
    for (size_t i = 0; i < rows; i++) {
        // Добавляем уже существующие элементы
        for (size_t j = 0; j < col; j++) {
            new_table.push_back(tab.at(j + i * (col)));
        }

        if (i == 0) {
            // Добавляем заголовок
            for (size_t i = 0; i < new_col; i++) {
                string heading_num;
                cout << "Введите название столбца" << endl;
                cin >> heading_num;
                new_table.push_back(heading_num);
            }
        } else {
            // Добавляем пустые элементы
            for (size_t j = 0; j < new_col; j++) {
                new_table.push_back("");
            }
        }
    }

    tab = new_table;
}

// Узнать количество строк
size_t Table::get_rows() const {
    return rows;
}

// Узнать количество столбцов
size_t Table::get_cols() const {
    return cols;
}

// Запись таблицы в файл
void Table::write_file(string filename) {
    ofstream fout;
    fout.open(filename);
    for (size_t i = 0; i < rows; i++) {
        for (size_t j = 0; j < cols; j++) {
            fout << tab.at(j + i * cols);

            if (j + 1 != cols) {
                fout << ";";
            }
        }
        fout << endl;
    }
    fout.close();
}

// Узнаем, пуста ли таблица
bool Table::table_empty() {
    return tab.empty();
}

//dop func
void Table::push_col(string col_name) { // Вынужденный метод на данном этапе.
    tab.push_back(col_name);
    this->cols = tab.size();
}

void Table::set_rows_size() {
    if ((this->rows * this->cols) >= tab.size()) {
        tab.resize((this->rows + 1) * this->cols);
    }
    tab.at(this->rows * this->cols).push_back('\0');
    this->rows++;
//    for (size_t i = this->rows * this->cols - 1; i < rows * this->cols; i++) {
//        table.push_back(" ");
//    }
}

// Узнаем ширину таблицы при выводе
size_t Table::get_width_table() {
    size_t width = 0;
    for (size_t i = 0; i < cols; i++) {
        width = width + max_elem_size_in_col(tab, rows, cols, i) + 3;
    }

    return width;
}

size_t Table::get_max_width_col() {
    int max_elem_width = max_elem_size_in_col(tab, rows, cols, 0);
    size_t max_width_col = 0;
    for (int i = 1; i < cols; i++) {
        if (max_elem_size_in_col(tab, rows, cols, i) > max_elem_width) {
            max_elem_width = max_elem_size_in_col(tab, rows, cols, i);
            max_width_col = i;
        }
    }

    return max_width_col;
}

// Удаление последнего элемента вектора
void Table::del_last() {
    tab.pop_back();
}

