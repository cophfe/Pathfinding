#include "Scene.h"
#include "GameObject.h"
#include "Transform.h"
#include "Game.h"

void Scene::load()
{
	//GameObject* gO = new GameObject("Player.png", nullptr, true, Vector2(0, 0), 30, 1);
	//sortingLayers[SORTING::MIDGROUND].push_back(gO);
	camera = new SmoothCamera({ 0,0 }, 0, 1 , { 0 }, 10);

	pathfinder = new Pathfinder();

	collisionManager = new CollisionManager();
}

void Scene::draw()
{
    BeginDrawing();
    ClearBackground(RAYWHITE);

	camera->StartCamera();

	for (auto& drawLayer : sortingLayers)
	{
		for (auto gameObject : drawLayer)
		{
			gameObject->draw();
		}
	}

#ifdef DRAW_DEBUG
	collisionManager->debugDraw();
#endif // DRAW_DEBUG
	
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

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// 	   REGULAR UPDATE
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	for (int i = 0; i < (int)SORTING::COUNT; i++)
	{
		for (int j = 0; j < sortingLayers[i].size(); j++)
		{
			sortingLayers[i][j]->updateComponents();
		}
	}

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// 	   FIXED UPDATE
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	collisionTimer += Game::getDeltaTime();
	while (collisionTimer >= PHYSICS_TIME_STEP)
	{
		camera->UpdateCamera();
		collisionManager->update();
		collisionTimer -= PHYSICS_TIME_STEP;

		for (int i = 0; i < (int)SORTING::COUNT; i++)
		{
			for (int j = 0; j < sortingLayers[i].size(); j++)
			{
				sortingLayers[i][j]->fixedUpdateComponents();
			}
		}
	}
}

void Scene::unload()
{
	for (int i = 0; i < (int)SORTING::COUNT; i++)
	{
		for (int j = 0; j < sortingLayers[i].size(); j++)
		{
			delete sortingLayers[i][j];
		}
	}

	delete collisionManager;

	delete pathfinder;

	delete camera;

}

void Scene::start()
{
	for (int i = 0; i < (int)SORTING::COUNT; i++)
	{
		for (int j = 0; j < sortingLayers[i].size(); j++)
		{
			sortingLayers[i][j]->startComponents();
		}
	}

	
}

CollisionManager* Scene::getCollisionManager()
{
	return collisionManager;
}

Scene::~Scene()
{
	unload();
}

void Scene::setCameraTarget(Transform* transform)
{
	camera->Target(transform);
}

GameObject* Scene::addGameObject(GameObject* object, SORTING layer)
{
	sortingLayers[(int)SORTING::MIDGROUND].push_back(object);
	return object;
}