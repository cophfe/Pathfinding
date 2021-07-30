#include "Game.h"

#include "Scene.h"
#include "Room.h"
#include "GameObject.h"
#include "Transform.h"

#include "AgentComponent.h"
#include "AgentDataComponent.h"
#include "PlayerComponent.h"
#include "PathfinderComponent.h"

void Game::init(GameProperties* properties )
{
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// 		Initialize Game
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	if (properties->enableAntiAliasing)
		SetConfigFlags(FLAG_MSAA_4X_HINT);
    
    //initialize window with initial properties
    InitWindow(properties->windowWidth, properties->windowHeight, properties->windowName.c_str());
    
	if (properties->targetFPS != 0)
		SetTargetFPS(properties->targetFPS);

	timeAtStart = time(0);
	timeScale = 1;
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// 	   draw Loading Screen
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// since loading the textures can take a while first a loading screen is rendered
	BeginDrawing();
	ClearBackground(BLACK);
	DrawText("Loading...", properties->windowWidth/2 - 170, properties->windowHeight / 2 - 40, 80, RAYWHITE);
	EndDrawing();
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// 	   Initialize TextureManager
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	textureManager = new TextureManager();
	textureManager->loadTexturesFromFolder(properties->spriteLocation);
	textureManager->loadShadersFromFolder(properties->shaderLocation);
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//	   Generate Rooms
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	roomManager = new RoomManager(time(0));
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// 	   Create Room
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	Room* firstRoom = roomManager->createFirstRoom();	
	scene = firstRoom;
	
	scene->start();
}

void Game::gameLoop()
{
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
		std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();

        // update Scene
		scene->update();
	
		// do fixedUpdate (only happens every PHYSICS_TIME_STEP)
		scene->fixedUpdate();

        
        // draw Scene
		BeginDrawing();
		scene->draw();
		EndDrawing();

		std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();
		deltaTime = (float)std::chrono::duration_cast<std::chrono::duration<double>>(end - start).count();

		//this happens independant of deltaTime
		//have to wait until end of gameLoop to delete scene
		if (nextScene != nullptr)
		{
			scene->beforeDelete(nextScene);
			delete scene;
			scene = nextScene;
			scene->start();
			nextScene = nullptr;
		}
    }
}

void Game::shutdown()
{
    CloseWindow();

	delete scene;

	delete roomManager;
	delete textureManager;
}

void Game::switchScene(Scene* newScene)
{
	nextScene = newScene;
}

float Game::getDeltaTime()
{
	return getInstance().deltaTime * getInstance().timeScale;
}

float Game::getUnscaledDeltaTime()
{
	return getInstance().deltaTime;
}

TextureManager* Game::getTextureManager()
{
	return textureManager;
}

Game& Game::getInstance()
{
	static Game instance = Game(); 
	return instance;
}
