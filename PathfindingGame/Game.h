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
	std::string windowName = "Adventures of Beear";
	std::string spriteLocation;
	std::string shaderLocation;
	int windowWidth = 1000;
	int windowHeight = 750;
	int targetFPS = 0;
	bool enableAntiAliasing = true;
};

//A singleton, holds all vital game data
class Game
{
public:
	void init(GameProperties* properties);
	void gameLoop();
	void shutdown();
	//sets the next scene, to be switched to at the end of the game loop
	void switchScene(Scene* newScene);
	
	//Getters
	static float getDeltaTime();
	static float getUnscaledDeltaTime();
	TextureManager* getTextureManager();
	inline RoomManager* getRoomManager() { return roomManager; };
	inline Scene* getScene() { return scene; }
	inline float getTimeScale() { return timeScale; }
	static Game& getInstance();
	//setters
	inline void setTimeScale(float scale) { timeScale = scale; }
	//stop game from doing game loop
	void endGameLoop() { shouldClose = true; }

	//create a menu scene
	Scene* createMenuScene();
	//create a death scene
	Scene* createDeathScene(int floorCount);
	
	Game(Game const&) = delete;
	void operator=(Game const&) = delete;

private:
	//box2D has continuous collision on dynamic v static so a high delta time isn't the biggest problem
	static constexpr float maxDeltaTime = 0.5f;
	static Game* instance;

	//constructor is private
	Game() { memset(this, 0, sizeof(Game)); };

	inline bool checkWindowShouldClose() { return WindowShouldClose() || shouldClose; }

	//Scene info
	Scene* scene;
	Scene* nextScene = nullptr;

	//Texture and Room data
	TextureManager* textureManager = nullptr;
	RoomManager* roomManager = nullptr;
	
	//Time info
	float deltaTime;
	float timeScale;
	int timeAtStart;

	bool shouldClose = false;
};

