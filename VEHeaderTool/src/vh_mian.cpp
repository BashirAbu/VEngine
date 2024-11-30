#include <iostream>
#include <vector>
#include <stdio.h>
int main(int argc, char** argv)
{

    std::string passedArgs = "";

    for (size_t i = 1; i < argc; i++)
    {
        passedArgs += (std::string)argv[i] + " ";
    }

    std::string command = "header-parser.exe " + passedArgs;
    FILE* jsonHeaderFile = _popen(command.c_str(), "r");
    fseek(jsonHeaderFile, 0L, SEEK_END);
    size_t jsonHeaderSize = (size_t)ftell(jsonHeaderFile);
    char buffer[256] = {};
    std::string jsonHeader = "";
    while (fgets(buffer, sizeof(buffer), jsonHeaderFile) != nullptr)
    {
        jsonHeader += buffer;
    }

    std::cout << jsonHeader << std::endl;

    return 0;
}