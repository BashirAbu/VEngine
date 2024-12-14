#pragma once
#include "ve_defines.h"
#include <raylib.h>
#include <mutex>
#include <stdio.h>
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
		Model* LoadModel(std::filesystem::path filepath);


		const std::string& LoadScene(std::filesystem::path filepath);
		const std::string& LoadConstruct(std::filesystem::path filepath);
		const std::string& LoadProject(std::filesystem::path filepath);

#ifndef VE_EDITOR
		FILE* dataFile = NULL;
		int64_t dataFileHeadersDescSize = 0;
		int64_t sizeOfSingleHeader = 0;
		int64_t numberOfAssets = 0;

		struct AssetData 
		{
			uint8_t* data;
			int64_t size;
		};

		AssetData GetAssetData(std::string relativePath);

#endif
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
		std::mutex fontsMutex;

		std::unordered_map<std::string, Model> models;
		std::mutex modelsMutex;



		std::unordered_map<std::string, std::string> scenes;
		std::mutex scenesMutex;
		std::unordered_map<std::string, std::string> constructs;
		std::mutex constructsMutex;
		std::unordered_map<std::string, std::string> projects;
		std::mutex projectsMutex;
		friend class SceneManager;
		friend class Engine;
	};
}