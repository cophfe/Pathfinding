#include "Game.h"

#include "Scene.h"
#include "GameObject.h"
#include "Transform.h"

void Game::init(GameProperties* properties )
{

	//Tell raylib to use antialiasing
	if (properties->enableAntiAliasing)
		SetConfigFlags(FLAG_MSAA_4X_HINT);
    
    //initialize window with initial properties
    InitWindow(properties->windowWidth, properties->windowHeight, properties->windowName.c_str());
    
	if (properties->targetFPS != 0)
		SetTargetFPS(properties->targetFPS);

	//load textures in from the target folder
	textureManager = new TextureManager();
	textureManager->LoadTexturesFromFolder(properties->spriteLocation);
	/*
		scene creation
		the plan is to create scenes from json files or something, sorta like a midpoint between a level editor and typing out scenes in code 
		for right now it will just create one scene with a player object in it
	*/
	scenes.push_back(new Scene());
	scenes[0]->load();
	auto background = new GameObject("player", scenes[0], true, { 0 }, { 0 }, 5.0f);
	background->getSprite().setTint({ 0xFF,0,0,0xCF });
	auto player = new GameObject("player", scenes[0]);

	PlayerComponent* pC = (PlayerComponent*)player->addComponent<PlayerComponent>();
	pC->init(200, 3, player);
	scenes[0]->getCamera()->Target(player->getTransform());

	new GameObject("bee", player, true, Vector2{ 500, 0 }, 0.0f, 1.0f);
	new GameObject("death", scenes[0], true, Vector2{ 1000, 0 }, 0.0f, 1.0f);
	new GameObject("harold", scenes[0], true, Vector2{ 1500, 0 }, 0.0f, 1.0f);
	new GameObject("happy", scenes[0], true, Vector2{ 2000, 0 }, 0.0f, 1.0f);
	new GameObject("anime", scenes[0], true, Vector2{ 2500, 0 }, 0.0f, 1.0f);
}

void Game::gameLoop()
{
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
		std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();

        // update Scene
        scenes[currentScene]->update();
        
        // Draw Scene
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
	//success?
}

Game& Game::getInstance()
{
	static Game instance = Game(); 
	return instance;
}
