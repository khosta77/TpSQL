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
#define PATH_TO_TABLES "./project/tables/"
#define CSV ".csv"
// DDL - Data Definition Language
#define CREATE "create"
#define READ "read"
#define ALTER "alter"
#define DROP "drop"
// DML - Data Manipulation Language
#define SELECT "select"
#define INSERT "insert"
#define UPDATE "update"
//#define DELETE "delet"
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


/*
TODO: Чтобы таблица по умолчанию создавалась формата (.csv)
 - Добавлено

TODO: Считывание готовых таблиц
 - Добавлено

TODO: Команда DELETE

TODO: Красивый вывод таблицы

TODO: Проверка корректной работы с .csv

TODO: .gitignor
 - 90 %
 */
class Tcmd {
private:
    vector<string> tables;
public:
// Construct/destruct
    explicit Tcmd();

private:
    void get_tables_from_memory();  // Получение элементов tables из памяти или создание памяти
    void update_tables_from_memory();  // Обновление памяти

// Методы ниже представляют собой команды TpSQL
//  DDL - Data Definition Language
    void create(vector<string> cmd);
    void read(vector<string> cmd);
    void drop(vector<string> cmd);
    void alter(vector<string> cmd);
//  DML - Data Manipulation Language
    void select(vector<string> cmd);
    void insert(vector<string> cmd);
    void update(vector<string> cmd);
//    void delet(vector<string> cmd);

//  DCL - Data Control Language
    void show(vector<string> cmd);
private:
// Additional functions
    vector<string> get_split_cmd();  // Получение введенной команды для ее последующего анализа
    string del_symbol(string str, string c);  // Удалит в str символ c
    string tolower(string str);  // А -> а
    string touper(string str);  // a -> A
//    bool FileIsExist(string filePath);  // Существет файл или нет
    bool content_symbol(string str, char symbol);  // Содержит str символ symbol или нет
    int num_table(string table_name);  // Status - есть таблица в системе или нет (-1)
};

#endif  // PROJECT_INCLUDE_TCMD_H_
