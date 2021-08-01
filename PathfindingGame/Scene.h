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
	//loads everything
	virtual void load(SceneProperties* properties);
	//draws everything 
	//note:	(does not call BeginDrawing() or EndDrawing() itself)
	virtual void draw();
	//called every frame
	virtual void update();
	//called every frame, only updates components every PHYSICS_TIME_STEP
	virtual void fixedUpdate();
	//called at deletion
	virtual void unload();
	//note: the time it takes to start does not contribute to deltaTime
	virtual void start();
	//called before scene is deleted
	virtual void beforeDelete(Scene* nextScene) {}

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

	//getters
	CollisionManager* getCollisionManager();
	inline SmoothCamera* getCamera() { return camera; }
	
	//game object stuff
	GameObject* addGameObject(GameObject* object, SORTING layer = SORTING::MIDGROUND);
	void removeGameObjectFromChildren(GameObject* object);
	
	void setCameraTarget(Transform* transform);
	void setBackground(Color backgroundColor) { this->backgroundColor = backgroundColor; }

protected:
	//to keep things simple there is a fixed amount of sorting layers
	std::forward_list<GameObject*> sortingLayers[(int)SORTING::COUNT];
	//camera for rendering obviously
	SmoothCamera* camera;
	//holds box2d world
	CollisionManager* collisionManager;
	//the clear color
	Color backgroundColor;
	//used for fixed update
	float collisionTimer = 0;
	//checks if the scene is currently deleting itself
	bool isDeleting = false;
};