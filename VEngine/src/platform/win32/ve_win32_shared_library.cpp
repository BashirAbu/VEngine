#ifdef VE_EDITOR
#include "platform/ve_shared_library.h"
#ifdef VE_WINDOWS
#include <Windows.h>

namespace VE 
{
	SharedLibrary::SharedLibrary()
	{
	}
	SharedLibrary::~SharedLibrary()
	{
		if (platform_data)
		{
			FreeLibrary((HMODULE)platform_data);
		}
	}
	void SharedLibrary::Load(std::filesystem::path filepath)
	{
		filepath = filepath.string() + ".dll";
		platform_data = (void*)LoadLibraryA(filepath.string().c_str());
		VE_ASSERT(platform_data);
	}
	void SharedLibrary::Unload()
	{
		if (platform_data)
		{
			FreeLibrary((HMODULE)platform_data);
			platform_data = NULL;
		}
	}
	void* SharedLibrary::GetProcAddress(std::string name)
	{
		return ::GetProcAddress((HMODULE)platform_data, name.c_str());;
	}
}
#endif
#endif