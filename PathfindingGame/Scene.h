#pragma once
#include "NecessaryHeaders.h"

class GameObject;
#include "Pathfinder.h"
#include "SmoothCamera.h"
#include "CollisionManager.h"
class Transform;

struct SceneProperties
{
	const char* backgroundTiling = nullptr;
	float backgroundScale = 1;
	const char* wall = "wall";
	Color backgroundColor = Color{0x96,0x6C,0x23,0xFF};
	int pathWidth = 11;
	int pathHeight = 11;
	float hexOffset = 250;
	int randomSeed = 9999999;
	bool generateWalls = true;
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
	UI,
	COUNT
};

class Scene
{
public:
	void load(SceneProperties& properties, int sceneID);
	void draw();
	void update();
	void unload(); 
	void start();

	CollisionManager* getCollisionManager();
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	Scene() = default;
	//copy
	Scene(const Scene&) = delete;
	Scene& operator=(const Scene&) = delete;
	//move
	Scene(Scene&&) = delete;
	Scene& operator=(Scene&&) = delete;
	~Scene();
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

	inline SmoothCamera* getCamera() { return camera; }
	void setCameraTarget(Transform* transform);
	GameObject* addGameObject(GameObject* object, SORTING layer = SORTING::MIDGROUND);
	inline Pathfinder* getPathfinder() { return pathfinder; }

private:
	//to keep things simple there is a fixed amount of sorting layers: 3
	std::vector<GameObject*> sortingLayers[(int)SORTING::COUNT];
	//camera for rendering obviously
	SmoothCamera* camera;
	
	//used for fixed update
	float collisionTimer = 0;

	Pathfinder* pathfinder;
	CollisionManager* collisionManager;
	//bounds is the only physics body that isn't attached to a rigidbody component
	b2Body* bounds;

	Color backgroundColor;
};