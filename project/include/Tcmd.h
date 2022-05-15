#ifndef PROJECT_INCLUDE_TCMD_H_
#define PROJECT_INCLUDE_TCMD_H_

#include <iostream>
#include <string>
#include <vector>
//#include <pair>
#include <iomanip>
#include <algorithm>
#include <cctype>
#include <iomanip>
#include <fstream>
#include <cstdio>

#include "Table.h"

using namespace std;

#define PATH_DATA_LOG "./project/data.txt"

#define CREATE "create"
#define ALTER "alter"
#define DROP "drop"

#define SELECT "select"
#define INSERT "insert"
#define UPDATE "update"

#define FROM "from"
#define EXIT "exit"

#define SHOW "show"

#define TABLE "table"
#define TABLES "tables"
#define INTO "into"
#define VALUES "values"
// https://habr.com/ru/post/564390/ - материал с командами
// https://www.schoolsw3.com/sql/sql_update.php
class Tcmd {
private:
    vector<string> tables;
public:
    // Construct/destruct
    explicit Tcmd();

private:
    void get_tables_from_memory();
    void update_tables_from_memory();
    // DDL
    void create(vector<string> cmd); // 7
    void drop(vector<string> cmd); // 7
    void alter(vector<string> cmd);  // Пока что нету
    // DML
    void select(vector<string> cmd); // 7/2
    void insert(vector<string> cmd); // 7
    void update(vector<string> cmd); // 7
    // DC
    void show(vector<string> cmd); // 7
private:
    vector<string> get_split_cmd();
    string del_symbol(string str, string c);
    string tolower(string str);
    string touper(string str);
    bool FileIsExist(string filePath);
    bool content_symbol(string str, char symbol);
    int num_table(string table_name);
};

// Construct/destruct
Tcmd::Tcmd() {
    get_tables_from_memory();
    cout << "===";
    for (vector<string> cmd;;) {
        cmd = get_split_cmd();
//        for (size_t i = 0; i < cmd.size(); i++) {
//            cout << cmd[i] << endl;
//        }
        if ((cmd[0] == CREATE || cmd[0] == DROP || cmd[0] == ALTER) && cmd.size() > 2) {  //
            if (cmd[0] == CREATE) {
                create(cmd);
            } else if (cmd[0] == DROP) {
                drop(cmd);
            } else if (cmd[0] == ALTER) {
                alter(cmd);
            }
        } else if ((cmd[0] == SELECT || cmd[0] == INSERT || cmd[0] == UPDATE) && cmd.size() > 2) {  //
            if (cmd[0] == SELECT) {
                select(cmd);
            } else if (cmd[0] == INSERT) {
                insert(cmd);
            } else if (cmd[0] == UPDATE) {
                update(cmd);
            }
        } else if ((cmd[0] == SHOW)) {
            show(cmd);
        } else if (cmd[0] == EXIT) { //
            cout << "Выход\n";
            break;
        } else { //
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

void Tcmd::create(vector<string> cmd) {
    if (tolower(cmd[1]) == TABLE) {
        tables.push_back(cmd[2]);
        update_tables_from_memory();
        Table table;
        if (cmd.size() > 3) {
            if (cmd[3] == "(") {
                table.set_rows_size(1);
                for (size_t i = 3, j = 1; cmd[i] != ")"; i++, j++) {
                    if (cmd[i] != ")" && cmd[i] != "(") {
                        // TODO: переделать этот метод
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
            std::cout << "Ошибка удаления файла" << endl;
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
    if (tolower(cmd[1]) == TABLE) {
        // TODO: Реализовать работу команды ALTER
    } else {
        cout << "Ошибка!\n";
    }
}

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
    } else {

        cout << "Ошибка!\n";
        // TODO: Реализовать вывод по колонкам
        //    tbl.print_col([Номер(имя) колонки]) -  Выведет колонку
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
//                        tbl.set_rows_size(tbl.get_rows() + 1);
                        tbl.set_empty_row(1);
//                        tbl.set_rows_size(tbl.get_rows() + 1);  // + 1
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
                if (tolower(cmd[i]) == "set") {
//                    cout << "-->start set" << endl;
//                    vector<string> col_nval;
                    bool mrk_next_set = false;
//                    cout << "---->" << cmd[i] << endl;
                    for (size_t j = i; j < cmd.size(); j++) {
//                        cout << "---->" << cmd[j] << endl;
                        if (content_symbol(cmd[j], '=')) {
//                            cout << "--->content =" << endl;
                            col_nval.push_back(del_symbol(cmd[j], "="));
                            mrk_next_set = true;
                        } else if (mrk_next_set) {
                            col_nval.push_back(del_symbol(cmd[j], ","));
                            mrk_next_set = false;
                        }

                        if (tolower(cmd[j]) == "where") {
//                            vector<string> ID_val;
//                            cout << "-->start where" << endl;
                            bool mrk_next_where = false;
                            for (size_t k = j; k < cmd.size(); k++) {
//                                cout << "--->" << cmd[k] << endl;
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
            }  // UPDATE test1.csv SET c1= VAL, c2= VOL, WHERE c1= _;
            // TODO: реализация обновления данных
            Table tbl;
            tbl.read_file(cmd[1]);
            if ((ID_val.size() % 2 == 0) && (col_nval.size() % 2 == 0)) {
                for (size_t id_c = 0, id_r = 1; id_c < ID_val.size() && id_r < ID_val.size(); id_c += 2, id_r += 2) {
                    for (size_t r_i = 0; r_i < tbl.get_cols(); r_i++) {
                        if (tbl.get_elem(0, r_i) == ID_val[id_c]) {
                            for (size_t r_j = 0; r_j < tbl.get_rows(); r_j++) {
                                if (tbl.get_elem(r_j, r_i) == ID_val[id_r]) {
                                    // [РАБОТАЕТ] TODO: вот здесь корректно определяетс нужные строки
                                    for (size_t col_c = 0, col_v = 1; col_c < col_nval.size() && col_v < col_nval.size(); col_c += 2, col_v += 2) {
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
// dop func
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
