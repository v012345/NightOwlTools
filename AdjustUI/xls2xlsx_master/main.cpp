#include <iostream>
#include <filesystem>
#include <thread>
#include <vector>
#include <set>
#include <chrono>
#include <nlohmann/json.hpp>
#include <fstream>
#include <map>
using json = nlohmann::json;
int main(int argc, char const *argv[])
{
    // return 0;
    auto start = std::chrono::high_resolution_clock::now();
    // std::cout << "xls to xlsx..." << std::endl;
    // std::cout << "md5 need to be implemented" << std::endl;

    if (!system(".\\calculate_md5.exe"))
    {
        std::cout << "calculate_md5.exe done" << std::endl;
    }

    json xls_json;
    std::ifstream xls_json_input("xls_md5_map.json");
    xls_json_input >> xls_json;
    xls_json_input.close();

    if (!std::filesystem::exists("xlsx_md5_map.json"))
    {
        std::ofstream xlsx_md5("xlsx_md5_map.json");
        xlsx_md5 << "{}" << std::endl;
        xlsx_md5.close();
    }
    json xlsx_json;
    std::ifstream xlsx_json_input("xlsx_md5_map.json");
    xlsx_json_input >> xlsx_json;
    xlsx_json_input.close();

    std::vector<std::string> xlsFiles;
    std::vector<std::thread> cmds;

    for (auto &&file : std::filesystem::directory_iterator(std::filesystem::current_path().parent_path()))
    {

        if (file.is_regular_file() && file.path().extension().string() == ".xls")
        {

            if (
                xlsx_json[file.path().stem().string()].is_null() ||
                xls_json[file.path().stem().string()].get<std::string>() != xlsx_json[file.path().stem().string()].get<std::string>())
            {
                std::cout << file.path().stem().string() << std::endl;
                xlsFiles.push_back(file.path().stem().string());
            }

            // if (need_not_convert_xls.find(file.path().stem().string()) == need_not_convert_xls.end())
            // {
            //     xlsFiles.push_back(file.path().stem().string());
            // }
        }
    }
    int deal_count_per_thread = 30;
    int convert_count = 0;

    for (size_t i = 0; i <= xlsFiles.size() / deal_count_per_thread; i++)
    {
        std::string cmd = ".\\xls2xlsx_worker.exe ";
        for (size_t j = i * deal_count_per_thread; j < i * deal_count_per_thread + deal_count_per_thread; j++)
        {
            if (j >= xlsFiles.size())
                break;
            cmd.append("\"").append(xlsFiles[j]).append("\" ");
            convert_count++;
        }

        cmds.push_back(std::thread([cmd, i]()
                                   { system(cmd.c_str()); }));
    }
    for (auto &&cmd : cmds)
    {
        cmd.join();
    }
    std::cout << "convert " << convert_count << " xls files" << std::endl;

    std::ofstream xlsx_json_onput("xlsx_md5_map.json");
    xlsx_json_onput << std::setw(4) << xls_json;
    xlsx_json_onput.close();
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    std::cout << "convert xls to xlsx run time " << duration.count() / 1000000 << " s" << std::endl;
    return 0;
}
