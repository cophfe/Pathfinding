#pragma once
#include "NecessaryHeaders.h"
#include "Sprite.h"
#include <filesystem>
#include <algorithm>
#include <map>

#include <nlohmann/json.hpp>
#include <fstream>
#include <typeinfo>
using namespace nlohmann;

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
	
	void loadTexturesFromFolder(std::string folder);
	void loadShadersFromFolder(std::string folder);

	Sprite* genSprite(std::string name, GameObject* gameObject);
	Texture2D* getTextureInfo(std::string name);

	Shader* getShader(std::string name);
	
	void unload();

private:

	//used to tell if is animated or not
	struct CursedTextureWrapper
	{
		Texture2D* texture;
		bool isAnimated;
	};
	
	//holds the missing texture
	RenderTexture2D missingTexture;
	//holds all textures (except for render textures)
	std::map<std::string, CursedTextureWrapper> textureMap;
	//holds all shaders
	std::map<std::string, Shader> shaderMap;
};

