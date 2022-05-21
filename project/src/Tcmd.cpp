#include "../include/Tcmd.h"

//
// Construct/destruct
//
Tcmd::Tcmd() {
    get_tables_from_memory();
    cout << "===";
    for (vector<string> cmd;;) {
        cmd = get_split_cmd();
        if ((cmd[0] == CREATE || cmd[0] == DROP || cmd[0] == ALTER || cmd[0] == READ) && cmd.size() > 2) {
            if (cmd[0] == CREATE) {
                create(cmd);
            }else if (cmd[0] == READ) {
                read(cmd);
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
//            else if (cmd[0] == DELETE) {
//                delet(cmd);
//            }
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
    fstream flog(PATH_DATA_LOG);
    if (flog.is_open()) {
        string line;
        while (getline(flog, line)) {
            tables.push_back(line);
        }
        flog.close();
    } else {
        flog.close();
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
        if (cmd.size() > 3) {
            if (cmd[3] == "(") {
                Table tbl;
                tbl.set_rows_size(1);
                for (size_t i = 4; i < cmd.size(); i++) {
                    if (cmd[i] == ")") {
                        break;
                    } else {
                        tbl.push_col(del_symbol(cmd[i], ","));
                    }
                }
                tbl.write_file(PATH_TO_TABLES + cmd[2] + CSV);
                this->tables.push_back(cmd[2] + CSV);
                update_tables_from_memory();
            } else {
                std::cout << "Ошибка!\n" << "--> Несответствие команде" << endl;
            }
        } else {
            std::cout << "Ошибка!\n" << "--> Несответствие команде" << endl;
        }
    } else {
        cout << "Ошибка!\n";
    }
}

// READ TABLE test_read [path];
void Tcmd::read(vector<string> cmd) {
    if (tolower(cmd[1]) == TABLE && cmd.size() == 4) {
        Table tbl;
        tbl.read_file(cmd[3]);
        if (tbl.table_empty()) {
            std::cout << "Ошибка!\n" << "--> Файл пуст/не существует" << endl;
            return;
        }
        tbl.write_file(PATH_TO_TABLES + cmd[2] + CSV);
        this->tables.push_back(cmd[2] + CSV);
        update_tables_from_memory();
    } else {
        cout << "Ошибка!\n";
    }
}

void Tcmd::drop(vector<string> cmd) {
    if (tolower(cmd[1]) == TABLE) {
        if (remove((PATH_TO_TABLES + cmd[2] + CSV).c_str()) != 0) {
            std::cout << "Ошибка!\n" << "--> Ошибка удаления файла" << endl;
        } else {
            for (size_t i = 0; i < tables.size(); i++) {
                if (tables[i] == (cmd[2] + CSV)) {
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
        switch (num_table(cmd[2] + CSV)) {
            case -1: {
                cout << "Ошибка!\n" << "--> Таблица не найдена!" << endl;
                break;
            }
            default: {
                if (tolower(cmd[3]) == ADD) {  // Добавление столбца в конец
                    Table tbl;
                    tbl.read_file(PATH_TO_TABLES + cmd[2] + CSV);
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
                    new_tbl.write_file(PATH_TO_TABLES + cmd[2] + CSV);
                } else if (tolower(cmd[3]) == DROP) {
                    Table tbl;
                    tbl.read_file(PATH_TO_TABLES + cmd[2] + CSV);
                    Table new_tbl;
                    size_t control_del = -1;
//                    tbl.out_str();
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
                    new_tbl.write_file(PATH_TO_TABLES + cmd[2] + CSV);
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
                    tbl.read_file(PATH_TO_TABLES + cmd[2]);
                    if (remove((PATH_TO_TABLES + cmd[2]).c_str()) != 0) {
                        std::cout << "Ошибка!\n" << "--> Переименновывания файла" << endl;
                    } else {
                        for (size_t i = 0; i < tables.size(); i++) {
                            if (tables[i] == (cmd[2] + CSV)) {
                                tables.erase(tables.begin() + i);
                                vector<string>(tables).swap(tables);
                            }
                        }
                        update_tables_from_memory();
                        tables.push_back(cmd[5]);
                        update_tables_from_memory();
                        tbl.write_file(PATH_TO_TABLES + cmd[5] + CSV);
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
        switch (num_table(cmd[3] + CSV)) {
            case -1: {
                cout << "Таблица не найдена!" << endl;
                break;
            }
            default: {
                Table tbl;
                tbl.read_file(PATH_TO_TABLES + cmd[3] + CSV);
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
        switch (num_table(cmd[num_path]  + CSV)) {
            case -1: {
                cout << "Таблица не найдена!" << endl;
                break;
            }
            default: {
                Table tbl, tbl_new;
                for (size_t i = 0; i < col_name.size(); i++) {
                    tbl_new.push_col(col_name[i]);
                }
                tbl.read_file(PATH_TO_TABLES + cmd[num_path] + CSV);
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
    if (tolower(cmd[1]) == INTO && cmd.size() >= 4) {
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
                switch (num_table(cmd[2] + CSV)) {
                    case -1: {
                        cout << "Ошибка!\n" << "--> Таблица не найдена!\n";
                        break;
                    }
                    default: {
                        Table tbl;
                        tbl.read_file(PATH_TO_TABLES + cmd[2] + CSV);
                        tbl.set_empty_row(1);
                        for (size_t i = 0; i < col.size(); i++) {
                            for (size_t j = 0; j < tbl.get_cols(); j++) {
                                if (tbl.get_elem(0, j) == col[i]) {
                                    tbl.set_elem(tbl.get_rows() - 1, j, val[i]);
                                }
                            }
                        }
                        tbl.write_file(PATH_TO_TABLES + cmd[2] + CSV);
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
    switch (num_table(cmd[1] + CSV)) {
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
            tbl.read_file(PATH_TO_TABLES + cmd[1] + CSV);
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
                tbl.write_file(PATH_TO_TABLES + cmd[1] + CSV);
            } else {
                cout << "Ошибка!\n" << "--> col или row без значения\n";
            }
        }
    }
}

//void Tcmd::delet(vector<string> cmd) {
//    switch (num_table(cmd[2])) {
//        case -1: {
//            cout << "Ошибка!\n" << "--> Таблица не найдена!\n";
//            break;
//        }
//        default: {
//            if (cmd.size() == 3) {
//                Table tbl;
//                tbl.read_file(PATH_TO_TABLES + cmd[2]);
//                for (size_t i = 0; i < tbl.get_rows() * tbl.get_cols() - tbl.get_cols(); i++) {
//                    tbl.del_last();
//                }
//                tbl.set_rows_size(1);
//                tbl.write_file(PATH_TO_TABLES + cmd[2]);
//            } else {
//                Table tbl;
//                tbl.read_file(PATH_TO_TABLES + cmd[2]);
//                string col_where_key_word_for_delete;
//                for (size_t i = 0; i < cmd[4].size(); i++) {
//                    if (cmd[4][i] != '=') {
//                        col_where_key_word_for_delete = col_where_key_word_for_delete + cmd[4][i];
//                    }
//                }
//
//                size_t row_num_for_delete;
//                for (size_t i = 0; i < tbl.get_cols(); i++) {
//                    if (tbl.get_elem(0, i) == col_where_key_word_for_delete) {
//                        for (size_t j = 0; j < tbl.get_rows(); j++) {
//                            if (tbl.get_elem(i,j) == cmd[5]) {
//                                row_num_for_delete = j;
//                                break;
//                            }
//                        }
//                    }
//                }
//
//                vector<string> new_tab;
//                for (size_t i = 0; i < tbl.get_rows(); i++) {
//                    if (i !=  row_num_for_delete) {
//                        for (size_t j = 0; j < tbl.get_cols(); j++) {
//                            new_tab.push_back(tbl.get_elem(i, j));
//                        }
//                    }
//                }
//
//                tbl.set_table(new_tab);
//                tbl.set_rows_size(tbl.get_rows() - 1);
//                tbl.write_file(PATH_TO_TABLES + cmd[2]);
//            }
//            break;
//        }
//    }
//}

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

