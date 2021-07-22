#include "TextureManager.h"
#include <iostream>

static bool checkExtension(const std::filesystem::path& path)
{	
	return (std::filesystem::is_regular_file(path) && (path.extension().string() == std::string(".png") || path.extension().string() == std::string(".gif")));
}

static bool checkIsSpriteSheet(std::filesystem::path& jsonDirectory, std::filesystem::path& )
{

}
TextureManager::TextureManager()
{
}

TextureManager::~TextureManager()
{
	UnloadTextures();
}

void TextureManager::LoadTexturesFromFolder(std::string folder)
{
	//This whole thing isn't very performant but it only happens once at the start so whatever
	//should textures be loaded in and out depending on when they're needed? HAHA no of course not, just load them all in at once thats completely reasonable

	//check the folder for texture to add to the array
	//texture will be saved into AnimatedTexture'es

	//sprite sheets will be generated with json arrays through adobe animate
	std::filesystem::path jsonDirectory;

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
				jsonDirectory = entry.path();
			}
			//if it's not a subdirectory we check if it has the correct extensions
			//if so it will be loaded onto the gpu
			else if (checkExtension(path))
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
				textureMap.insert(std::pair<std::string, CursedTextureWrapper>(path.stem().string(), { tex, false }));
			}

		}
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
			
			/*std::ifstream stream = std::ifstream(entry);*/

			std::ifstream ifs(entry.path());
			json document = json::parse(ifs);

			//get all necessary info
			//this will break if the json file isn't formatted correctly
			auto& frames = document["frames"];
				
			animatedTexture->textureNumber = frames.size();
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

Sprite* TextureManager::GenSprite(std::string name, GameObject* gameObject)
{
	auto iterator = textureMap.find(name);
	if (iterator != textureMap.end())
	{
		CursedTextureWrapper& textureWrapper = (*iterator).second;
		if (textureWrapper.isAnimated)
			return new AnimatedSprite((AnimatedTexture*)textureWrapper.texture, gameObject);
		else
			return new Sprite(textureWrapper.texture, gameObject);
	}
	else
	{
		return new Sprite(textureMap["missing"].texture, gameObject);
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

void TextureManager::UnloadTextures()
{
	for (auto& textureComplex : textureMap)
	{
		UnloadTexture(*textureComplex.second.texture);

		if (textureComplex.second.isAnimated)
		{
			delete[]((AnimatedTexture*)textureComplex.second.texture)->coordinates;
		}
		delete textureComplex.second.texture;
	}
}
