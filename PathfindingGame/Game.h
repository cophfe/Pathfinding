#pragma once
#include "NecessaryHeaders.h"
#include "TextureManager.h"
#include "RigidBodyComponent.h"
#include "RoomManager.h"
#include <chrono>

class PlayerComponent;
class Scene;
class GameObject;
class TransformObject;


struct GameProperties
{
	std::string windowName = "Game";
	std::string spriteLocation;
	std::string shaderLocation;
	int windowWidth = 1000;
	int windowHeight = 750;
	int targetFPS = 0;
	bool enableAntiAliasing = true;
};

class Game
{
public:

	void init(GameProperties* properties);
	void gameLoop();
	void shutdown();
	void switchScene(Scene* newScene);
	static float getDeltaTime();
	static float getUnscaledDeltaTime();
	TextureManager* getTextureManager();
	inline RoomManager* getRoomManager() { return roomManager; };
	inline Scene* getScene() { return scene; }
	inline void setTimeScale(float scale) { timeScale = scale; }
	inline float getTimeScale() { return timeScale; }
	static Game& getInstance();
	void endGameLoop() { shouldClose = true; }

	Scene* createMenuScene();
	Scene* createDeathScene(int floorCount);
	
	Game(Game const&) = delete;
	void operator=(Game const&) = delete;
private:

	Game() { memset(this, 0, sizeof(Game)); };

	static Game* instance;

	float deltaTime;
	float timeScale;
	inline bool checkWindowShouldClose() { return WindowShouldClose() || shouldClose; }
	Scene* scene;
	Scene* nextScene = nullptr;
	//std::map<int, SceneData*> savedData;
	int timeAtStart;
	bool shouldClose = false;

	TextureManager* textureManager = nullptr;
	RoomManager* roomManager = nullptr;
};

