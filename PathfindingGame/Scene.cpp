#include "Scene.h"
#include "GameObject.h"
#include "Transform.h"
#include "Game.h"

void Scene::load(SceneProperties& properties, int sceneID)
{

	camera = new SmoothCamera({ 0,0 }, 0, 1 , { 0 }, 10);
	GameObject::resetIdCounter();

	pathfinder = new Pathfinder(properties.pathWidth, properties.pathHeight, properties.hexOffset,
			{ -properties.pathWidth * properties.hexOffset * 0.5f, -properties.pathHeight * properties.hexOffset * 0.5f });

	collisionManager = new CollisionManager();

	pathfinder->generateBoundsFromGraph(collisionManager, &bounds);
	pathfinder->generateWalls(collisionManager, properties.wall, this);
	pathfinder->initDraw(properties.backgroundTiling, properties.backgroundScale);
	
	backgroundColor = properties.backgroundColor;
}

void Scene::draw()
{
    BeginDrawing();
    ClearBackground(backgroundColor);

	camera->StartCamera();

	pathfinder->draw();

	for (auto& drawLayer : sortingLayers)
	{
		for (auto gameObject : drawLayer)
		{
			gameObject->draw();
		}
	}

	//		DEBUG RENDERING
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#ifdef DRAW_DEBUG
	for (int i = 0; i < (int)SORTING::COUNT - 1; ++i)
	{
		for (auto gameObject : sortingLayers[i])
		{
			gameObject->debugDraw();
		}
	}
	collisionManager->debugDraw();
#endif // DRAW_DEBUG
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	
	camera->EndCamera();
	for (auto gameObject : sortingLayers[(int)SORTING::UI])
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

	if (bounds)
		collisionManager->getWorld()->DestroyBody(bounds);

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
	sortingLayers[(int)layer].push_back(object);
	return object;
}