#include "table.h"
#include <string>
#include <iostream>
#include <map>
#include <set>
#include <vector>
#include <sstream>
#include <filesystem>
#include <fstream>
#include <nlohmann/json.hpp>
#include <typeinfo>
#include <thread>
#include <chrono>
#include "tableManager.h"
#include "auditor.h"
// include Lua headers
extern "C"
{
#include "lua-5.4.4/src/lua.h"
#include "lua-5.4.4/src/lualib.h"
#include "lua-5.4.4/src/lauxlib.h"
}
using namespace std::chrono;

// namespace fs = std::filesystem;
using json = nlohmann::json;
using namespace OpenXLSX;

TableManager *tableManager = TableManager::GetInstance();

static int GetTableDataRowNumber(lua_State *L)
{
    const char *table_name = lua_tostring(L, 1);
    lua_Integer row_count = tableManager->getTable(table_name)->getDataRowCount();
    lua_pushinteger(L, row_count);
    return 1;
}

static int GetRowDataById(lua_State *L)
{

    const char *table_name = lua_tostring(L, 1);
    const char *id = lua_tostring(L, 2);
    lua_newtable(L);
    Table *table = tableManager->getTable(table_name);
    if (!table->hasId(id))
    {
        std::cerr << "表 " << table_name << " 不存在 id " << id << "\n";
        return 0;
    }

    for (auto &&i : table->getRowData(id))
    {
        lua_pushstring(L, i.first.c_str());
        if (i.second.type() == OpenXLSX::XLValueType::Integer)
        {
            lua_pushstring(L, std::to_string(i.second.get<int64_t>()).c_str());
        }
        else if (i.second.type() == OpenXLSX::XLValueType::Float)
        {
            lua_pushstring(L, std::to_string(i.second.get<float>()).c_str());
        }
        else
        {
            lua_pushstring(L, i.second.get<std::string>().c_str());
        }

        lua_settable(L, -3);
    }
    return 1;

    // lua_newtable(L);             //创建一个表格，放在栈顶
    // lua_pushstring(L, "mydata"); //压入key
    // lua_pushnumber(L, 66);       //压入value
    // lua_settable(L, -3);         //弹出key,value，并设置到table里面去

    // lua_pushstring(L, "subdata"); //压入key
    // lua_newtable(L);              //压入value,也是一个table
    // lua_pushstring(L, "mydata");  //压入subtable的key
    // lua_pushnumber(L, 53);        // value
    // lua_settable(L, -3);          //弹出key,value,并设置到subtable

    // lua_settable(L, -3);          //这时候父table的位置还是-3,弹出key,value(subtable),并设置到table里去
    // lua_pushstring(L, "mydata2"); //同上
    // lua_pushnumber(L, 77);
    // lua_settable(L, -3);
    // return 1; //堆栈里现在就一个table.其他都被弹掉了。
}

static int IsHasId(lua_State *L)
{
    const char *table_name = lua_tostring(L, 1);
    const char *id = lua_tostring(L, 2);
    Table *table = tableManager->getTable(table_name);
    lua_pushboolean(L, table->hasId(id));
    return 1;
}

static int GetColumnByName(lua_State *L)
{
    const char *table_name = lua_tostring(L, 1);
    const char *column_name = lua_tostring(L, 2);
    lua_newtable(L);
    Table *table = tableManager->getTable(table_name);
    std::vector<OpenXLSX::XLCellValue> column_data;
    try
    {
        column_data = table->getColumnData(column_name);
    }
    catch (const std::exception &e)
    {
        std::cerr << "表 " << table_name << " 不存在 " << column_name << " 列\n";
        return 0;
    }

    int i = 1;
    for (auto &&data : column_data)
    {
        lua_pushinteger(L, i);
        if (data.type() == OpenXLSX::XLValueType::Integer)
        {
            lua_pushstring(L, std::to_string(data.get<int64_t>()).c_str());
        }
        else if (data.type() == OpenXLSX::XLValueType::Float)
        {
            lua_pushstring(L, std::to_string(data.get<float>()).c_str());
        }
        else
        {
            lua_pushstring(L, data.get<std::string>().c_str());
        }
        lua_settable(L, -3);
        i++;
    }

    // lua_pushboolean(L, table->hasId(id));
    return 1;
}
static int GetRowDataByRowNumber(lua_State *L)
{
    const char *table_name = lua_tostring(L, 1);
    int row_number = lua_tointeger(L, 2) + 1; // lua 没有表头 , c++ 里有表头 , 所以差了一行
    lua_newtable(L);
    Table *table = tableManager->getTable(table_name);
    if (row_number < 1 || row_number > table->getRealRowCount())
    {
        std::cerr << "表 " << table_name << " 不存在 " << row_number << " 行\n";
        return 0;
    }

    for (auto &&i : table->getRowDataByRowNumber(row_number))
    {
        lua_pushstring(L, i.first.c_str());
        if (i.second.type() == OpenXLSX::XLValueType::Integer)
        {
            lua_pushstring(L, std::to_string(i.second.get<int64_t>()).c_str());
        }
        else if (i.second.type() == OpenXLSX::XLValueType::Float)
        {
            lua_pushstring(L, std::to_string(i.second.get<float>()).c_str());
        }
        else
        {
            lua_pushstring(L, i.second.get<std::string>().c_str());
        }
        lua_settable(L, -3);
    }
    return 1;
}
int main(int argc, const char *argv[])
{
    system("chcp 65001");

    // *** call Lua function from C++ ***
    // lua_getglobal(L, "MyLuaFunction");    // find the Lua function
    // lua_pushnumber(L, 73);                // push number as first param
    // lua_pushstring(L, "From C++ to Lua"); // push string as second param
    // lua_pcall(L, 2, 0, 0);                // call the function passing 2 params

    system(".\\xls2xlsx_master.exe");
    json rule;
    std::ifstream("rule.json") >> rule;
    audit_init_table_config(rule["table_config"]);
    // audit_column_type(rule["column_type_check"]);
    // std::cout << tableManager->getTable("entity_ai")->getDataRowCount() << std::endl;
    // audit_has_one_conditions(rule["has_one_conditions"]);
    lua_State *L = luaL_newstate();                                  // create a new lua instance
    luaL_openlibs(L);                                                // give lua access to basic libraries
    lua_register(L, "GetTableDataRowNumber", GetTableDataRowNumber); // register our C++ function with Lua
    lua_register(L, "GetRowDataById", GetRowDataById);               // register our C++ function with Lua
    lua_register(L, "IsHasId", IsHasId);                             // register our C++ function with Lua
    lua_register(L, "GetColumnByName", GetColumnByName);             // register our C++ function with Lua
    lua_register(L, "GetRowDataByRowNumber", GetRowDataByRowNumber); // register our C++ function with Lua
    luaL_dofile(L, "./scripts/main.lua");                            // loads the Lua script
    std::cout << "handled " << tableManager->getTableNumber() << " tables" << std::endl;
    return 0;
}
