#include "ve_assets_manager.h"
#include "ve_engine.h"
#include "ve_font.h"
#ifndef VE_EDITOR
#include "ve_assets_packager.h"
#endif
namespace VE 
{
	
	AssetsManager::AssetsManager()
	{
#ifndef VE_EDITOR
		dataFile = fopen("data.VEData", "rb");
		VE_ASSERT(dataFile);
		fread(&dataFileHeadersDescSize, sizeof(dataFileHeadersDescSize), 1, dataFile);
		sizeOfSingleHeader = sizeof(AssetHeader);
		numberOfAssets = dataFileHeadersDescSize / sizeOfSingleHeader;
#else
		assetsFolderPath = Engine::GetSingleton()->GetDesc()->projectDetails.assetsPath;

#endif
	}
	AssetsManager::~AssetsManager()
	{
#ifndef VE_EDITOR
		fclose(dataFile);
#endif
		Clear();
	}
	AssetsManager* AssetsManager::GetSingleton()
	{
		static AssetsManager* assetsManager = new AssetsManager();
		return assetsManager;
	}
#ifndef VE_EDITOR
	AssetsManager::AssetData AssetsManager::GetAssetData(std::string relativePath)
	{
		AssetData data = {};
		fseek(dataFile, sizeof(dataFileHeadersDescSize), SEEK_SET);
		for (size_t i = 0; i < (size_t)numberOfAssets; i++)
		{
			AssetHeader header = {};
			fread(&header, sizeof(header), 1, dataFile);
			if (strstr(relativePath.c_str(), header.name))
			{
				data.data = (uint8_t*) malloc(header.size + 1);
				data.data[header.size] = '\0';
				data.size = header.size;
				fseek(dataFile, (long)(dataFileHeadersDescSize + sizeof(dataFileHeadersDescSize)), SEEK_SET);
				fseek(dataFile, (long)header.offset, SEEK_CUR);
				fread(data.data, (long)data.size, 1, dataFile);
				break;
			}
		}
		return data;
	}
#endif

