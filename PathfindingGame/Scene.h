#pragma once
#include "NecessaryHeaders.h"

#include "GameObject.h"
#include "Pathfinder.h"

struct SceneProperties
{
	Color backgroundColor;
	bool enablePhysics;
};

enum SORTING
{
	BACKGROUND,
	CENTRE,
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

	void addGameObject(GameObject& object);

private:
	SceneProperties* properties;
	//UI is kept seperately for rendering
	std::vector<GameObject*> UI;
	//to keep things simple there is a fixed amount of sorting layers: 3
	std::vector<GameObject*> sortingLayers[SORTING::COUNT];
	//camera for rendering obviously
	Camera2D* camera; //should probably have a custom class wrapper for cleaner movement

	Pathfinder* pathfinder;
	Image image;
};

