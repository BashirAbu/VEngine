#pragma once
#include "ve_defines.h"
#include <raylib.h>
#include <mutex>

namespace VE 
{
	class Font;

	class VE_API AssetsManager 
	{
	public:
		~AssetsManager();

		static AssetsManager* GetSingleton();

		Texture* LoadTexture(std::filesystem::path filepath);
		Image* LoadImage(std::filesystem::path filepath);
		Sound* LoadSound(std::filesystem::path filepath);
		Font* LoadFont(std::filesystem::path filepath, int32_t fontSize);

	private:
		AssetsManager();
		void Clear();

		std::filesystem::path assetsFolderPath;
		std::unordered_map<std::string, Texture> textures;
		std::mutex texturesMutex;
		std::unordered_map<std::string, Image> images;
		std::mutex imagesMutex;
		std::unordered_map<std::string, Sound> sounds;
		std::mutex soundsMutex;
		std::unordered_map<std::string, Font*> fonts;
		std::mutex fontsMutex;
		friend class SceneManager;
		friend class Engine;
	};
}