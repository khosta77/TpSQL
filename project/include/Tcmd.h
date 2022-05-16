#ifndef PROJECT_INCLUDE_TCMD_H_
#define PROJECT_INCLUDE_TCMD_H_

#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <algorithm>
#include <cctype>
#include <iomanip>
#include <fstream>
#include <cstdio>

#include "Table.h"

using namespace std;

#define PATH_DATA_LOG "./project/data.txt"

// DDL - Data Definition Language
#define CREATE "create"
#define ALTER "alter"
#define DROP "drop"
// DML - Data Manipulation Language
#define SELECT "select"
#define INSERT "insert"
#define UPDATE "update"
// DCL - Data Control Language
#define SHOW "show"
// DAL - Data Additionl Language
#define FROM "from"
#define EXIT "exit"
#define TABLE "table"
#define TABLES "tables"
#define INTO "into"
#define VALUES "values"
#define SET "set"
#define WHERE "where"
#define ADD "add"
#define DROP "drop"
#define RENAME "rename"
#define TO "to"

// TODO: полезные ссылки
//   https://habr.com/ru/post/564390/
//   https://www.schoolsw3.com/sql/sql_update.php
// (Стереть в финальной версии)

class Tcmd {
private:
    vector<string> tables;
public:
    // Construct/destruct
    explicit Tcmd();

private:
    void get_tables_from_memory();  // Получение элементов tables из памяти или создание памяти
    void update_tables_from_memory();  // Обновление памяти
    /* Методы ниже представляют собой команды TpSQL */
    // DDL - Data Definition Language
    void create(vector<string> cmd);
    void drop(vector<string> cmd);
    void alter(vector<string> cmd);
    // DML - Data Manipulation Language
    void select(vector<string> cmd);
    void insert(vector<string> cmd);
    void update(vector<string> cmd);
    // DCL - Data Control Language
    void show(vector<string> cmd);
private:
    // Additional functions
    vector<string> get_split_cmd();  // Получение введенной команды для ее последующего анализа
    string del_symbol(string str, string c);  // Удалит в str символ c
    string tolower(string str);  // А -> а
    string touper(string str);  // a -> A
    bool FileIsExist(string filePath);  // Существет файл или нет
    bool content_symbol(string str, char symbol);  // Содержит str символ symbol или нет
    int num_table(string table_name);  // Status - есть таблица в системе или нет (-1)
};

//
// Construct/destruct
//
Tcmd::Tcmd() {
    get_tables_from_memory();
    cout << "===";
    for (vector<string> cmd;;) {
        cmd = get_split_cmd();
        if ((cmd[0] == CREATE || cmd[0] == DROP || cmd[0] == ALTER) && cmd.size() > 2) {
            if (cmd[0] == CREATE) {
                create(cmd);
            } else if (cmd[0] == DROP) {
                drop(cmd);
            } else if (cmd[0] == ALTER && cmd.size() >= 5) {
                alter(cmd);
            }
        } else if ((cmd[0] == SELECT || cmd[0] == INSERT || cmd[0] == UPDATE) && cmd.size() > 2) {
            if (cmd[0] == SELECT) {
                select(cmd);
            } else if (cmd[0] == INSERT) {
                insert(cmd);
            } else if (cmd[0] == UPDATE) {
                update(cmd);
            }
        } else if ((cmd[0] == SHOW)) {
            show(cmd);
        } else if (cmd[0] == EXIT) {
            cout << "Выход\n";
            break;
        } else {
            cout << "Ошибка!\n";
        }
    }
}

void Tcmd::get_tables_from_memory() {
    if (FileIsExist(PATH_DATA_LOG)) {
        ifstream fin;
        fin.open(PATH_DATA_LOG);
        string line;
        while (getline(fin, line)) {
            tables.push_back(line);
        }
        fin.close();
    } else {
        ofstream fout;
        fout.open(PATH_DATA_LOG);
        fout.close();
    }
}

void Tcmd::update_tables_from_memory() {
    ofstream fout;
    fout.open(PATH_DATA_LOG);
    for (size_t i = 0; i < tables.size(); i++) {
        fout << tables[i] << endl;
    }
    fout.close();
}

