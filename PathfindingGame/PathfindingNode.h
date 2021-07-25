#pragma once
#include "NecessaryHeaders.h"

struct PathfindingNode
{
	enum class Type : char
	{
		BLOCKED = 0,
		EASY = 5,
		REGULAR = 10,
		HARD = 20
	};

	//the game does not support nodes with a type per direction
	//each direction has the same type
	Type type;
	PathfindingNode* previous;
	PathfindingNode* neighbors[6];
	int fScore;
	int gScore;
	int indexX, indexY;
	Vector2 position;

};