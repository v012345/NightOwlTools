#include <iostream>
#include <cstdlib>
#include <fstream>
#include <filesystem>
#include <string>
namespace fs = std::filesystem;

// 不复制 .git , .svn , .vscode , logDir 和 空文件夹
void duplicateFiles(fs::path form, fs::path to);
void duplicateFiles(fs::path form, fs::path to, fs::path root);

int main(int argc, char const *argv[])
{
    std::cout << "start DuplicateDirectory.exe" << std::endl;
    duplicateFiles(argv[1], argv[2]);
    std::cout << "end DuplicateDirectory.exe" << std::endl;
    return 0;
}

void duplicateFiles(fs::path form, fs::path to)
{
    duplicateFiles(form, to, form);
}

void duplicateFiles(fs::path form, fs::path to, fs::path root)
{
    for (auto &&directoryOrFile : fs::directory_iterator(form))
    {
        if (directoryOrFile.is_directory())
        {
            std::string directoryName = directoryOrFile.path().filename().string();
            if (directoryName != ".git" && directoryName != ".svn" && directoryName != ".vscode" && directoryName != "logDir")
            {
                duplicateFiles(directoryOrFile, to, root);
            }
        }
        else
        {
            if (directoryOrFile.path().filename().string() != ".gitignore")
            {
                auto toFolder = to.string() + "/" + fs::relative(directoryOrFile.path(), root).parent_path().string();
                if (!fs::exists(toFolder))
                {
                    fs::create_directories(toFolder);
                }
                std::cout << "copy " + directoryOrFile.path().string() << std::endl;
                fs::copy(directoryOrFile, toFolder, fs::copy_options::update_existing);
            }
        }
    }
}
