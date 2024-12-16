#pragma once
#include <filesystem>
namespace VH 
{
	struct Meta 
	{
		std::string key;
		std::string value;
	};
	struct ParsedName
	{
		std::vector<Meta> meta;
		std::vector<std::string> nameSpaces;
		std::string dataType = "";
		std::string name = "";
	};

	struct Component
	{
		ParsedName name;
		std::vector<ParsedName> properites;
	};

	struct Enum 
	{
		std::string name = "";
		std::vector<std::string> nameSpaces;
		std::vector<std::string> items;
		std::vector<Meta> meta;
	};

	struct System 
	{
		std::string name;
		std::vector<Meta> meta;
		std::vector<std::string> nameSpaces;
		std::vector<ParsedName> components;
	};

	struct Callback
	{
		std::string name;
		std::vector<Meta> meta;
		std::vector<std::string> nameSpaces;
	};

	class HeaderFile 
	{
	public:
		HeaderFile();
		~HeaderFile();

		void Prase();

		std::string json;
		std::vector<Component> components;
		std::vector<Enum> enums;
		std::vector<System> systems;
		std::vector<Callback> callbacks;
		std::filesystem::path headerFilepath;
	private:
	};
}