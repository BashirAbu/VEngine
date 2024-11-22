#pragma once
#include "ve_defines.h"
#include <raylib.h>

namespace VE 
{
	class VE_API AssetsManager 
	{
	public:
		~AssetsManager();

		static AssetsManager* GetSingleton();

		Texture* LoadTexture(std::filesystem::path filepath);
		Image* LoadImage(std::filesystem::path filepath);
		Sound* LoadSound(std::filesystem::path filepath);

	private:
		AssetsManager();
		void Clear();

		std::filesystem::path assetsFolderPath;
		std::unordered_map<std::string, Texture> textures;
		std::unordered_map<std::string, Image> images;
		std::unordered_map<std::string, Sound> sounds;
		friend class SceneManager;
		friend class Engine;
	};
}