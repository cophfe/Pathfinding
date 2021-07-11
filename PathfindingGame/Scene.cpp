#include "Scene.h"
#include "GameObject.h"
#include "Transform.h"

void Scene::load()
{
	//GameObject* gO = new GameObject("Player.png", nullptr, true, Vector2(0, 0), 30, 1);
	//sortingLayers[SORTING::MIDGROUND].push_back(gO);
	camera = new SmoothCamera({ 0,0 }, 0, 1, { 0 }, 10);
}

void Scene::draw()
{
    BeginDrawing();
    ClearBackground(RAYWHITE);

	camera->UpdateCamera();
	camera->StartCamera();

	for (auto& drawLayer : sortingLayers)
	{
		for (auto gameObject : drawLayer)
		{
			gameObject->draw();
		}
	}

	camera->EndCamera();
    
	for (auto gameObject : UI)
	{
		gameObject->draw();
	}
	DrawFPS(0, 0);

    EndDrawing();
}

void Scene::update()
{
	for (int i = 0; i < SORTING::COUNT; i++)
	{
		for (int j = 0; j < sortingLayers[i].size(); j++)
		{
			sortingLayers[i][j]->updateComponents();
		}
	}
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

void Scene::setCameraTarget(Transform* transform)
{
	camera->Target(transform);
}

GameObject* Scene::addGameObject(GameObject* object, SORTING layer)
{
	sortingLayers[SORTING::MIDGROUND].push_back(object);
	return object;
}