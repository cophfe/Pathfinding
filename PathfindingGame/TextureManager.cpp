#include "TextureManager.h"
#include <iostream>

static bool checkExtension(const std::filesystem::path& path)
{	
	return (std::filesystem::is_regular_file(path) && (path.extension().string() == std::string(".png")));
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
	//jesus christ this is an abomination
	//check the folder for textures to add to the array
	//textures will be saved into TextureComplex'es, which have a name given to them
	for (auto& entry : std::filesystem::directory_iterator(folder))
	{
		if (entry.exists())
		{
			auto& path = entry.path();
			//subdirectories are put into their own textureComplex (so they can be converted into animated sprites)
			if (std::filesystem::is_directory(entry.status()))
			{
				int count = std::count_if(std::filesystem::directory_iterator(path), std::filesystem::directory_iterator{}, checkExtension);
				if (count > 0)
				{
					//animated sprite name is based on the 
					TextureComplex animatedTexture = TextureComplex();
					animatedTexture.textures = new Texture2D[count];
					animatedTexture.textureNumber = count;
					int currentCount = 0;
					for (auto& subEntry : std::filesystem::directory_iterator(folder))
					{
						auto& sPath = subEntry.path();
						if (checkExtension(sPath))
						{
							//each texture is loaded in here
							if (currentCount >= count)
								break;
							*(animatedTexture.textures) = LoadTexture(sPath.string().c_str());
#pragma warning(suppress:6385) // this warning sucks and is always wrong
							if (animatedTexture.textures[count].id == 0)
								continue; //error causer here, probably
							++currentCount;
						}
					}
					textureMap.insert(std::pair<std::string, TextureComplex >(path.stem().string(), animatedTexture));
				}
				
			}
			if (checkExtension(path))
			{
				TextureComplex tC = TextureComplex();
				tC.textures = new Texture2D();
				*(tC.textures) = LoadTexture(path.string().c_str());
				//if it failed to load do not add it to the map
				if (tC.textures->id == 0) 
				{
					delete tC.textures;
					continue;
				}
				tC.textureNumber = 1;
				textureMap.insert(std::pair<std::string, TextureComplex>(path.stem().string(), tC));
			}
			
		}
	}
}

Sprite TextureManager::GenSprite(std::string name, GameObject* gameObject)
{
	auto& textureComplex = textureMap[name];
	if (textureComplex.textureNumber == 1)
		return Sprite(&textureComplex, gameObject);
	else
		return AnimatedSprite(&textureComplex, gameObject);
}

void TextureManager::UnloadTextures()
{
	for (auto& textureComplex : textureMap)
	{
		for (int i = 0; i < textureComplex.second.textureNumber; i++)
		{
			UnloadTexture(textureComplex.second.textures[i]);
		}
		if (textureComplex.second.textureNumber == 1)
			delete textureComplex.second.textures;
		else
			delete[] textureComplex.second.textures;
	}
}
