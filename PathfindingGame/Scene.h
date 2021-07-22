#pragma once
#include "NecessaryHeaders.h"

class GameObject;
#include "Pathfinder.h"
#include "SmoothCamera.h"
#include "CollisionManager.h"
class Transform;


#define PATHWIDTH 16
#define PATHHEIGHT 10
#define HEX_OFFSET 250

struct SceneProperties
{
	const char* backgroundTiling = nullptr;
	float backgroundScale = 1;
	const char* wall = "wall";
	Color backgroundColor = BLACK;
	int pathWidth = 10;
	int pathHeight = 10;
	float hexOffset = 250;
};

enum class SORTING
{
	BACKGROUND,
	MIDGROUND,
	FOREGROUND,
	COUNT
};

class Scene
{
public:
	void load(SceneProperties& properties);
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
	SceneProperties* properties;
	//UI is kept seperately for rendering
	std::vector<GameObject*> UI;
	//to keep things simple there is a fixed amount of sorting layers: 3
	std::vector<GameObject*> sortingLayers[(int)SORTING::COUNT];
	//camera for rendering obviously
	SmoothCamera* camera;
	
	float collisionTimer = 0;

	Pathfinder* pathfinder;

	CollisionManager* collisionManager;

	b2Body* bounds;

	Color backgroundColor;
};

/*
{
	"gameobjects" : [
	{
		"components" : [
		],

	}
	,{

	}
	]
}
*/