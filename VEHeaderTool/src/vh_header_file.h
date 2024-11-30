#pragma once
#include <filesystem>
namespace VH 
{
	struct ParsedName
	{
		std::vector<std::string> meta;
		std::vector<std::string> nameSpaces;
		std::string dataType = "";
		std::string name = "";
	};

	struct Component
	{
		ParsedName name;
		std::vector<ParsedName> properites;
	};

	struct System 
	{
		std::string name;
		std::vector<std::string> meta;
		std::vector<std::string> nameSpaces;
		std::vector<ParsedName> components;
	};

	class HeaderFile 
	{
	public:
		HeaderFile();
		~HeaderFile();

		void Prase();

		std::string json;
		std::vector<Component> components;
		std::vector<System> systems;
		std::filesystem::path headerFilepath;
	private:
	};
}