//
// DDL - Data Definition Language
//
void Tcmd::create(vector<string> cmd) {
    if (tolower(cmd[1]) == TABLE) {
        tables.push_back(cmd[2]);
        update_tables_from_memory();
        Table table;
        // TODO: Переделать это, таблица пустой создаватся не должна
        if (cmd.size() > 3) {
            if (cmd[3] == "(") {
//                Table table;
                table.set_rows_size(1);
                for (size_t i = 3, j = 1; cmd[i] != ")"; i++, j++) {
                    if (cmd[i] != ")" && cmd[i] != "(") {
                        table.push_col(del_symbol(cmd[i], ","));
                    }
                }
                table.write_file(cmd[2]);
            } else {
                table.write_file(cmd[2]);
            }
        } else {
            table.write_file(cmd[2]);
        }
    } else {
        cout << "Ошибка!\n";
    }
}

void Tcmd::drop(vector<string> cmd) {
    if (tolower(cmd[1]) == TABLE) {
        if (remove(cmd[2].c_str()) != 0) {
            std::cout << "Ошибка!\n" << "--> Ошибка удаления файла" << endl;
        } else {
            for (size_t i = 0; i < tables.size(); i++) {
                if (tables[i] == cmd[2]) {
                    tables.erase(tables.begin() + i);
                    vector<string>(tables).swap(tables);
                }
            }
            update_tables_from_memory();
            std::cout << "Файл успешно удалён" << endl;
        }
    } else {
        cout << "Ошибка!\n";
    }
}

void Tcmd::alter(vector<string> cmd) {
    if (tolower(cmd[1]) == TABLE && cmd.size() == 5) {
        switch (num_table(cmd[2])) {
            case -1: {
                cout << "Ошибка!\n" << "--> Таблица не найдена!" << endl;
                break;
            }
            default: {
                if (tolower(cmd[3]) == ADD) {  // Добавление столбца в конец
                    Table tbl;
                    tbl.read_file(cmd[2]);
                    Table new_tbl;
                    for (size_t i = 0; i < tbl.get_cols(); i++) {
                        new_tbl.push_col(tbl.get_elem(0, i));
                    }
                    new_tbl.push_col(cmd[4]);
                    if (tbl.get_rows() > new_tbl.get_rows()) {
                        for (size_t i = 0; i < tbl.get_rows(); i++) {
                            new_tbl.set_empty_row(1);
                        }
                    }
                    for (size_t i = 1; i < tbl.get_rows(); i++) {
                        for (size_t j = 0; j < tbl.get_cols(); j++) {
                            new_tbl.set_elem(i, j, tbl.get_elem(i, j));
                        }
                    }
                    new_tbl.write_file(cmd[2]);
                } else if (tolower(cmd[3]) == DROP) {
                    Table tbl;
                    tbl.read_file(cmd[2]);
                    Table new_tbl;
                    size_t control_del = -1;
                    tbl.out_str();
                    for (size_t i = 0; i < tbl.get_cols(); i++) {
                        if (tbl.get_elem(0, i) != cmd[4]) {
                            new_tbl.push_col(tbl.get_elem(0, i));
                        } else {
                            control_del = i;
                        }
                    }
                    if (control_del == -1) {
                        cout << "Ошибка!\n" << "--> В таблица нет такой колонки!" << endl;
                    }
                    if (tbl.get_rows() > new_tbl.get_rows()) {
                        for (size_t i = 0; i < tbl.get_rows(); i++) {
                            new_tbl.set_empty_row(1);
                        }
                    }
                    for (size_t i = 0; i < tbl.get_rows(); i++) {
                        for (size_t j = 0, cof = 0; j < tbl.get_cols(); j++) {
                            if (j != control_del) {
                                new_tbl.set_elem(i, j - cof, tbl.get_elem(i, j));
                            } else {
                                cof = 1;
                            }
                        }
                    }
                    new_tbl.write_file(cmd[2]);
                } else {
                    cout << "Ошибка!\n" << "--> Команда не распознана" << endl;
                }
            }
        }
    } else if (tolower(cmd[1]) == TABLE && cmd.size() == 6) {
        if (tolower(cmd[3]) == RENAME && tolower(cmd[4]) == TO) {
            switch (num_table(cmd[2])) {
                case -1: {
                    cout << "Ошибка!\n" << "--> Таблица не найдена!" << endl;
                    break;
                }
                default: {
                    Table tbl;
                    tbl.read_file(cmd[2]);
                    if (remove(cmd[2].c_str()) != 0) {
                        std::cout << "Ошибка!\n" << "--> Переименновывания файла" << endl;
                    } else {
                        for (size_t i = 0; i < tables.size(); i++) {
                            if (tables[i] == cmd[2]) {
                                tables.erase(tables.begin() + i);
                                vector<string>(tables).swap(tables);
                            }
                        }
                        update_tables_from_memory();
                        tables.push_back(cmd[5]);
                        update_tables_from_memory();
                        tbl.write_file(cmd[5]);
                    }
                }
            }
        }
    } else {
        cout << "Ошибка!\n";
    }
}

