#include <iostream>
#include "pugixml.hpp"
int main(int argc, char const *argv[])
{
    std::cout << "start adjusting ui" << std::endl;
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file("../cocosstudio/ui/UiFashionLotteryMain.csd");
    /* code */
    if (!result)
    {
        std::cout << "faile to parse csd" << std::endl;
        return 0;
    }

    std::cout << doc.child("GameFile").child("Content").child("Content").attribute("ctype").value() << std::endl;
    std::cout << "end adjusting ui" << std::endl;
    return 0;
}
