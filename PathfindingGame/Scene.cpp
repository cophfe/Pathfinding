#include "Scene.h"
#include "GameObject.h"

void Scene::load()
{
	//GameObject* gO = new GameObject("Player.png", nullptr, true, Vector2(0, 0), 30, 1);
	//sortingLayers[SORTING::CENTRE].push_back(gO);
	camera = new Camera2D();
	camera->offset = { 0 };
	camera->rotation = 0;
	camera->target = { 0 };
	camera->zoom = 1;
}

void Scene::draw()
{
    BeginDrawing();
    ClearBackground(RAYWHITE);
	BeginMode2D(*camera);

	for (auto& drawLayer : sortingLayers)
	{
		for (auto gameObject : drawLayer)
		{
			gameObject->draw();
		}
	}

	EndMode2D();
    
	for (auto gameObject : UI)
	{
		gameObject->draw();
	}
	DrawFPS(0, 0);

    EndDrawing();
}

void Scene::update()
{
	
}

void Scene::unload()
{
	for (int i = 0; i < SORTING::COUNT; i++)
	{
		for (int j = 0; j < sortingLayers[i].size(); j++)
		{
			delete sortingLayers[i][j];
		}
		
	}
}

void Scene::addGameObject(GameObject* object)
{
	sortingLayers[SORTING::CENTRE].push_back(object);
}