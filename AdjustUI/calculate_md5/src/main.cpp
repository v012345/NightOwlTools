#include "md5file.h"
#include <iostream>
#include <filesystem>
#include <map>
#include <string>
#include <fstream>

int main(int argc, char const *argv[])
{
    std::map<std::string, std::string> simpleJson;
    // std::cout << getFileMD5(.append("action_attack.xls").string()) << std::endl;
    for (auto &&xls : std::filesystem::directory_iterator(std::filesystem::current_path().parent_path()))
    {
        if (xls.path().extension() == ".xls")
        {
            // std::cout << xls.path().stem().string() << std::endl;
            // std::cout << getFileMD5(xls.path().string()) << std::endl;
            simpleJson.insert(std::make_pair(xls.path().stem().string(), getFileMD5(xls.path().string())));
        }
    }
    std::ofstream json("xls_md5_map.json");
    json << "{" << std::endl;
    auto last = simpleJson.rbegin();
    std::string last_name = (*last).first;
    for (auto &&i : simpleJson)
    {
        json << "    \"" << i.first << "\": \"" << i.second << "\"";
        last_name != i.first &&json << ",";
        json << std::endl;
    }
    json << "}" << std::endl;

    return 0;
}