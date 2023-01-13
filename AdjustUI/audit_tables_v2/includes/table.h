#pragma once
#include <OpenXLSX.hpp>
#include <string>
#include <map>
#include <vector>
#include <set>
#include <nlohmann/json.hpp>
using json = nlohmann::json;

enum ColumnType
{
    ARRAY,
    JSON,
    NUMBER,
    STRING,
    UNKNOWN
};
class Table
{
private:
    std::string table_name;
    std::map<std::string, enum ColumnType> columns;
    std::map<std::string, enum ColumnType> columns_type;
    // id 对应的行数 , 就可以通过 id:string 来应数据了
    std::map<std::string, uint32_t> id_to_row_number;
    // xlsx 文件
    OpenXLSX::XLDocument table;
    // xlsx 文件左下角的分页
    OpenXLSX::XLWorksheet sheet;
    uint32_t real_row_count;
    uint32_t data_row_count;
    // 表头
    std::map<std::string, std::uint32_t> table_head;
    //数据
    std::map<std::string, std::vector<OpenXLSX::XLCellValue>> data;
    std::map<std::string, std::set<std::int32_t>> foreign_keys;
    std::set<std::int32_t> primary_key;

public:
    Table(std::string path, std::string table_name);
    std::map<std::string, std::uint32_t> getTableHead();
    std::vector<OpenXLSX::XLCellValue> getColumn(std::string column_name);
    std::vector<OpenXLSX::XLCellValue> getColumnData(std::string column_name);
    std::map<std::string, OpenXLSX::XLCellValue> getRowData(std::string id);
    std::map<std::string, OpenXLSX::XLCellValue> getRowDataByRowNumber(int row_number);
    //
    // std::vector<OpenXLSX::XLCellValue> getRawColumn(std::string column_name);
    // 主要是始初化 id_to_row_number id 到 行数的映射
    void init_primary_key_map(std::string primary_key);
    std::map<std::string, std::set<std::int32_t>> getForeignKeys();
    void insertForeignKeys(std::string foreign_key);
    void setPrimaryKey(std::string primary_key);
    int64_t get_row_number_by_id(std::string id);
    std::set<std::int32_t> getForeignKey(std::string foreign_key);
    std::set<std::int32_t> getPrimaryKey();
    OpenXLSX::XLCellValue getData(std::string id, std::string column_name);
    bool checkColumnType(std::string column_name, std::string type);
    bool hasId(std::string id);
    int getRealRowCount();
    int getDataRowCount();
    void setColumnType(std::string column_name, std::string type);
    ColumnType getColumnType(std::string column_name);
    std::string getName();
    std::map<std::string, enum ColumnType> getColumns();
    ~Table();
};
