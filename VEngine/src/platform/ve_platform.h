#pragma once
#include "ve_defines.h"
#include <filesystem>
namespace VE 
{
	std::filesystem::path SaveFileDialog();
	std::filesystem::path OpenFileDialog();
}