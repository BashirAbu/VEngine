#pragma once
#include "ve_defines.h"
#include <unordered_map>
#include <string>
#include <raylib.h>
#include <filesystem>
namespace VE 
{
	class VE_API AssetsManager 
	{
	public:
		~AssetsManager();

		static AssetsManager* GetSingleton();

		Texture* LoadTexture(std::filesystem::path filepath);
		Image* LoadImage(std::filesystem::path filepath);

	private:
		AssetsManager();
		void Clear();

		std::filesystem::path assetsFolderPath;
		std::unordered_map<std::string, Texture> textures;
		std::unordered_map<std::string, Image> images;
		friend class SceneManager;
		friend class Engine;
	};
}