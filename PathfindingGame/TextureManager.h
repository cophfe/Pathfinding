#pragma once
#include "NecessaryHeaders.h"
#include "Sprite.h"
#include <filesystem>
#include <algorithm>
#include <map>

class GameObject;

class TextureManager
{
public:
	TextureManager();
	//Copy and Move constructors and assigners (deleted)
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//copy
	TextureManager(const TextureManager&) = delete;
	TextureManager& operator=(const TextureManager&) = delete;
	//move
	TextureManager(TextureManager&&) = delete;
	TextureManager& operator=(TextureManager&&) = delete;
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	~TextureManager();
	
	void LoadTexturesFromFolder(std::string folder);

	Sprite* GenSprite(std::string name, GameObject* gameObject);

	void UnloadTextures();

private:
	std::map<std::string, TextureComplex> textureMap;
	
};

