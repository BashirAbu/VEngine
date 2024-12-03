#include "ve_assets_manager.h"
#include "ve_engine.h"

namespace VE 
{
	AssetsManager::AssetsManager() 
	{
		assetsFolderPath = Engine::GetSingleton()->GetDesc()->projectDetails.assetsPath;
	}
	AssetsManager::~AssetsManager()
	{
		Clear();
	}
	AssetsManager* AssetsManager::GetSingleton()
	{
		static AssetsManager* assetsManager = new AssetsManager();
		return assetsManager;
	}
	Texture* AssetsManager::LoadTexture(std::filesystem::path filepath)
	{
		std::lock_guard<std::mutex> lock(texturesMutex);
		filepath = filepath.generic_string();
		std::filesystem::path fullpath = assetsFolderPath.generic_string() + filepath.generic_string();
		if (textures.find(filepath.string()) != textures.end())
		{
			return &textures[filepath.string()];
		}
		else 
		{
			textures[filepath.string()] = ::LoadTexture(fullpath.string().c_str());
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
		std::filesystem::path fullpath = assetsFolderPath.generic_string() + filepath.generic_string();

		if (images.find(filepath.string()) != images.end())
		{
			return &images[filepath.string()];
		}
		else
		{
			images[filepath.string()] = ::LoadImage(fullpath.string().c_str());
			return &images[filepath.string()];
		}
	}
	Sound* AssetsManager::LoadSound(std::filesystem::path filepath)
	{
		std::lock_guard<std::mutex> lock(soundsMutex);
		std::filesystem::path fullpath = assetsFolderPath.generic_string() + filepath.generic_string();

		if (sounds.find(filepath.string()) != sounds.end())
		{
			return &sounds[filepath.string()];
		}
		else
		{
			sounds[filepath.string()] = ::LoadSound(fullpath.string().c_str());
			return &sounds[filepath.string()];
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
	}
}