#include <iostream>
#include <filesystem>
#include <stdio.h>
#include <vector>
#include <string.h>
#include <cassert>
#include "ve_assets_packager.h"



int main(int argc, char** argv)
{
    std::filesystem::path projectPath = argv[1];
    HeaderDesc headerDesc = {};
    
    int64_t offset = 0;
    std::vector<std::filesystem::path> assetsFullPath;

    for (auto file : std::filesystem::recursive_directory_iterator(projectPath))
    {
        if (file.is_regular_file())
        {
            if (file.path().extension().string() == (std::string)".VEProject")
            {
                FILE* assetFile = fopen(file.path().generic_string().c_str(), "rb");
                if (!assetFile)
                {
                    std::cerr << "Failed to open file: " << file.path().generic_string() << std::endl;
                }

                if (fseek(assetFile, 0, SEEK_END) != 0)
                {
                    std::cerr << "Failed to read size of file: " << file.path().generic_string() << std::endl;
                }


                size_t fileSize = ftell(assetFile);
                fclose(assetFile);


                AssetHeader header = {};
                std::filesystem::path relativePath = std::filesystem::relative(file.path().generic_string(), projectPath.generic_string());
                assert(relativePath.generic_string().size() < sizeof(header.name));
                strcpy(header.name, relativePath.generic_string().c_str());
                header.size = fileSize;
                header.offset = offset;
                headerDesc.headers.push_back(header);

                std::cout << file.path().filename() << std::endl;
                offset += fileSize;
                assetsFullPath.push_back(file.path());

                break;
            }
        }
    }


    for (auto file : std::filesystem::recursive_directory_iterator((projectPath / "assets")))
    {
        if (file.is_regular_file())
        {
            FILE* assetFile = fopen(file.path().generic_string().c_str(), "rb");
            if (!assetFile)
            {
                std::cerr << "Failed to open file: " << file.path().generic_string() << std::endl;
            }
            
            if (fseek(assetFile, 0, SEEK_END) != 0)
            {
                std::cerr << "Failed to read size of file: " << file.path().generic_string() << std::endl;
            }


            size_t fileSize = ftell(assetFile);
            fclose(assetFile);


            AssetHeader header = {};
            std::filesystem::path relativePath = std::filesystem::relative(file.path().generic_string(), (projectPath / "assets").generic_string());
            assert(relativePath.generic_string().size() < sizeof(header.name));
            strcpy(header.name, relativePath.generic_string().c_str());
            header.size = fileSize;
            header.offset = offset;
            headerDesc.headers.push_back(header);
    
            std::cout << file.path().filename() << std::endl;
            offset += fileSize;
            assetsFullPath.push_back(file.path());
        }
    }
    
    int64_t headerDescSize = sizeof(AssetHeader) * headerDesc.headers.size();

    FILE* dataFile = fopen("data.VEData", "wb");


    assert(fwrite(&headerDescSize, sizeof(headerDescSize), 1, dataFile));
    
    for (size_t i = 0; i < headerDesc.headers.size(); i++)
    {
        assert(fwrite(headerDesc.headers[i].name, sizeof(headerDesc.headers[i].name), 1, dataFile));
        assert(fwrite(&headerDesc.headers[i].size, sizeof(headerDesc.headers[i].size), 1, dataFile));
        assert(fwrite(&headerDesc.headers[i].offset, sizeof(headerDesc.headers[i].offset), 1, dataFile));
    }
    
    for (size_t i = 0; i < assetsFullPath.size(); i++)
    {
        FILE* assetFile = fopen(assetsFullPath[i].generic_string().c_str(), "rb");
        
        std::vector <char> buffer(headerDesc.headers[i].size);
        fread(buffer.data(), buffer.size(), 1, assetFile);
        fclose(assetFile);
        assert(fwrite(buffer.data(), buffer.size(), 1, dataFile));
        
    }
    fclose(dataFile);


    //try get data.

    //FILE* data = fopen("data.VEData", "rb");

    //int64_t headerDescSize = 0;

    //fread(&headerDescSize, sizeof(headerDescSize), 1, data);

    //int64_t sizeOfSingleHeader = sizeof(AssetHeader);

    //int64_t numberOfAssets = headerDescSize / sizeOfSingleHeader;

    //for (size_t i = 0; i < numberOfAssets; i++)
    //{
    //    AssetHeader header = {};
    //    fread(&header, sizeof(header), 1, data);
    //    std::cout << header.name << std::endl;


    //    if (strstr(header.name, "health"))
    //    {
    //        std::vector<uint8_t> buf(header.size, 0);
    //        fseek(data, headerDescSize + 8, SEEK_SET);
    //        fseek(data, header.offset, SEEK_CUR);

    //        fread(buf.data(), buf.size(), 1, data);

    //        std::cout << buf.data() << std::endl;

    //    }
    //    
    //}
    //fclose(data);
    return 0;
}