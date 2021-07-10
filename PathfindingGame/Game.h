#pragma once

#include "NecessaryHeaders.h"
#include "TextureManager.h"

class Scene;
class GameObject;
class TransformObject;


struct GameProperties
{
	std::string windowName = "Game";
	std::string spriteLocation = "C:\\Users\\cphfe\\Desktop\\AIE\\AIForGames\\Pathfinding\\Images";
	int windowWidth = 800;
	int windowHeight = 600;
	int targetFPS = 0;
	bool enableAntiAliasing = true;
};

class Game
{
public:
	void init(GameProperties* properties);
	void gameLoop();
	void shutdown();

	static float getDeltaTime();
	TextureManager* getTextureManager();

	void changeScene(int sceneIndex);

	static Game& getInstance() { static Game instance; return instance; }

	Game() {};
	Game(Game const&) = delete;
	void operator=(Game const&) = delete;
private:
	static Game* instance;

	float deltaTime;

	char currentScene;
	
	std::vector<Scene*> scenes;

	TextureManager* textureManager;
};

