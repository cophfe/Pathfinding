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
	//scenes.reserve(sizeof(Scene) * amountOfScenes); 
	scenes.push_back(new Scene());
	scenes[0]->load();
	auto background = scenes[0]->addGameObject(new GameObject("player"));
	background->getSprite().setTint({ 0xFF,0,0,0xCF });
	background->getTransform()->setScale(5);
	auto player = scenes[0]->addGameObject(new GameObject("player"));
	auto bee = new GameObject("bee", player->getTransform(), true, Vector2{ 100, 300 }, 0.0f, 1.0f);
	PlayerComponent* pC = (PlayerComponent*)player->addComponent<PlayerComponent>();
	pC->init(200, 3, player);
	scenes[0]->getCamera()->Target(player->getTransform());

	InitPhysics();
}

void Game::gameLoop()
{
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
		std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
		//update physics
		UpdatePhysics();

        // update Scene
        scenes[currentScene]->update();
        
        // Draw Scene
        scenes[currentScene]->draw();
		std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();
		deltaTime = std::chrono::duration_cast<std::chrono::duration<double>>(end - start).count();
    }
}

void Game::shutdown()
{
	ClosePhysics();

    CloseWindow();

	for (size_t i = 0; i < scenes.size(); i++)
	{
		delete scenes[i];
	}
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
