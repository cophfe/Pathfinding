#pragma once

#include "NecessaryHeaders.h"


#include "Scene.h"
#include "TextureManager.h"

struct GameProperties
{
	const char* windowName = "Game";
	int windowWidth = 800;
	int windowHeight = 600;
	int targetFPS = 0;
	bool enableAntiAliasing = true;
};

class Game
{
public:
	void init(GameProperties* properties = nullptr);
	void gameLoop();
	void shutdown();
private:
	float deltaTime;

	char currentScene = 0;
	
	std::vector<Scene*> scenes;

	TextureManager textureManager;
};

