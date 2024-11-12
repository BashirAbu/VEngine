#include "platform/ve_platform.h"
#define VE_WIN32
#ifdef VE_WIN32
#include <Windows.h>
#include <iostream>

namespace VE 
{
	std::filesystem::path SaveFileDialog() 
	{
		std::filesystem::path path;
		
        OPENFILENAMEA ofn = {};
        char szFile[256];
        HWND hwndOwner = 0;
        ofn.lStructSize = sizeof(ofn);
        ofn.hwndOwner = hwndOwner;
        ofn.lpstrFile = szFile;
        ofn.lpstrFile[0] = '\0';
        ofn.nMaxFile = sizeof(szFile);
        ofn.lpstrFilter = "All\0*.*\0";
        ofn.lpstrDefExt = VE_SCENE_FILE_EXTENSION;
        ofn.nFilterIndex = 1;
        ofn.lpstrFileTitle = NULL;
        ofn.nMaxFileTitle = 0;
        ofn.lpstrInitialDir = NULL;
        ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR;

        if (GetSaveFileNameA(&ofn) != TRUE)
        {
            //TODO: do better logging
            std::cerr << "ERROR: failed to save file: " << ofn.lpstrFile << std::endl;
        }

        path = ofn.lpstrFile;
		return path;
	}
    std::filesystem::path OpenFileDialog()
    {
        std::filesystem::path path;

        OPENFILENAMEA ofn;
        char szFile[256];
        HWND hwndOwner = 0;
        ZeroMemory(&ofn, sizeof(ofn));
        ofn.lStructSize = sizeof(ofn);
        ofn.hwndOwner = hwndOwner;
        ofn.lpstrFile = szFile;
        ofn.lpstrFile[0] = '\0';
        ofn.nMaxFile = sizeof(szFile);
        ofn.lpstrFilter = "All\0*.*\0";
        ofn.nFilterIndex = 1;
        ofn.lpstrFileTitle = NULL;
        ofn.nMaxFileTitle = 0;
        ofn.lpstrInitialDir = NULL;
        ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

        if (GetOpenFileNameA(&ofn) != TRUE)
        {
            std::cerr << "ERROR: failed to open file: " << ofn.lpstrFile << std::endl;
        }

        path = ofn.lpstrFile;

        return path;
    }
}

#endif
