#pragma once
#include "table.h"
#include <map>
// 先不考虑值copy的问题 , 先不使用指针
class TableManager
{
private:
    std::map<std::string, Table*> tables;

protected:
    TableManager();
    static TableManager *instance;

public:
    TableManager(TableManager &other) = delete;
    void operator=(const TableManager &) = delete;
    static TableManager *GetInstance();
    Table* getTable(std::string table_name);
    bool addTable(std::string table_name);
    void touchTable(std::string table_name);
    bool isExsit(std::string table_name);
    int getTableNumber();
};
