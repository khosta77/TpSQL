#ifndef PROJECT_INCLUDE_TCMD_H_
#define PROJECT_INCLUDE_TCMD_H_

#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <algorithm>
#include <cctype>

using namespace std;

#define CREATE "create"
#define ALTER "alter"
#define DROP "drop"

#define SELECT "select"


#define FROM "from"
#define EXIT "exit"

// https://habr.com/ru/post/564390/ - материал с командами

/*
 * CREATE TABLE test;
 * // Не надо указывать тип таблицы, по умолчанию csv
 *
 * SELECT * FROM test;
 * // вывести ВСЕ элементы
 * // если вместо * col вывести все под col
 *
 * */

class Tcmd {
private:
public:
    // Construct/destruct
    explicit Tcmd();

private:
    // DDL
    void create(vector<string> cmd);
    void alter(vector<string> cmd);
    void drop(vector<string> cmd);
    // DML
    void select(vector<string> cmd);
    void insert(vector<string> cmd);
    void update(vector<string> cmd);
//    void delete(vector<string>);
    // DCL
    void show(vector<string> cmd);
private:
    vector<string> get_split_cmd();
    string tolower(string str);
    string touper(string str);
};

// Construct/destruct
Tcmd::Tcmd() {
    cout << "===";
    for (vector<string> cmd;;) {
        cmd = get_split_cmd();
        if (cmd[0] == CREATE || cmd[0] == DROP || cmd[0] == ALTER) {  //
            cout << touper(cmd[0]) << ":\n";
            for (size_t i = 1; i < cmd.size(); i++) {
                cout << "\tcmd[" << i << "]:" << cmd[i] << endl;
            }
            if (cmd[0] == CREATE) {
                create(cmd);
            } else if (cmd[0] == DROP) {
                drop(cmd);
            } else if (cmd[0] == ALTER) {
                alter(cmd);
            }
        } else if (cmd[0] == SELECT) {  //
            cout << touper(cmd[0]) << ":\n";
            for (size_t i = 1; i < cmd.size(); i++) {
                if (tolower(cmd[i]) == FROM) {
                    cout << "FROM:\n";
                } else {
                    cout << "\tcmd[" << i << "]:" << cmd[i] << endl;
                }
            }
        } else if (cmd[0] == EXIT) { //
            cout << "Выход\n";
            break;
        } else { //
            cout << "Ошибка!\n";
        }
    }
}

void Tcmd::create(vector<string> cmd) {
    if (tolower(cmd[1]) == "table") {
// Создать таблицу с именем в переменной cmd[2]
    } else {
        cout << "Ошибка!\n";
    }
}

void Tcmd::alter(vector<string> cmd) {
    if (tolower(cmd[1]) == "table") {
// Модифицировать таблицу с именем в переменной cmd[2]
    } else {
        cout << "Ошибка!\n";
    }
}

void Tcmd::drop(vector<string> cmd) {
    if (tolower(cmd[1]) == "table") {
// Удалить таблицу с именем в переменной cmd[2]
    } else {
        cout << "Ошибка!\n";
    }
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

#endif  // PROJECT_INCLUDE_TCMD_H_
