#include "Pathfinder.h"
#include "RigidBodyComponent.h"
#include "CollisionManager.h"

Pathfinder::Pathfinder(int amountX, int amountY, float nodeOffset, Vector2 startPosition)
{
	this->startPosition = startPosition;
	this->nodeOffset = nodeOffset;
	length = amountX * amountY;
	sizeX = amountX;
	sizeY = amountY;
	closedList = new bool[length];
	
	nodes = new PathfindingNode ** [amountX];

	for (int i = 0; i < amountX; i++)
	{
		nodes[i] = new PathfindingNode*[amountY];
	}
	for (int x = 0; x < amountX; x++)
	{
		for (int y = 0; y < amountY; y++)
		{
			nodes[x][y] = new PathfindingNode();
			nodes[x][y]->cost = PathfindingNode::Type::REGULAR;
			if (rand() < RAND_MAX/7)
				nodes[x][y]->cost = PathfindingNode::Type::BLOCKED;
			nodes[x][y]->indexX = x;
			nodes[x][y]->indexY = y;
			if (y % 2 != 0)
				nodes[x][y]->position = Vector2(nodes[x][y]->indexX, (float)nodes[x][y]->indexY/ 1.15f) * nodeOffset + startPosition;
			else	
				nodes[x][y]->position = Vector2(nodes[x][y]->indexX + 0.5f, (float)nodes[x][y]->indexY/1.15f) * nodeOffset + startPosition;
			
		}
	}
	for (int x = 0; x < amountX; x++)
	{
		for (int y = 0; y < amountY; y++)
		{
			/*
			  Neighbor index

				 0  *  1
			   *	     *
			   5         2
			   *	     *
				 4  *  3

			*/
			//okay this is possibly the most cursed code ever but it works
			int lX = x > 0;
			int hX = x < amountX - 1;
			int lY = y > 0;
			int hY = y < amountX - 1;

			if ((y % 2))
			{
				nodes[x][y]->neighbors[0] = (PathfindingNode*)((size_t)nodes[x - 1 * lX][y - 1 * lY] * (lX * lY));
				nodes[x][y]->neighbors[1] = (PathfindingNode*)((size_t)nodes[x][y - 1 * lY] * (lY));
				nodes[x][y]->neighbors[3] = (PathfindingNode*)((size_t)nodes[x][y + 1 * hY] * (hY));
				nodes[x][y]->neighbors[4] = (PathfindingNode*)((size_t)nodes[x - 1 *lX ][y + hY] * (lX * hY));
			}
			else
			{
				nodes[x][y]->neighbors[0] = (PathfindingNode*)((size_t)nodes[x - 1 * lX][y] * (lX));
				nodes[x][y]->neighbors[1] = (PathfindingNode*)((size_t)nodes[x + 1 * hX][y - 1 * lY] * (hX * lY));
				nodes[x][y]->neighbors[3] = (PathfindingNode*)((size_t)nodes[x + 1 * hX][y + 1 * hY] * (hX * hY));
				nodes[x][y]->neighbors[4] = (PathfindingNode*)((size_t)nodes[x][y + 1 * hY] * (hY));
			}
			nodes[x][y]->neighbors[2] = (PathfindingNode*)((size_t)nodes[x + 1 * hX][y] * (hX));
			nodes[x][y]->neighbors[5] = (PathfindingNode*)((size_t)nodes[x - 1 * lX][y] * (lX));
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
	}
	delete[] nodes;
	delete[] closedList;
	
}

PathfindingNode* Pathfinder::getNodeFromPoint(float x, float y)
{
	static float boxWidth = nodeOffset;
	static float boxHalfWidth = nodeOffset/2;
	static float boxHeight = nodeOffset / 1.15f;

	int indexY = y / boxHeight;
	
	//check if odd
	bool oddY = (indexY % 2) != 0;

	int indexX = oddY ? (x + boxHalfWidth) / boxWidth : x / boxWidth;
	
	float relativeY = y - (indexY * boxHeight);
	
}

PathfindingNode* Pathfinder::getNodeFromPoint(const Vector2* position)
{
	int x = ((position->x - startPosition.x) / nodeOffset) + 0.5f;
	int y = ((position->y - startPosition.y) / nodeOffset) + 0.5f;
	if (x >= sizeX || y >= sizeY || x < 0 || y < 0)
		return nullptr;
	return nodes[x][y];
}

PathfindingNode* Pathfinder::getNodeFromIndex(int x, int y)
{
	return nodes[x][y];
}

void Pathfinder::draw()
{
	Color regular = GRAY;
	Color wall = DARKBLUE;
	regular.a = 100;
	wall.a = 200;
	Color* c = &regular;

	for (int x = 0; x < sizeX; x++)
	{
		for (int y = 0; y < sizeY; y++)
		{
			if (nodes[x][y]->cost == PathfindingNode::Type::BLOCKED)
				c = &wall;
			else
				c = &regular;
			DrawPoly(nodes[x][y]->position, 6, nodeOffset / 2 * 1.15473441f * 0.95f, 0, *c);
			DrawCircleV(nodes[x][y]->position, 5, BLUE);

			//draw lines
			/*for (int i = 0; i < 6; i++)
			{
				if (nodes[x][y]->neighbors[i])
					DrawLineEx(nodes[x][y]->neighbors[i]->position, nodes[x][y]->position, 5, wall);
			}*/
	
		}
	}
	
	if (start) DrawPoly(start->position, 6, nodeOffset / 2 * 1.15473441f * 0.7f, 0, RED);
	if (target) DrawPoly(target->position, 6, nodeOffset / 2 * 1.15473441f * 0.7f, 0, PURPLE);
	
}

int Pathfinder::getHeuristic(PathfindingNode* node, PathfindingNode* end)
{
	//excellent resource for hex grid:
	//http://www-cs-students.stanford.edu/~amitp/Articles/HexLOS.html
	//gives exact heuristic value

	int dx = end->indexX - node->indexX;
	int dy = end->indexY - node->indexY;

	if (((dx > 0) - (dx < 0)) == ((dy > 0) - (dy < 0)))
		return std::max(abs(dx), abs(dy));
	else
		return abs(dx) + abs(dy);

	return (int)PathfindingNode::Type::REGULAR * (dx + dy);
}

int Pathfinder::AStarPath(PathfindingNode* start, PathfindingNode* target, std::vector<Vector2>* finalPath)
{
	this->start = start;
	this->target = target;
	//if (bool)target->cost is false, then target 
	if (start == nullptr || target == nullptr || target->cost == PathfindingNode::Type::BLOCKED || start == target)
		return 0;

	openList.clear();
	memset(closedList, 0, sizeof(bool) * length);
	start->gScore = 0;
	start->previous = nullptr;

	openList.push(start);
	
	while (openList.size() > 0)
	{
		PathfindingNode* current = openList.pop();
		closedList[current->indexX + current->indexY * sizeX] = true;

		if (current == target)
		{
			finalPath->clear();
			finalPath->push_back(current->position);
			static int pathLength = 0;
			pathLength = current->gScore;
			while (current->previous != nullptr)
			{
				finalPath->push_back( current->position );
				current = current->previous;
			}
			return pathLength;
		}

		for (int n = 0; n < 6; n++)
		{
			PathfindingNode* neighbor = current->neighbors[n];

			if (neighbor == nullptr || neighbor->cost == PathfindingNode::Type::BLOCKED || closedList[neighbor->indexX + neighbor->indexY * sizeX])
				continue;

			int newScore = current->gScore + (int)current->cost;
			if (openList.contains(neighbor))
			{
				if (neighbor->gScore > newScore)
				{
					neighbor->previous = current;
					neighbor->gScore = newScore;
					neighbor->fScore = neighbor->gScore + getHeuristic(neighbor, target);
					openList.updateNode(neighbor);
				}
			}
			else
			{
				neighbor->previous = current;
				neighbor->gScore = newScore;
				neighbor->fScore = neighbor->gScore + getHeuristic(neighbor, target);
				openList.push(neighbor);
			}
		}
	}
}

void Pathfinder::generateBoundsFromGraph(CollisionManager* collision, b2Body** boundsBodyPointer)
{
	//generate bounds surrounding the graph

	b2BodyDef bDef; //default bodyDef is perfect
	bDef.userData.pointer = (uintptr_t)*boundsBodyPointer;
	b2FixtureDef fDef;
	fDef.filter.categoryBits = RigidBodyComponent::BOUNDS;
	fDef.filter.maskBits = RigidBodyComponent::ALL;
	b2ChainShape bounds;
	Vector2* vertices = new Vector2[4];
	vertices[0] = { (startPosition.x - nodeOffset / 2) / PHYSICS_UNIT_SCALE, -(startPosition.y - nodeOffset/2) / PHYSICS_UNIT_SCALE };
	vertices[1] = { (startPosition.x + nodeOffset * sizeX - nodeOffset / 2) / PHYSICS_UNIT_SCALE, -(startPosition.y - nodeOffset / 2) / PHYSICS_UNIT_SCALE };
	vertices[2] = { (startPosition.x + nodeOffset * sizeX - nodeOffset / 2) / PHYSICS_UNIT_SCALE, (startPosition.y + nodeOffset * sizeY - nodeOffset / 2) / -PHYSICS_UNIT_SCALE };
	vertices[3] = { (startPosition.x - nodeOffset / 2) / PHYSICS_UNIT_SCALE, (startPosition.y + nodeOffset * sizeY - nodeOffset / 2) / -PHYSICS_UNIT_SCALE };
	bounds.CreateLoop((b2Vec2*)vertices, 4);
	fDef.shape = &bounds;

	b2Body* boundsBody = collision->getWorld()->CreateBody(&bDef);
	boundsBody->CreateFixture(&fDef);
}
