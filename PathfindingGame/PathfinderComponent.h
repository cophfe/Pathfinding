#pragma once
#include "NecessaryHeaders.h"
#include "Component.h"
#include "Pathfinder.h"

class Scene;

class PathfinderComponent :
    public Component
{
public:
	static constexpr const char* BACKGROUND_TILING = "background";
	static constexpr float BACKGROUND_SCALE = 1.436781609195402f * 1.2;
	static constexpr Color BACKGROUND_COLOR = Color{ 0x96,0x6C,0x23,0xFF };
	static constexpr float HEX_OFFSET = 300;
	static constexpr int DEFAULT_PATH_WIDTH = 11;
	static constexpr int DEFAULT_PATH_HEIGHT = 11;

	inline Pathfinder* getPathfinder() { return pathfinder; }
	void init(Scene* scene, int pathWidth = DEFAULT_PATH_WIDTH, int pathHeight = DEFAULT_PATH_HEIGHT, bool generateWalls = true);
	void start();
	void unload();
	void drawBackground();
	void drawBoundWalls();

private:
	Pathfinder* pathfinder;
	bool generateWalls;
	b2Body* bounds;

};

