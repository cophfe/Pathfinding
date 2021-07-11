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

enum SORTING
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

	inline SmoothCamera* getCamera() { return camera; }
	void setCameraTarget(Transform* transform);
	GameObject* addGameObject(GameObject* object, SORTING layer = SORTING::MIDGROUND);

private:
	SceneProperties* properties;
	//UI is kept seperately for rendering
	std::vector<GameObject*> UI;
	//to keep things simple there is a fixed amount of sorting layers: 3
	std::vector<GameObject*> sortingLayers[SORTING::COUNT];
	//camera for rendering obviously
	SmoothCamera* camera;

	Pathfinder* pathfinder;
	Image image;
};

