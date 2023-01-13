#include "tableManager.h"
TableManager::TableManager() {}
TableManager *TableManager::instance = nullptr;
TableManager *TableManager::GetInstance()
{

    if (instance == nullptr)
    {
        instance = new TableManager();
    }
    return instance;
}

bool TableManager::isExsit(std::string table_name)
{
    return this->tables.find(table_name) != this->tables.end();
}
int TableManager::getTableNumber()
{
    return this->tables.size();
}

bool TableManager::addTable(std::string table_name)
{
    try
    {
        this->tables.insert(std::make_pair(table_name, new Table("./xlsx/" + table_name + ".xlsx", table_name)));
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
        return false;
    }
    return true;
}

Table *TableManager::getTable(std::string table_name)
{
    if (!this->isExsit(table_name))
    {
        this->addTable(table_name);
    }
    return this->tables.find(table_name)->second;
}

void TableManager::touchTable(std::string table_name)
{
    if (!this->isExsit(table_name))
    {
        this->addTable(table_name);
    }
}
