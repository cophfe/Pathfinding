#include "Scene.h"
#include "GameObject.h"
#include "Transform.h"
#include "Game.h"

void Scene::load(SceneProperties* properties)
{
	camera = new SmoothCamera({ 0,0 }, 0, 1 , { 0 }, 10);
	GameObject::resetIdCounter();

	collisionManager = new CollisionManager();
	backgroundColor = BLACK;
}

void Scene::draw()
{
    ClearBackground(backgroundColor);

	camera->StartCamera();


	for (int i = 1; i < (int)SORTING::COUNT - 1; ++i)
	{
		for (auto gameObject : sortingLayers[i])
		{
			gameObject->draw();
		}
	}

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//		DEBUG RENDERING
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#ifdef DRAW_DEBUG
	for (int i = 0; i < (int)SORTING::COUNT; ++i)
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
#ifdef DRAW_DEBUG
	DrawFPS(0, 0);
#endif // DRAW_DEBUG
}

void Scene::update()
{
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// 	   REGULAR UPDATE
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	for (int i = 0; i < (int)SORTING::COUNT; i++)
	{
		for (auto gameObject : sortingLayers[i])
		{
			gameObject->updateComponents();
		}
	}
}

void Scene::fixedUpdate()
{
	collisionTimer += Game::getDeltaTime();
	while (collisionTimer >= PHYSICS_TIME_STEP)
	{
		camera->UpdateCamera();
		collisionManager->update();
		collisionTimer -= PHYSICS_TIME_STEP;

		for (int i = 0; i < (int)SORTING::COUNT; i++)
		{
			for (auto gameObject : sortingLayers[i])
			{
				gameObject->fixedUpdateComponents();
			}
		}
	}
}

void Scene::unload()
{
	isDeleting = true;
	for (int i = 0; i < (int)SORTING::COUNT; i++)
	{
		for (auto&& gameObject : sortingLayers[i])
		{
			delete gameObject;
		}
	}

	delete collisionManager;

	delete camera;
}

void Scene::start()
{
	for (int i = 0; i < (int)SORTING::COUNT; i++)
	{
		for (auto gameObject : sortingLayers[i])
		{
			gameObject->startComponents();
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
	sortingLayers[(int)layer].push_front(object);
	return object;
}

void Scene::removeGameObjectFromChildren(GameObject* object)
{
	//do not remove gameObjects if scene is deleting itself
	if (!isDeleting)
	{
		for (int i = 0; i < (int)SORTING::COUNT; i++)
		{
			sortingLayers[i].remove(object);
		}
	}
}
