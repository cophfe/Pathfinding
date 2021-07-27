#include "Game.h"

#include "Scene.h"
#include "GameObject.h"
#include "Transform.h"

#include "AgentComponent.h"
#include "AgentDataComponent.h"
#include "PlayerComponent.h"

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
	roomManager->generateMap();
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// 	   Create Scene
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	currentScene = 0;
	scenes.push_back(new Scene());
	SceneProperties scene1Properties;
	scene1Properties.backgroundTiling = "background";
	scene1Properties.backgroundScale = 1.436781609195402f;
	scenes[currentScene]->load(scene1Properties, currentScene);
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// 	   Initialize Player
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	auto target = new GameObject(scenes[currentScene], "beearBody");
	PlayerComponent* pC = target->addComponent<PlayerComponent>();
	pC->init(scenes[currentScene]);
	
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// 	   Initialize Actors
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	///actor blackboard is made
	AgentDataComponent* blackboard = (new GameObject(scenes[currentScene], nullptr, false, { 0 }, 0, 1, SORTING::BACKGROUND))->addComponent<AgentDataComponent>();
	blackboard->init(target, scenes[currentScene]->getPathfinder());
	//then actors
	GameObject* actor = new GameObject(scenes[currentScene], "beeBody", true, { 300,400 }, 0, 1.0f);
	actor->addComponent<AgentComponent>()->init(blackboard, scenes[currentScene]->getCollisionManager());
	
	scenes[0]->start();
}

void Game::gameLoop()
{
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
		std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();

        // update Scene
        scenes[currentScene]->update();
        
        // draw Scene
        scenes[currentScene]->draw();

		std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();
		deltaTime = (float)std::chrono::duration_cast<std::chrono::duration<double>>(end - start).count();
    }
}

void Game::shutdown()
{
    CloseWindow();

	for (size_t i = 0; i < scenes.size(); i++)
	{
		delete scenes[i];
	}

	delete roomManager;
	delete textureManager;
}

float Game::getDeltaTime()
{
	return getInstance().deltaTime;
}

TextureManager* Game::getTextureManager()
{
	return textureManager;
}

void Game::changeScene(int sceneIndex)
{
	//move gameobjects from one scene to other IF needed (done before this function is called)
	
	//unload previous scene
	
	//load in new scene info 
	//scenes[currentScene]->start();
	//success?
}

void Game::GenerateScene(int sceneIndex)
{
	
}

Game& Game::getInstance()
{
	static Game instance = Game(); 
	return instance;
}
