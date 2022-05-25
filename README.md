# TpSQL

## Проект подготовительная программа по программированию на С/С++, весна 2022

Проект создавалася в учебных целях, собой представляет СУБД аналогичную SQL, с сохранением синтаксиса и функционала. 

### Команда:

* Степан Филимонов
    * Интерфейс
    * Сборка проекта
* Даниил Долгополов
    * Работа с таблицами

### Как собрать и запустить программу?
```bash
$ make
$ ./TpSQL
```

*Может не работать, проблема с директорией по умолчанию (24.05.2022), тогда надо ручками в мэйках путь поменять.*

### Команды
```
# Создание таблицы
CREATE TABLE [name] ( colname1, colname2, ... );

# Удаление таблицы
DROP TABLE [name];

# Вставка(в конец)|Удаление колонки
INSERT TABLE [name] ADD|DROP [colname];

# Переименновать таблицу
INSERT TABLE [name] RENAME TO [newname];
...
```
