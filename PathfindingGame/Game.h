#pragma once

#include "NecessaryHeaders.h"
#include "TextureManager.h"

#include "RigidBodyComponent.h"
#include "PlayerComponent.h"
#include <chrono>

class Scene;
class GameObject;
class TransformObject;


struct GameProperties
{
	std::string windowName = "Game";
	std::string spriteLocation;
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

	static Game& getInstance();

	Game(Game const&) = delete;
	void operator=(Game const&) = delete;
private:
#pragma warning(disable : 4244) //initialization warning can begone

	Game() { memset(this, 0, sizeof(Game)); };

	static Game* instance;

	float deltaTime;

	char currentScene;
	
	std::vector<Scene*> scenes;

	TextureManager* textureManager;
};

