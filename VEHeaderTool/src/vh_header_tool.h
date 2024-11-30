#pragma once
#include "vh_header_file.h"
namespace VH 
{
	enum class Type 
	{
		Engine,
		Project
	};
	class HeaderTool 
	{
	public:
		HeaderTool(Type type);
		~HeaderTool();

		void GenerateEditorUIFile();
		void GenerateSerializationFile();
		void Prase();
		std::vector<HeaderFile> headerFiles;
		std::filesystem::path generatedFilepath;
		Type type;
	private:
	};
}