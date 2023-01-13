#include "table.h"

Table::Table(std::string path, std::string table_name) : table_name(table_name)
{
    this->table.open(path);
    this->sheet = table.workbook().worksheet(table.workbook().worksheetNames()[0]);

    OpenXLSX::XLRowIterator row = sheet.rows().begin();
    uint16_t column_index = 0;
    for (auto &&value : std::vector<OpenXLSX::XLCellValue>(row->values()))
    {
        if (value.type() != OpenXLSX::XLValueType::Empty)
        {
            this->columns.insert(std::make_pair(value.get<std::string>(), UNKNOWN));
            this->columns_type.insert(std::make_pair(value.get<std::string>(), UNKNOWN));
            this->table_head.insert(std::make_pair(value.get<std::string>(), column_index++));
        }
    }
}
std::map<std::string, enum ColumnType> Table::getColumns()
{
    return this->columns;
}
std::string Table::getName()
{
    return this->table_name;
};
bool Table::hasId(std::string id)
{
    return this->id_to_row_number.find(id) != this->id_to_row_number.end();
}
ColumnType Table::getColumnType(std::string column_name)
{
    return this->columns_type.at(column_name);
}

void Table::setColumnType(std::string column_name, std::string type)
{
    if (type == "number")
    {
        this->columns_type.at(column_name) = NUMBER;
    }
    else if (type == "json")
    {
        this->columns_type.at(column_name) = JSON;
    }
    else if (type == "string")
    {
        this->columns_type.at(column_name) = STRING;
    }
    else if (type == "array")
    {
        this->columns_type.at(column_name) = ARRAY;
    }
}
bool Table::checkColumnType(std::string column_name, std::string type)
{
    // this->sheet.column()
    // auto columns = this->sheet.column(0);
    // for (auto &&i : columns)
    // {
    // }

    if (type == "number")
    {
        /* code */
    }
    else if (type == "string")
    {
        /* code */
    }
    else if (type == "array")
    {
        /* code */
    }

    return false;
}
void Table::setPrimaryKey(std::string primary_key)
{
    std::set<std::int32_t> primary_keys;
    for (OpenXLSX::XLCellValue &v : this->getColumn(primary_key))
    {
        if (v.type() == OpenXLSX::XLValueType::Integer)
        {
            primary_keys.insert(v.get<std::int32_t>());
        }
        else if (v.type() == OpenXLSX::XLValueType::String)
        {
            primary_keys.insert(stoi(v.get<std::string>()));
        }
        else
        {
            break;
        }
    }
    this->primary_key = primary_keys;
}
void Table::insertForeignKeys(std::string foreign_key)
{
    if (this->foreign_keys.find(foreign_key) != this->foreign_keys.end())
    {
        return;
    }
    std::set<std::int32_t> foreign_keys;
    for (OpenXLSX::XLCellValue &v : this->getColumn(foreign_key))
    {
        if (v.type() == OpenXLSX::XLValueType::Integer)
        {
            // std::cout << m.get<int>() << std::endl;
            foreign_keys.insert(v.get<std::int32_t>());
        }
        else if (v.type() == OpenXLSX::XLValueType::String)
        {
            std::stringstream test(v.get<std::string>().c_str());
            std::string segment;
            std::vector<std::string> seglist;

            while (std::getline(test, segment, '|'))
            {
                try
                {
                    foreign_keys.insert(stoi(segment));
                }
                catch (const std::exception &e)
                {
                    if (this->table_name != "item_artifact_star")
                    {
                        std::cout << this->table_name << " column(" << foreign_key << ") has a value " << segment << " which can convert to int" << std::endl;
                    }
                    // std::cerr << e.what() << '\n';
                }
            }
        }
    }

    this->foreign_keys.insert(std::make_pair(foreign_key, foreign_keys));
}
void Table::init_primary_key_map(std::string primary_key)
{
    uint32_t column_index = this->table_head.at(primary_key);
    uint32_t row_index = 1;
    OpenXLSX::XLRowIterator row = this->sheet.rows().begin();
    while (row != this->sheet.rows().end())
    {
        OpenXLSX::XLCellValue cell_value;
        try
        {
            cell_value = (row->values<std::vector<OpenXLSX::XLCellValue>>()).at(column_index);
        }
        catch (const std::exception &e)
        {
            row_index--;
            break;
        }
        if (cell_value.type() == OpenXLSX::XLValueType::Integer)
            this->id_to_row_number.insert(std::make_pair(std::to_string(cell_value.get<int64_t>()), row_index));
        else if (cell_value.type() == OpenXLSX::XLValueType::String)
            this->id_to_row_number.insert(std::make_pair(cell_value.get<std::string>(), row_index));
        else
            break;
        row++;
        row_index++;
    }
    this->real_row_count = row_index - 1;
    this->data_row_count = row_index - 2;
}
int Table::getRealRowCount()
{
    return this->real_row_count;
}
int Table::getDataRowCount()
{
    return this->data_row_count;
}
std::map<std::string, std::set<std::int32_t>> Table::getForeignKeys()
{
    return this->foreign_keys;
}
std::map<std::string, std::uint32_t> Table::getTableHead()
{
    return this->table_head;
}
std::set<std::int32_t> Table::getForeignKey(std::string foreign_key)
{
    return this->foreign_keys.at(foreign_key);
}
int64_t Table::get_row_number_by_id(std::string id)
{
    return this->id_to_row_number.at(id);
}

