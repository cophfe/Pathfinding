#pragma once
#include "PathfindingNode.h"
#include "NecessaryHeaders.h"
#include "Heap.h"

class CollisionManager;

class Pathfinder
{
public:
	Pathfinder(int amountX, int amountY, float nodeOffset, Vector2 startPosition = { 0 });
	~Pathfinder();

	PathfindingNode* getNodeFromPoint(float x, float y);
	PathfindingNode* getNodeFromPoint(const Vector2* position);
	PathfindingNode* getNodeFromIndex(int x, int y);

	void draw();

	int getHeuristic(PathfindingNode* node, PathfindingNode* end);
	int AStarPath(PathfindingNode* start, PathfindingNode* end, std::vector<Vector2>* finalPath);

	void generateBoundsFromGraph(CollisionManager* collision, b2Body** boundsBodyPointer);
private:
	PathHeap openList;
	bool* closedList;

	PathfindingNode*** nodes;

	Vector2 startPosition;
	int sizeX, sizeY, length;
	float nodeOffset;
};

