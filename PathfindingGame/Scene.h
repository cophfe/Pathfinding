#pragma once
#include "NecessaryHeaders.h"

class GameObject;
#include "SmoothCamera.h"
#include "CollisionManager.h"
#include "Pathfinder.h"
class Transform;

struct SceneProperties
{
	int randomSeed = 0;
};

//scenes can be loaded in and out of memory to switch with a new scene
//when the current scene is unloaded, it asks every gameObject if it wants to save any data
//then the gameObject asks every component
//if a component says yes, and returns a ComponentData* value, the GameObject will say yes, and return a GameObjectData* value
//then the scene will return it's scene data to Game
//this only saves changes. If no data is saved, the scene will load just like it did initially
//struct SceneData
//{
//	//tied to the id's of gameObjects
//	//std::map<int, GameObjectData*> gameObjectData;
//};

enum class SORTING
{
	BACKGROUND,
	MIDGROUND,
	FOREGROUND,
	UI,
	COUNT
};

class Scene
{
public:
	virtual void load(SceneProperties* properties);
	virtual void draw();
	virtual void update();
	virtual void fixedUpdate();
	virtual void unload(); 
	virtual void start();

	CollisionManager* getCollisionManager();
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	Scene() = default;
	//copy
	Scene(const Scene&) = delete;
	Scene& operator=(const Scene&) = delete;
	//move
	Scene(Scene&&) = delete;
	Scene& operator=(Scene&&) = delete;
	//destructor
	virtual ~Scene();
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

	inline SmoothCamera* getCamera() { return camera; }
	void setCameraTarget(Transform* transform);
	GameObject* addGameObject(GameObject* object, SORTING layer = SORTING::MIDGROUND);
	void removeGameObjectFromChildren(GameObject* object);
	virtual void beforeDelete(Scene* nextScene) {}

protected:
	//to keep things simple there is a fixed amount of sorting layers: 3
	std::forward_list<GameObject*> sortingLayers[(int)SORTING::COUNT];
	//camera for rendering obviously
	SmoothCamera* camera;
	//holds box2d world
	CollisionManager* collisionManager;
	
	bool isDeleting = false;
	bool shouldChangeScene = false;
	//used for fixed update
	float collisionTimer = 0;


	Color backgroundColor;
};