#pragma once
#include "ve_defines.h"
#include <filesystem>
namespace VE 
{
	VE_API std::filesystem::path SaveFileDialog(std::string extension = "");
	VE_API std::filesystem::path OpenFileDialog();
}