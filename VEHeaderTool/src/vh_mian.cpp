#include <iostream>
#include <vector>
#include <stdio.h>
#include "vh_header_tool.h"
int main(int argc, char** argv)
{
    std::string passedArgs = "";

    for (size_t i = 4; i < argc; i++)
    {
        passedArgs += (std::string)argv[i] + " ";
    }

   

    std::filesystem::path genSourcePath = (std::string)argv[2];
    std::filesystem::path searchPath = (std::string)argv[3];
    std::string passedType = argv[1];

    VH::HeaderTool headerTool(passedType == "e"? VH::Type::Engine : VH::Type::Project);
    headerTool.generatedFilepath = genSourcePath;

    for (auto headerFile : std::filesystem::recursive_directory_iterator(searchPath))
    {
        if (headerFile.is_regular_file())
        {
            if (headerFile.path().extension() == ".h" || headerFile.path().extension() == ".hpp")
            {
                std::filesystem::path headerFilepath = searchPath / headerFile.path().filename();

                std::string arguments = headerFilepath.generic_string() + " " + passedArgs;

                std::string command = "header-parser.exe " + arguments;
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
                VH::HeaderFile headerFile;
                headerFile.json = jsonHeader;
                headerFile.headerFilepath = std::filesystem::relative(headerFilepath, "../VEngine/src/");
                headerTool.headerFiles.push_back(headerFile);
            }
        }
    }


    headerTool.Prase();

    headerTool.GenerateSerializationFile();
    headerTool.GenerateEditorUIFile();

    return 0;
}