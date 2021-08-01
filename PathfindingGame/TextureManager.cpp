#include "TextureManager.h"
#include <iostream>

static bool checkExtensionTexture(const std::filesystem::path& path)
{	
	//check if extension is valid for a texture (also if it is a regular file)
	return (std::filesystem::is_regular_file(path) && (path.extension().string() == std::string(".png") || path.extension().string() == std::string(".gif")));
}

static bool checkExtensionShader(const std::filesystem::path& path)
{
	//check if is a regular file (not a folder)
	return (std::filesystem::is_regular_file(path));
}


TextureManager::TextureManager()
{
	//generate missing texture
	missingTexture = LoadRenderTexture(200, 200);
	BeginTextureMode(missingTexture);
	ClearBackground(BLACK);
	DrawRectangle(0, 0, 100, 100, PURPLE);
	DrawRectangle(100, 100, 100, 100, PURPLE);
	EndTextureMode();

	//convert missing texture into an animated texture (so that any animated texture - specific funtions will not crash the game)
	AnimatedTexture* animatedTexture = new AnimatedTexture();
	animatedTexture->coordinates = new Vector2i[1];
	animatedTexture->textureNumber = 1;
	animatedTexture->spriteWidth = 200;
	animatedTexture->spriteHeight = 200;

	memcpy(animatedTexture, &missingTexture.texture, sizeof(Texture2D));
	SetTextureFilter(*animatedTexture, FILTER_BILINEAR);
	textureMap["missing"] = CursedTextureWrapper{ animatedTexture, true };
}

TextureManager::~TextureManager()
{
	unload();
}

void TextureManager::loadTexturesFromFolder(std::string folder)
{
	//check if folder is valid
	if (!std::filesystem::exists(folder) || !std::filesystem::is_directory(folder))
	{
		return;
	}

	//This whole thing isn't very performant but it only happens once at the start so whatever
	//check the folder for texture to add to the array
	//texture will be saved into the textureMap

	//sprite sheets are generated with json arrays through adobe animate
	//any textures with a corrosponding json array are considered to be sprite sheets
	std::filesystem::path jsonDirectory;
	//used to confirm if it has a json folder at all or not
	bool hasJson = false;

	for (auto& entry : std::filesystem::directory_iterator(folder))
	{
		//sometimes I like to keep up the charade that this has error checking
		if (entry.exists())
		{
			auto& path = entry.path();

			//we try to find the json subdirectory
			//it holds all the information about the spritesheets
			if (std::filesystem::is_directory(entry.status()) && entry.path().stem().string() == std::string("_json"))
			{
				//if found, set the directory path
				jsonDirectory = entry.path();
				hasJson = true;
			}

			//if it's not a subdirectory we check if it has the correct extensions
			//if so it will be loaded onto the gpu
			else if (checkExtensionTexture(path))
			{
				Texture2D* tex = new Texture2D();
				*tex = LoadTexture(path.string().c_str());
				SetTextureFilter(*tex, FILTER_BILINEAR);
				//if it failed to load do not add it to the map
				if (tex->id == 0)
				{
					delete tex;
					continue;
				}
				//texture name is the stem of the path (for "C:/users/texture.png" the name will be "texture")
				textureMap.insert(std::pair<std::string, CursedTextureWrapper>(path.stem().string(), { tex, false }));
			}
		}
	}

	if (!hasJson)
	{
		return;
	}
	//after loading all we check which one's names match with files in the json subdirectory
	for (auto& entry : std::filesystem::directory_iterator(jsonDirectory))
	{
		//first make sure the entry is a json file
		if (entry.path().extension().string() != std::string(".json"))
			continue;

		//check if any json files match with any image files
		auto iterator = textureMap.find(entry.path().stem().string());
		if (iterator != textureMap.end())
		{
			//if so, we assume that the image file is a spritesheet and turn it into an AnimatedTexture
			Texture2D* oldTexture = (*iterator).second.texture;
			AnimatedTexture* animatedTexture = new AnimatedTexture();
			memcpy(animatedTexture, oldTexture, sizeof(Texture2D));
			SetTextureFilter(*animatedTexture, FILTER_BILINEAR);
			(*iterator).second.texture = animatedTexture; //copy data into the new animated textures
			(*iterator).second.isAnimated = true;
			//goodbye old pal
			delete oldTexture;

			std::ifstream ifs(entry.path());
			json document = json::parse(ifs);

			//get all necessary info
			//this will break if the json file isn't formatted correctly
			auto& frames = document["frames"];
				
			animatedTexture->textureNumber = (int)frames.size();
			animatedTexture->coordinates = new Vector2i[animatedTexture->textureNumber];
			animatedTexture->spriteWidth = frames[0]["frame"]["w"];
			animatedTexture->spriteHeight = frames[0]["frame"]["h"];
			int i = 0;
			for (auto& spriteInfo : frames)
			{
				animatedTexture->coordinates[i].x = spriteInfo["frame"]["x"];
				animatedTexture->coordinates[i].y = spriteInfo["frame"]["y"];
				i++;
			}

			ifs.close();
		}
	}
}

void TextureManager::loadShadersFromFolder(std::string folder)
{
	// return if invalid directory
	if (!std::filesystem::exists(folder) || !std::filesystem::is_directory(folder))
		return;

	//load fragment shaders
	for (auto& entry : std::filesystem::directory_iterator(folder))
	{
		if (checkExtensionShader(entry.path()))
		{
			Shader s = LoadShader(0, entry.path().string().c_str());

			//idk how to check if it failed to load so let's just cross our fingers and hope it's the same as textures
			if (s.id == 0)
				continue;

			shaderMap[entry.path().stem().string()] = s;
		}
	}
}

Sprite* TextureManager::genSprite(std::string name, GameObject* gameObject)
{
	//check if name corrosponds to texture
	auto iterator = textureMap.find(name);
	if (iterator != textureMap.end())
	{
		//if so, generate a sprite from that texture and return it
		CursedTextureWrapper& textureWrapper = (*iterator).second;
		if (textureWrapper.isAnimated)
			return new AnimatedSprite((AnimatedTexture*)textureWrapper.texture, gameObject);
		else
			return new Sprite(textureWrapper.texture, gameObject);
	}
	else
	{
		//if not, generate a sprite from the missing texture
		return new AnimatedSprite((AnimatedTexture*)textureMap["missing"].texture, gameObject);
	}
	
}

Texture2D* TextureManager::getTextureInfo(std::string name)
{
	auto iterator = textureMap.find(name);
	if (iterator != textureMap.end())
		return (*iterator).second.texture;
	else
		return textureMap["missing"].texture;

}

Shader* TextureManager::getShader(std::string name)
{
	auto iterator = shaderMap.find(name);
	if (iterator != shaderMap.end())
	{
		return &((*iterator).second);
	}
	return nullptr;
}

void TextureManager::unload()
{
	//called in destructor

	//since the missing texture is a special case it is deleted seperately
	UnloadRenderTexture(missingTexture);
	delete[] ((AnimatedTexture*)textureMap["missing"].texture)->coordinates;
	delete textureMap["missing"].texture;
	textureMap.erase("missing");
	
	//then all other textures are deleted
	for (auto& textureComplex : textureMap)
	{
		UnloadTexture(*textureComplex.second.texture);

		if (textureComplex.second.isAnimated)
		{
			delete[] ((AnimatedTexture*)textureComplex.second.texture)->coordinates;
		}
		delete textureComplex.second.texture;
	}

	//shaders are also unloaded
	for (auto& shader : shaderMap)
	{
		UnloadShader(shader.second);
	}
}