std::map<std::string, OpenXLSX::XLCellValue> Table::getRowDataByRowNumber(int row_number)
{
    std::map<std::string, OpenXLSX::XLCellValue> row_table;
    OpenXLSX::XLRow row = this->sheet.row(row_number);
    std::vector<OpenXLSX::XLCellValue> row_data = row.values<std::vector<OpenXLSX::XLCellValue>>();
    for (auto &&i : this->table_head)
    {
        row_table.insert(std::make_pair(i.first, row_data.at(i.second)));
    }

    return row_table;
}

std::map<std::string, OpenXLSX::XLCellValue> Table::getRowData(std::string id)
{
    std::map<std::string, OpenXLSX::XLCellValue> row_table;
    OpenXLSX::XLRow row = this->sheet.row(this->get_row_number_by_id(id));
    std::vector<OpenXLSX::XLCellValue> row_data = row.values<std::vector<OpenXLSX::XLCellValue>>();
    for (auto &&i : this->table_head)
    {
        row_table.insert(std::make_pair(i.first, row_data.at(i.second)));
    }

    return row_table;
}

OpenXLSX::XLCellValue Table::getData(std::string id, std::string column_name)
{
    return this->sheet.row(this->id_to_row_number.at(id)).values<std::vector<OpenXLSX::XLCellValue>>().at(this->table_head.at(column_name));
}

std::set<std::int32_t> Table::getPrimaryKey()
{
    return this->primary_key;
}
std::vector<OpenXLSX::XLCellValue> Table::getColumnData(std::string column_name)
{
    if (this->data.find(column_name) == this->data.end())
    {
        OpenXLSX::XLRowRange rows = sheet.rows(2, this->real_row_count);
        std::vector<OpenXLSX::XLCellValue> column_data;
        auto column_index = this->table_head.at(column_name);
        for (auto &&row : rows)
        {
            column_data.push_back(row.values<std::vector<OpenXLSX::XLCellValue>>().at(column_index));
        }
        this->data.insert(std::make_pair(column_name, column_data));
    }
    return this->data.at(column_name);
}

// 没有取整列的方法 , 还是我没找到?
std::vector<OpenXLSX::XLCellValue> Table::getColumn(std::string column_name)
{
    if (this->data.find(column_name) == this->data.end())
    {
        OpenXLSX::XLRowIterator row = sheet.rows().begin();
        row++;
        std::vector<OpenXLSX::XLCellValue> column_data;
        ;
        while (row != this->sheet.rows().end())
        {
            try
            {
                OpenXLSX::XLCellValue m = std::vector<OpenXLSX::XLCellValue>(row->values()).at(this->table_head.at(column_name));
                if (m.type() == OpenXLSX::XLValueType::Empty)
                    break;
                column_data.push_back(m);

                row++;
            }
            catch (const std::exception &e)
            {
                // std::cout << "does " << this->table_name << " have " << this->sheet.rowCount() << " rows? I think it has some blank rows" << std::endl;
                break;
            }
        }
        // std::cout << this->table_name << " have " << column_data.size() << " rows" << std::endl;
        this->data.insert(std::make_pair(column_name, column_data));
    }
    return this->data.at(column_name);
}
Table::~Table()
{
    this->table.close();
}