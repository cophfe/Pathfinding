#include "Pathfinder.h"

Pathfinder::Pathfinder(int amountX, int amountY, float nodeOffset)
{
	this->nodeOffset = nodeOffset;
	length = amountX * amountY;
	sizeX = amountX;
	sizeY = amountY;
	closedList = new bool*[amountX];
	
	nodes = new PathfindingNode ** [amountX];

	for (int i = 0; i < amountX; i++)
	{
		closedList[i] = new bool [amountY];
		nodes[i] = new PathfindingNode*[amountY];

	}
	for (int x = 0; x < amountX; x++)
	{
		for (int y = 0; y < amountY; y++)
		{
			nodes[x][y] = new PathfindingNode();
			nodes[x][y]->cost = PathfindingNode::Type::REGULAR;
			nodes[x][y]->indexX = x;
			nodes[x][y]->indexY = y;
			memset(nodes[x][y]->neighbors, 0, sizeof(PathfindingNode*) * 4);
		}
	}
	for (int x = 0; x < amountX; x++)
	{
		for (int y = 0; y < amountY; y++)
		{
			if (x > 0)
				nodes[x][y]->neighbors[0] = nodes[x-1][y];

			if (x < amountX - 1)
				nodes[x][y]->neighbors[1] = nodes[x+1][y];

			if (y > 0)
				nodes[x][y]->neighbors[2] = nodes[x][y-1];

			if (y < amountY -1)
				nodes[x][y]->neighbors[3] = nodes[x][y+1];
		}
		
	}

}

Pathfinder::~Pathfinder()
{
	for (int x = 0; x < sizeX; x++)
	{
		for (int y = 0; y < sizeY; y++)
		{
			delete nodes[x][y];
		}
		delete[] nodes[x];
		delete[] closedList[x];
	}
	delete[] nodes;
	delete[] closedList;
	
}

PathfindingNode* Pathfinder::getNodeFromPoint(float x, float y)
{
	return nullptr;
}

PathfindingNode* Pathfinder::getNodeFromIndex(int x, int y)
{
	return nodes[x][y];
}

int Pathfinder::getHeuristic(PathfindingNode* node, PathfindingNode* end)
{
	int dx = abs(end->indexX - node->indexX);
	int dy = abs(end->indexY - node->indexY);

	return (int)PathfindingNode::Type::REGULAR * (dx + dy);
}

int Pathfinder::AStarPath(PathfindingNode* start, PathfindingNode* end, std::vector<Vector2>* finalPath)
{
	finalPath->clear();

	//if (bool)end->cost is false, then end 
	if (start == nullptr || end == nullptr || end->cost == PathfindingNode::Type::BLOCKED || start == end)
		return 0;

	openList.clear();
	memset(closedList, 0, sizeof(bool) * length);
	start->gScore = 0;
	start->previous = nullptr;

	openList.push(start);
	
	while (openList.size() > 0)
	{
		PathfindingNode* current = openList.pop();
		closedList[current->indexX][current->indexY] = true;

		if (current == end)
		{
			finalPath->push_back({ end->indexX * nodeOffset, end->indexY * nodeOffset });
			static int pathLength = 0;
			pathLength = current->fScore;
			while (current->previous != nullptr)
			{
				finalPath->push_back({ current->indexX * nodeOffset , current->indexY * nodeOffset });
				current = current->previous;
			}
			return pathLength;
		}

		for (int n = 0; n < 4; n++)
		{
			PathfindingNode* neighbor = current->neighbors[n];

			if (neighbor == nullptr || neighbor->cost == PathfindingNode::Type::BLOCKED || closedList[neighbor->indexX][neighbor->indexY])
				continue;

			int newScore = current->gScore + (int)current->cost;
			if (openList.contains(neighbor))
			{
				if (neighbor->gScore > newScore)
				{
					neighbor->previous = current;
					neighbor->gScore = newScore;
					neighbor->fScore = neighbor->gScore + getHeuristic(neighbor, end);
					openList.updateNode(neighbor);
				}
			}
			else
			{
				neighbor->previous = current;
				neighbor->gScore = newScore;
				neighbor->fScore = neighbor->gScore + getHeuristic(neighbor, end);
				openList.push(neighbor);
			}
		}
	}
}
