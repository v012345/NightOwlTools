#include <iostream>
#include "pugixml.hpp"

void traverse();

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

void traverse()
{
    for (pugi::xml_node tool = tools.child("Tool"); tool; tool = tool.next_sibling("Tool"))
    {
        std::cout << "Tool " << tool.attribute("Filename").value();
        std::cout << ": AllowRemote " << tool.attribute("AllowRemote").as_bool();
        std::cout << ", Timeout " << tool.attribute("Timeout").as_int();
        std::cout << ", Description '" << tool.child_value("Description") << "'\n";
    }

    for (pugi::xml_node_iterator it = tools.begin(); it != tools.end(); ++it)
    {
        std::cout << "Tool:";

        for (pugi::xml_attribute_iterator ait = it->attributes_begin(); ait != it->attributes_end(); ++ait)
        {
            std::cout << " " << ait->name() << "=" << ait->value();
        }

        std::cout << std::endl;
    }
}
