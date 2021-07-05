#pragma once
#include "NecessaryHeaders.h"


class TextureManager
{
public:
	
	bool LoadTextures(std::string folder);
	bool LoadTexture(std::string texture);

	Texture2D* GetTexture(std::string textureName);
	Texture2D* GetTextureArray(std::string textureName);
private:
	std::vector<Texture2D*> globalTextures;

};

