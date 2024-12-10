#ifdef VE_EDITOR
#pragma once
#include "ve_defines.h"
namespace VE
{
	class VE_API SharedLibrary 
	{
	public:
		SharedLibrary();
		~SharedLibrary();
		//Do not provide shared library extension. the class handles the extension.
		void Load(std::filesystem::path filepath);
		void Unload();
		void* GetProcAddress(std::string name);
	private:
		void* platform_data;
	};
}
#endif