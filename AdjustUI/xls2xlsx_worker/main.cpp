#include "Aspose.Cells.h"
#include <iostream>
#include <string>
#include <sys/stat.h>
#include <direct.h>
bool IsPathExist(const std::string &s)
{
    struct stat buffer;
    return (stat(s.c_str(), &buffer) == 0);
}

int main(int argc, char const *argv[])
{
    StringPtr sourcePath = new String("..\\");
    StringPtr resultPath = new String("xlsx\\");
    struct stat buffer;
    // boost::filesystem::path("xlsx")
    if (stat("xlsx", &buffer) != 0)
    {
        mkdir("xlsx");
    }

    // 第一个参数是自己 ..
    for (int i = 1; i < argc; i++)
    {
        std::string input = argv[i];
        std::cout << input << std::endl;
        intrusive_ptr<IWorkbook> wb = Factory::CreateIWorkbook(sourcePath->StringAppend(new String(input.append(".xls").c_str())));
        wb->Save(resultPath->StringAppend(new String(input.append("x").c_str())));
    }
    return 0;
}
