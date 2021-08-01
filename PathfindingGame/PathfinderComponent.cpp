#include "PathfinderComponent.h"
#include "Scene.h"

void PathfinderComponent::init(Scene* scene, int pathWidth, int pathHeight, bool generateWalls)
{
	this->generateWalls = generateWalls;
	//initiate pathfinder

	pathfinder = new Pathfinder(pathWidth, pathHeight, HEX_OFFSET,
		{ -pathWidth * HEX_OFFSET * 0.5f, -pathHeight * HEX_OFFSET * 0.5f });

	pathfinder->generateBoundsFromGraph(scene->getCollisionManager(), &bounds);
	pathfinder->initDraw(BACKGROUND_TILING, BACKGROUND_SCALE, BACKGROUND_COLOR);
}

void PathfinderComponent::start()
{
}

void PathfinderComponent::unload()
{
	delete pathfinder;
}

void PathfinderComponent::drawBackground()
{
	pathfinder->drawBack();
}

void PathfinderComponent::drawBoundWalls()
{
	pathfinder->drawWalls();
}