//
// DML - Data Manipulation Language
//
void Tcmd::select(vector<string> cmd) {
    if (cmd[1] == "*" && cmd.size() == 4) {
        switch (num_table(cmd[3])) {
            case -1: {
                cout << "Таблица не найдена!" << endl;
                break;
            }
            default: {
                Table tbl;
                tbl.read_file(cmd[3]);
                tbl.out_str();  // out_table бывш
            }
        }
    } else if (cmd[1] != "*") {
        vector<string> col_name;
        size_t num_path = -1;
        for (size_t i = 1; i < cmd.size(); i++) {
            if(tolower(cmd[i]) != FROM) {
                col_name.push_back(del_symbol(cmd[i], ","));
            } else {
                num_path = i + 1;
                break;
            }
        }
        switch (num_table(cmd[num_path])) {
            case -1: {
                cout << "Таблица не найдена!" << endl;
                break;
            }
            default: {
                Table tbl, tbl_new;
                for (size_t i = 0; i < col_name.size(); i++) {
                    tbl_new.push_col(col_name[i]);
                }
                tbl.read_file(cmd[num_path]);
                if (tbl.get_rows() > tbl_new.get_rows()) {
                    for (size_t i = 0; i < tbl.get_rows(); i++) {
                        tbl_new.set_empty_row(1);
                    }
                }
                for (size_t j = 0; j < tbl.get_cols(); j++) {
                    for (size_t j_new = 0; j_new < tbl_new.get_cols(); j_new++) {
                        if (tbl.get_elem(0, j) == tbl_new.get_elem(0, j_new)) {
                            for (size_t i = 0; i < tbl.get_rows() && i < tbl_new.get_rows(); i++) {
                                tbl_new.set_elem(i, j_new, tbl.get_elem(i, j));
                            }
                        }
                    }
                }
                tbl_new.out_str();  // out_table бывш
            }
        }
    } else {
        cout << "Ошибка!\n";
    }
}

void Tcmd::insert(vector<string> cmd) {
    if (tolower(cmd[1]) == INTO) {
        vector<string> col;
        size_t mrk_end_col;
        for (size_t i = 3; i < cmd.size(); i++) {
            if (cmd[i] != "(" && cmd[i] != ")") {
                col.push_back(del_symbol(cmd[i], ","));
            } else if (cmd[i] == ")") {
                mrk_end_col = i;
                break;
            }
        }
        if (tolower(cmd[mrk_end_col + 1]) == VALUES) {
            vector<string> val;
            for (size_t i = mrk_end_col + 2; i < cmd.size(); i++) {
                if (cmd[i] != "(" && cmd[i] != ")") {
                    val.push_back(del_symbol(cmd[i], ","));
                }
            }
            if (val.size() == col.size()) {
                switch (num_table(cmd[2])) {
                    case -1: {
                        cout << "Ошибка!\n" << "--> Таблица не найдена!\n";
                        break;
                    }
                    default: {
                        Table tbl;
                        tbl.read_file(cmd[2]);
                        tbl.set_empty_row(1);
                        for (size_t i = 0; i < col.size(); i++) {
                            for (size_t j = 0; j < tbl.get_cols(); j++) {
                                if (tbl.get_elem(0, j) == col[i]) {
                                    tbl.set_elem(tbl.get_rows() - 1, j, val[i]);
                                }
                            }
                        }
                        tbl.write_file(cmd[2]);
                    }
                }
            } else {
                cout << "Ошибка!\n" << "--> Не совпадает val.size != col.size\n";
            }
        } else {
            cout << "Ошибка!\n" << "--> Отсутствуют значения которые необходимо вписать\n";
        }
    } else {
        cout << "Ошибка!\n";
    }
}

