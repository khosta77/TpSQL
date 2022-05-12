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

#define CREATE "create"
#define ALTER "alter"
#define DROP "drop"

#define SELECT "select"
#define INSERT "insert"
#define UPDATE "update"

#define FROM "from"
#define EXIT "exit"

#define SHOW "show"

// https://habr.com/ru/post/564390/ - материал с командами

/*
 * CREATE TABLE test ( col1, col2, col3);
 * // Не надо указывать тип таблицы, по умолчанию csv
 *
 * SELECT * FROM test;
 * // вывести ВСЕ элементы
 * // если вместо * col вывести все под col
 *
 * */
#define MRK_TST 1

static void fprintf(string str) {
    if (MRK_TST == 1) {
        std::cout << str << endl;
    }
}

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
    void create(vector<string> cmd);
    void alter(vector<string> cmd);  // Пока что нету
    void drop(vector<string> cmd);
    // DML
    void select(vector<string> cmd);
    void insert(vector<string> cmd);
    void update(vector<string> cmd);
    // DCL
    void show(vector<string> cmd);
private:
    vector<string> get_split_cmd();
    string del_symbol(string str, string c);
    string tolower(string str);
    string touper(string str);
    bool FileIsExist(std::string filePath);
    int num_table(string table_name);
};

// Construct/destruct
Tcmd::Tcmd() {
    get_tables_from_memory();
    cout << "===";
    for (vector<string> cmd;;) {
        cmd = get_split_cmd();
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
        } else if ((cmd[0] == SHOW) && cmd.size() > 2) {
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
    if (FileIsExist("./project/data.txt")) {
        ifstream fin;
        fin.open("./project/data.txt");
        string line;
        while (getline(fin, line)) {
            tables.push_back(line);
        }
        fin.close();
        cout << "YES\n";
    } else {
        cout << "NO\n";
        ofstream fout;
        fout.open("./project/data.txt");
        fout.close();
    }
}

void Tcmd::update_tables_from_memory() {
    ofstream fout;
    fout.open("./project/data.txt");
    for (size_t i = 0; i < tables.size(); i++) {
        fout << tables[i] << endl;
    }
    fout.close();
}

void Tcmd::create(vector<string> cmd) {
    if (tolower(cmd[1]) == "table") {
        tables.push_back(cmd[2]);
        update_tables_from_memory();
        Table table;
        if (cmd.size() > 3) {
            if (cmd[3] == "(") {
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
    if (tolower(cmd[1]) == "table") {
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
    if (tolower(cmd[1]) == "table") {
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
                tbl.out_table();
            }
        }
    } else {
        cout << "Ошибка!\n";
        // TODO: Реализовать вывод по колонкам
        // 11.05
        // Есть вопросы с тем как лучше реализовать этот метод
        //   1) Можно считать всю таблицу и в этом методе пойти по колонкам
        //   2) Можно в классе Table
        // ???
    }
}

void Tcmd::insert(vector<string> cmd) {
    if (tolower(cmd[1]) == "into") {
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
        cout << cmd[mrk_end_col + 1] << endl;
        if (tolower(cmd[mrk_end_col + 1]) == "values") {
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
                        tbl.set_rows_size();  // + 1
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

}

void Tcmd::show(vector<string> cmd) {
    if (tolower(cmd[1]) == "tables") {
        for (size_t i = 0; i < tables.size(); i++) {
            cout << tables[i] << endl;
        }
    } else {
        cout << "Таблицы отсутствуют" << endl;
    };
}

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

bool Tcmd::FileIsExist(std::string filePath) {
    bool isExist = false;
    std::ifstream fin(filePath.c_str());
    if(fin.is_open()) {
        isExist = true;
    }
    fin.close();
    return isExist;
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
