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
	scenes[0]->addGameObject(new GameObject("player"));

	InitPhysics();
}

void Game::gameLoop()
{

    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
		//update physics
		UpdatePhysics();

        // update Scene
        scenes[currentScene]->update();
        
        // Draw Scene
        scenes[currentScene]->draw();
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
	return instance->deltaTime;
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