	Texture* AssetsManager::LoadTexture(std::filesystem::path filepath)
	{
		std::lock_guard<std::mutex> lock(texturesMutex);
		filepath = filepath.generic_string();
		if (textures.find(filepath.string()) != textures.end())
		{
			return &textures[filepath.string()];
		}
		else 
		{
#ifdef VE_EDITOR 
			std::filesystem::path fullpath = assetsFolderPath.generic_string() + filepath.generic_string();
			textures[filepath.string()] = ::LoadTexture(fullpath.string().c_str());
#else
			std::string filetype = filepath.extension().string();
			AssetData data = GetAssetData(filepath.generic_string());
			VE_ASSERT(data.size);
			Image img = ::LoadImageFromMemory(filetype.c_str(), data.data, (int)data.size);
			textures[filepath.string()] = ::LoadTextureFromImage(img);
			::UnloadImage(img);
#endif
			//This to avoid distortion while rotating or scaling.
			SetTextureFilter(textures[filepath.string()], TEXTURE_FILTER_BILINEAR);
			//This to avoid bleeding at the edges.
			SetTextureWrap(textures[filepath.string()], TEXTURE_WRAP_CLAMP);
			return &textures[filepath.string()];


		}
	}
	Image* AssetsManager::LoadImage(std::filesystem::path filepath)
	{
		std::lock_guard<std::mutex> lock(imagesMutex);
		

		if (images.find(filepath.string()) != images.end())
		{
			return &images[filepath.string()];
		}
		else
		{
#ifdef VE_EDITOR 
			std::filesystem::path fullpath = assetsFolderPath.generic_string() + filepath.generic_string();
			images[filepath.string()] = ::LoadImage(fullpath.string().c_str());
#else
			std::string filetype = filepath.extension().string();
			AssetData data = GetAssetData(filepath.generic_string());
			VE_ASSERT(data.size);
			images[filepath.string()] = ::LoadImageFromMemory(filetype.c_str(), data.data, (int)data.size);
#endif
			return &images[filepath.string()];
		}
	}
	Sound* AssetsManager::LoadSound(std::filesystem::path filepath)
	{
		std::lock_guard<std::mutex> lock(soundsMutex);

		if (sounds.find(filepath.string()) != sounds.end())
		{
			return &sounds[filepath.string()];
		}
		else
		{
#ifdef VE_EDITOR 
			std::filesystem::path fullpath = assetsFolderPath.generic_string() + filepath.generic_string();
			sounds[filepath.string()] = ::LoadSound(fullpath.string().c_str());
#else
			std::string filetype = filepath.extension().string();
			AssetData data = GetAssetData(filepath.generic_string());
			VE_ASSERT(data.size);
			Wave wave = ::LoadWaveFromMemory(filetype.c_str(), data.data, (int)data.size);
			sounds[filepath.string()] = ::LoadSoundFromWave(wave);
			::UnloadWave(wave);
#endif

			return &sounds[filepath.string()];
		}
	}
	Font* AssetsManager::LoadFont(std::filesystem::path filepath, int32_t fontSize)
	{
		std::lock_guard<std::mutex> lock(fontsMutex);

		if (fonts.find(filepath.string()) != fonts.end())
		{
			return fonts[filepath.string()];
		}
		else
		{

#ifdef VE_EDITOR 
			std::filesystem::path fullpath = assetsFolderPath.generic_string() + filepath.generic_string();
			FILE* fontFile = fopen(fullpath.generic_string().c_str(), "rb");
			fseek(fontFile, 0, SEEK_END);
			size_t fileSize = ftell(fontFile);
			rewind(fontFile); 
			uint8_t* buffer = (uint8_t*)malloc(fileSize);
			VE_ASSERT(fread(buffer, fileSize, 1, fontFile));
			fclose(fontFile);
			fonts[filepath.string()] = new Font(buffer, fileSize, fontSize);

#else
			AssetData data = GetAssetData(filepath.generic_string());
			VE_ASSERT(data.size);
			fonts[filepath.string()] = new Font(data.data, data.size, fontSize);
#endif

			return fonts[filepath.string()];
		}
	}
	const std::string& AssetsManager::LoadScene(std::filesystem::path filepath)
	{
		std::lock_guard<std::mutex> lock(scenesMutex);
		if (scenes.find(filepath.string()) != scenes.end())
		{
			return scenes[filepath.string()];
		}
		else
		{

#ifndef VE_EDITOR 
			AssetData data = GetAssetData(filepath.generic_string());
			VE_ASSERT(data.size);
			scenes[filepath.string()] = (const char*)data.data;
			free(data.data);
#endif
			return scenes[filepath.string()];
		}
	}
	const std::string& AssetsManager::LoadConstruct(std::filesystem::path filepath)
	{
		std::lock_guard<std::mutex> lock(constructsMutex);

		if (constructs.find(filepath.string()) != constructs.end())
		{
			return constructs[filepath.string()];
		}
		else
		{

#ifndef VE_EDITOR 

			AssetData data = GetAssetData(filepath.generic_string());
			VE_ASSERT(data.size);
			constructs[filepath.string()] = (const char*)data.data;
			free(data.data);
#endif

			return constructs[filepath.string()];
		}
	}
	const std::string& AssetsManager::LoadProject(std::filesystem::path filepath)
	{
		std::lock_guard<std::mutex> lock(projectsMutex);

		if (projects.find(filepath.string()) != projects.end())
		{
			return projects[filepath.string()];
		}
		else
		{

#ifndef VE_EDITOR 
			AssetData data = GetAssetData(filepath.generic_string());
			VE_ASSERT(data.size);
			projects[filepath.string()] = (const char*)data.data;
			free(data.data);
#endif
			return projects[filepath.string()];
		}
	}
	void AssetsManager::Clear()
	{
		for (auto img : images)
		{
			UnloadImage(img.second);
		}
		images.clear();
		for (auto tex : textures)
		{
			UnloadTexture(tex.second);
		}
		textures.clear();

		for (auto sound : sounds)
		{
			UnloadSound(sound.second);
		}
		sounds.clear();
		for (auto font : fonts)
		{
			delete font.second;
		}
		fonts.clear();


		scenes.clear();
		constructs.clear();
		projects.clear();
	}
}