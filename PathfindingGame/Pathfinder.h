#pragma once
#include "PathfindingNode.h"
#include "NecessaryHeaders.h"
#include "Heap.h"

class Pathfinder
{
public:
	Pathfinder(int amountX, int amountY, float nodeOffset);
	~Pathfinder();

	PathfindingNode* getNodeFromPoint(float x, float y);
	PathfindingNode* getNodeFromIndex(int x, int y);

	int getHeuristic(PathfindingNode* node, PathfindingNode* end);
	int AStarPath(PathfindingNode* start, PathfindingNode* end, std::vector<Vector2>* finalPath);

private:
	PathHeap openList;
	bool** closedList;

	PathfindingNode*** nodes;

	int sizeX, sizeY, length;
	float nodeOffset;
};

