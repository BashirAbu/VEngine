#include "ve_assets_manager.h"
#include "ve_engine.h"
#include "ve_font.h"
#include <rlgl.h>
#include "spirv_glsl.hpp"

#ifndef VE_EDITOR
#include "ve_assets_packager.h"
#endif



extern "C"
{
	void CAssetsManager(const char* filepath, unsigned char** outData, int* outSize, char* fileExtension)
	{
#ifndef VE_EDITOR
		VE::AssetsManager::AssetData data = VE::AssetsManager::GetSingleton()->GetAssetData(filepath);

		*outData = data.data;
		*outSize = data.size;
		std::filesystem::path path = filepath;
		strcpy(fileExtension, path.extension().string().c_str());
#endif
	}
}


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
			if (strstr(header.name, relativePath.c_str()))
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
			free(data.data);
			free(img.data);
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
			//load image from memory copys the buffer.
			free(data.data);
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
			free(data.data);
#endif

			return &sounds[filepath.string()];
		}
	}
	Font* AssetsManager::LoadFont(std::filesystem::path filepath, int32_t fontSize)
	{
		std::lock_guard<std::mutex> lock(fontsMutex);
#ifdef VE_EDITOR 
		std::filesystem::path fullpath = assetsFolderPath.generic_string() + filepath.generic_string();
		FILE* fontFile = fopen(fullpath.generic_string().c_str(), "rb");
		fseek(fontFile, 0, SEEK_END);
		size_t fileSize = ftell(fontFile);
		rewind(fontFile); 
		uint8_t* buffer = (uint8_t*)malloc(fileSize);
		VE_ASSERT(fread(buffer, fileSize, 1, fontFile));
		fclose(fontFile);
		return new Font(buffer, fileSize, fontSize);
#else
		AssetData data = GetAssetData(filepath.generic_string());
		VE_ASSERT(data.size);
		return new Font(data.data, data.size, fontSize);
#endif
	}
	Model* AssetsManager::LoadModel(std::filesystem::path filepath)
	{
		std::lock_guard<std::mutex> lock(modelsMutex);

		if (models.find(filepath.string()) != models.end())
		{
			return &models[filepath.string()];
		}
		else
		{
#ifdef VE_EDITOR 
			std::filesystem::path fullpath = assetsFolderPath.generic_string() + filepath.generic_string();
			models[filepath.string()] = ::LoadModel(fullpath.string().c_str());	
#else
			std::string fileName = filepath.filename().string();
			AssetData data = GetAssetData(filepath.generic_string());
			VE_ASSERT(data.size);
			models[filepath.string()] = LoadModelFromMemory(data.data, data.size, fileName.c_str());
			//free(data.data);
#endif

			return &models[filepath.string()];
		}
	}
	Shader* AssetsManager::LoadShader(std::filesystem::path filepath)
	{
		std::lock_guard<std::mutex> lock(shadersMutex);

		if (shaders.find(filepath.string()) != shaders.end())
		{
			return &shaders[filepath.string()];
		}
		else
		{
#ifdef VE_EDITOR
			std::filesystem::path fullpath = assetsFolderPath.generic_string() + filepath.generic_string();
			FILE* shaderFile = fopen(fullpath.generic_string().c_str(), "r");
			VE_ASSERT(shaderFile);
			fseek(shaderFile, 0, SEEK_END);
			size_t fileSize = ftell(shaderFile);
			rewind(shaderFile);
			std::string buffer(fileSize, '\0');
			fread(buffer.data(), fileSize, 1, shaderFile);
			fclose(shaderFile);

			std::string fragmentShader = "", vertexShader = "";

			std::stringstream stream(buffer);
			std::string line;
			std::string type = "non";
			while (std::getline(stream, line))
			{
				if (line.find("#shader") != std::string::npos) 
				{ // Check for #shader directive
					if (line.find("vertex") != std::string::npos) 
					{
						type = "vertex";
					}
					else if (line.find("fragment") != std::string::npos) 
					{
						type = "fragment";
					}
				}
				else 
				{
					if (type != "non")
					{
						if (type == "fragment")
						{
							fragmentShader += line + "\n";
						}
						else if (type == "vertex")
						{
							vertexShader += line + "\n";
						}
					}
				}
			}
			switch (rlGetVersion())
			{
			case RL_OPENGL_11: {}break;
			case RL_OPENGL_21: {}break;
			case RL_OPENGL_33: {}break;
			case RL_OPENGL_43: {}break;
			case RL_OPENGL_ES_20: {}break;
			case RL_OPENGL_ES_30: {}break;
			}
			std::filesystem::path generatedFragmentFilePath = fullpath.parent_path().generic_string() + "/__" + fullpath.stem().string() + ".frag";
			std::filesystem::path generatedVertexFilePath = fullpath.parent_path().generic_string() + "/__" + fullpath.stem().string() + ".vert";
			FILE* fragmentShaderFile = fopen(generatedFragmentFilePath.generic_string().c_str(), "w");
			FILE* vertexShaderFile = fopen(generatedVertexFilePath.generic_string().c_str(), "w");

			fwrite(fragmentShader.c_str(), fragmentShader.size() + 1, 1, fragmentShaderFile);
			fwrite(vertexShader.c_str(), vertexShader.size() + 1, 1, vertexShaderFile);

			fclose(fragmentShaderFile);
			fclose(vertexShaderFile);

			std::filesystem::path fragmentSpirvPath = generatedFragmentFilePath.parent_path().string() + "/" + fullpath.stem().string() + "_frag.spv";
			std::string command = "%VENGINE_DIR%\\glslangValidator\\glslangValidator.exe -G --aml " + generatedFragmentFilePath.generic_string() +
				" -o " + fragmentSpirvPath.generic_string();
			system(command.c_str());
			
			std::filesystem::path vertexSpirvPath = generatedVertexFilePath.parent_path().string() + "/" + fullpath.stem().string() + "_vert.spv";
			command = "%VENGINE_DIR%\\glslangValidator\\glslangValidator.exe -G --aml " + generatedVertexFilePath.generic_string() +
				" -o " + vertexSpirvPath.generic_string();
			system(command.c_str());

			std::remove(generatedFragmentFilePath.generic_string().c_str());
			std::remove(generatedVertexFilePath.generic_string().c_str());

			FILE* fragmentSpirvFile = fopen(fragmentSpirvPath.generic_string().c_str(), "rb");
			fseek(fragmentSpirvFile, 0, SEEK_END);
			size_t fragmentSpirvFileSize = ftell(fragmentSpirvFile);

			rewind(fragmentSpirvFile);

			std::vector<uint32_t> fragmentSpirvBinary(fragmentSpirvFileSize / sizeof(uint32_t));

			fread(fragmentSpirvBinary.data(), fragmentSpirvFileSize, 1, fragmentSpirvFile);
			fclose(fragmentSpirvFile);

			spirv_cross::CompilerGLSL glsl(std::move(fragmentSpirvBinary));

			spirv_cross::CompilerGLSL::Options options;
			options.version = 330;
			options.es = false;
			glsl.set_common_options(options);

			// Compile to GLSL, ready to give to GL driver.
			fragmentShader  = glsl.compile();
			std::ofstream frg(vertexSpirvPath.parent_path() / "gene.txt", std::ios::out);
			frg << fragmentShader;
			frg.close();

			shaders[filepath.string()] = ::LoadShaderFromMemory(vertexShader.empty()? NULL : vertexShader.c_str(), fragmentShader.empty() ? NULL : fragmentShader.c_str());
#else
#endif

			return &shaders[filepath.string()];
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

		for (auto model : models)
		{
			UnloadModel(model.second);
		}
		models.clear();



		scenes.clear();
		constructs.clear();
		projects.clear();
	}
}