#include "Game.h"


void Game::init(GameProperties* properties )
{

	//Tell raylib to use antialiasing
	if (properties->enableAntiAliasing)
		SetConfigFlags(FLAG_MSAA_4X_HINT);
    
    //initialize window with initial properties
    InitWindow(properties->windowWidth, properties->windowHeight, properties->windowName);
    SetTargetFPS(properties->targetFPS);

	/*
		scene creation
		the plan is to create scenes from json files or something, sorta like a midpoint between a level editor and typing out scenes in code 
		for right now it will just create one scene with a player object in it
	*/
	//scenes.reserve(sizeof(Scene) * amountOfScenes);
	scenes.push_back(new Scene());
}

void Game::gameLoop()
{
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update Scene
        scenes[currentScene]->update();
        
        // Draw Scene
        scenes[currentScene]->draw();
    }
}

void Game::shutdown()
{
    CloseWindow();

	for (size_t i = 0; i < scenes.size(); i++)
	{
		delete scenes[i];
	}
}
