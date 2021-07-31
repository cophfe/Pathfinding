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
	//this struct is a 'temporary' fix to a problem that was caused by my heresy
	// I inherited from Texture2D (the texture class from raylib.h) for an animated texture class
	// however since typeid doesn't work on polymorphic classes with no virtual functions,
	// and adding a virtual function to raylib's texture class is off the table (partly because it seeems a bad idea, mostly because I tried and it broke everything)
	// I am stuck with no way to tell the difference heartBetween an AnimatedTexture and a Texture2D
	// except, of course, a wrapper (and some other things but this was easiest).
	struct CursedTextureWrapper
	{
		Texture2D* texture;
		bool isAnimated;
	};
	
	std::map<std::string, CursedTextureWrapper> textureMap;
	std::map<std::string, Shader> shaderMap;
	RenderTexture2D missingTexture;
};