void Tcmd::update(vector<string> cmd) {
    switch (num_table(cmd[1])) {
        case -1: {
            cout << "Ошибка!\n" << "--> Таблица не найдена!\n";
            break;
        }
        default: {
            // Получение колонок в которые вставляем обновленные значени
            vector<string> col_nval;
            vector<string> ID_val;
            for (size_t i = 1; i < cmd.size(); i++) {
                if (tolower(cmd[i]) == SET) {
                    bool mrk_next_set = false;
                    for (size_t j = i; j < cmd.size(); j++) {
                        if (content_symbol(cmd[j], '=')) {
                            col_nval.push_back(del_symbol(cmd[j], "="));
                            mrk_next_set = true;
                        } else if (mrk_next_set) {
                            col_nval.push_back(del_symbol(cmd[j], ","));
                            mrk_next_set = false;
                        }

                        if (tolower(cmd[j]) == WHERE) {
                            bool mrk_next_where = false;
                            for (size_t k = j; k < cmd.size(); k++) {
                                if (content_symbol(cmd[k], '=')) {
                                    ID_val.push_back(del_symbol(cmd[k], "="));
                                    mrk_next_where = true;
                                } else if (mrk_next_where) {
                                    ID_val.push_back(cmd[k]);
                                    mrk_next_where = false;
                                }
                            }
                            break;
                        }
                    }
                }
            }
            Table tbl;
            tbl.read_file(cmd[1]);
            if ((ID_val.size() % 2 == 0) && (col_nval.size() % 2 == 0)) {
                for (size_t id_c = 0, id_r = 1; id_c < ID_val.size() && id_r < ID_val.size();
                                                                                      id_c += 2, id_r += 2) {
                    for (size_t r_i = 0; r_i < tbl.get_cols(); r_i++) {
                        if (tbl.get_elem(0, r_i) == ID_val[id_c]) {
                            for (size_t r_j = 0; r_j < tbl.get_rows(); r_j++) {
                                if (tbl.get_elem(r_j, r_i) == ID_val[id_r]) { // корректно работает
                                    for (size_t col_c = 0, col_v = 1;
                                                col_c < col_nval.size() && col_v < col_nval.size();
                                                                           col_c += 2, col_v += 2) {
                                        for (size_t c_i = 0; c_i < tbl.get_cols(); c_i++) {
                                            if (tbl.get_elem(0, c_i) == col_nval[col_c]) {
                                                tbl.set_elem(r_j, c_i, col_nval[col_v]);
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
                tbl.write_file(cmd[1]);
            } else {
                cout << "Ошибка!\n" << "--> col или row без значения\n";
            }
        }
    }
}

//
// DCL - Data Control Language
//
void Tcmd::show(vector<string> cmd) {
    if (tolower(cmd[1]) == TABLES) {
        for (size_t i = 0; i < tables.size(); i++) {
            cout << tables[i] << endl;
        }
    } else {
        cout << "Таблицы отсутствуют" << endl;
    };
}

//
// Additional functions
//
vector<string> Tcmd::get_split_cmd() {
    vector<string> mu;
    string tst;
    for (char a = cin.get(); a != ';'; a = cin.get()) {
        if (a == '\n') {
            cout << "===";
        } else {
            if (a == ' ') {
                mu.push_back(tst);
                tst = "";
            } else {
                tst += a;
            }
        }
    }
    mu.push_back(tst);
    mu[0] = tolower(mu[0]);
    return mu;
}

string Tcmd::del_symbol(string str, string c) {
    string new_str;
    for (size_t i = 0; i < str.size(); i++) {
        if (str[i] != c[0]) {
            new_str += str[i];
        }
    }
    return new_str;
}

string Tcmd::tolower(string str) {
    for (size_t i = 0; i < str.size(); i++) {
        if (str[i] <= 90 && str[i] >= 65) {
            str[i] += 32;
        }
    }
    return str;
}

string Tcmd::touper(string str) {
    for (size_t i = 0; i < str.size(); i++) {
        if (str[i] <= 122 && str[i] >= 97) {
            str[i] -= 32;
        }
    }
    return str;
}

bool Tcmd::FileIsExist(string filePath) {
    bool isExist = false;
    ifstream fin(filePath.c_str());
    if (fin.is_open()) {
        isExist = true;
    }
    fin.close();
    return isExist;
}

bool Tcmd::content_symbol(string str, char symbol) {
    bool ret = false;
    for (size_t i = 0; i < str.size(); i++) {
        if (str[i] == symbol) {
            ret = true;
        }
    }
    return ret;
}

int Tcmd::num_table(string table_name) {
    for (size_t i  = 0; i < tables.size(); i++) {
        if (tables[i] == table_name) {
            return static_cast<int>(i);
        }
    }
    return -1;
}

#endif  // PROJECT_INCLUDE_TCMD_H_
