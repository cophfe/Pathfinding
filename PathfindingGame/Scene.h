#pragma once
#include "NecessaryHeaders.h"

class GameObject;
#include "Pathfinder.h"
#include "SmoothCamera.h"
class Transform;

struct SceneProperties
{
	Color backgroundColor;
	bool enablePhysics;
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
	void load();
	void draw();
	void update();
	void unload(); 

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

private:
	SceneProperties* properties;
	//UI is kept seperately for rendering
	std::vector<GameObject*> UI;
	//to keep things simple there is a fixed amount of sorting layers: 3
	std::vector<GameObject*> sortingLayers[(int)SORTING::COUNT];
	//camera for rendering obviously
	SmoothCamera* camera;

	Pathfinder* pathfinder;
};
