#pragma once
#include "NecessaryHeaders.h"

//info node containing pathfinding data
struct PathfindingNode
{
	//type data is linked to cost
	enum class Type : char
	{
		BLOCKED = 0,
		EASY = 5,
		REGULAR = 10,
		HARD = 20
	};

	
	//contains all the data necessary for astar
	PathfindingNode* previous;
	PathfindingNode* neighbors[6];
	Vector2 position;
	int fScore;
	int gScore;
	int indexX, indexY;
	//the game does not support nodes with a cost per direction
	//each direction has the same type
	Type type;
};