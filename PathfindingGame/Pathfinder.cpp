#include "Pathfinder.h"
#include "RigidBodyComponent.h"
#include "CollisionManager.h"

#define HEX_RAD_TO_APOTHEM 0.86602540378f //approx 6/7, exact sqrt(3)/2
#define HEX_APOTHEM_TO_RAD 1.15470053838f //approx 7/6, exact 2/sqrt(3)
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
	//startPosition.y += 0.15470053838f * nodeOffset; // I have no idea why but this fixes a bug with getting the point
	for (int x = 0; x < amountX; x++)
	{
		for (int y = 0; y < amountY; y++)
		{
			nodes[x][y] = new PathfindingNode();
			nodes[x][y]->Type = PathfindingNode::Type::REGULAR;
			if (rand() < RAND_MAX/7)
				nodes[x][y]->Type = PathfindingNode::Type::BLOCKED;
			nodes[x][y]->indexX = x;
			nodes[x][y]->indexY = y;
			if (y % 2 == 0)
				nodes[x][y]->position = Vector2(nodes[x][y]->indexX, (float)nodes[x][y]->indexY * HEX_RAD_TO_APOTHEM) * nodeOffset + startPosition;
			else	
				nodes[x][y]->position = Vector2(nodes[x][y]->indexX + 0.5f, (float)nodes[x][y]->indexY * HEX_RAD_TO_APOTHEM) * nodeOffset + startPosition;
			
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
			int hY = y < amountY - 1;

			if ((y % 2 == 0))
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
	//since changing to hexagonal tiles the node from point has gotton more complicated
	//we will find the tile by converting the hexagons into boxes and then checking if its in the valid part of those boxes
	//this is based on the first answer here https://stackoverflow.com/questions/7705228/hexagonal-grids-how-do-you-find-which-hexagon-a-point-is-in

	//just to make it easier to understand these are defined here, the compiler will optimise them out anyway
	float boxWidth = nodeOffset;
	float boxHalfWidth = nodeOffset/2;
	float boxHeight = nodeOffset * HEX_RAD_TO_APOTHEM;

	int indexY = y / boxHeight;
	
	//check if odd
	bool oddY = (indexY % 2) != 0;
	int indexX = oddY ? ((x + boxHalfWidth) / boxWidth) : (x / boxWidth);
	
	float relativeY = y - (indexY * boxHeight);
	float relativeX = oddY ? (indexX * boxWidth) : (indexX * boxHeight - boxHalfWidth);

	/*
		relative X and relative Y are now relative to the top left of the box
		we need m to check if it is in the hexagon or not
		y = mx + c
		mx = c - y
		m = (c - y) / x
		//at point y = 0 x = halfWidth so
		m = (c - 0) / halfWidth
		c is more confusing, the left line's c is equal to the point the line intersects with the leftmost point, which is one side length from the top
		(right line's is just -c)
		according to https://www.omnicalculator.com/math/hexagon the side length is equal to the radius
		radius is equal to nodeOffset / 2 * HEX_APOTHEM_TO_RAD
		therefore
		c = nodeOffset * HEX_RAD_TO_APOTHEM - nodeOffset / 2 * HEX_APOTHEM_TO_RAD
		m = (nodeOffset * HEX_RAD_TO_APOTHEM - nodeOffset / 2 * HEX_APOTHEM_TO_RAD) / halfWidth;
		m = nodeOffset * (HEX_RAD_TO_APOTHEM - HEX_APOTHEM_TO_RAD/2) / (nodeOffset/2)
		m = 2*(HEX_RAD_TO_APOTHEM - HEX_APOTHEM_TO_RAD/2)
		m = 2*HEX_RAD_TO_APOTHEM - HEX_APOTHEM_TO_RAD
		c = m * halfWidth
	*/
	const float m = 2 * HEX_RAD_TO_APOTHEM - HEX_APOTHEM_TO_RAD;
	float c = m * boxHalfWidth;
	
	//if y < mx + c the point lies under the line and it is in a different hexagon then the box would suggest
	if (relativeY < (-m * relativeX) + c)
	{
		indexY--;
		if (!oddY)
			indexX--;
	}
	if (relativeY < (m * relativeX) - c)
	{
		indexY--;
		if (oddY)
			indexX++;
	}

	if (indexX >= sizeX || indexY >= sizeY || indexX < 0 || indexY < 0)
		return nullptr;
	else
		return nodes[indexX][indexY];
}

PathfindingNode* Pathfinder::getNodeFromPoint(const Vector2* position)
{
	/*int x = ((position->x - startPosition.x) / nodeOffset) + 0.5f;
	int y = ((position->y - startPosition.y) / nodeOffset) + 0.5f;
	if (x >= sizeX || y >= sizeY || x < 0 || y < 0)
		return nullptr;
	return nodes[x][y];*/

	float boxWidth = nodeOffset;
	float boxHalfWidth = nodeOffset / 2;
	float boxHeight = nodeOffset * HEX_RAD_TO_APOTHEM;

	float sideLen = (nodeOffset / 2 * HEX_APOTHEM_TO_RAD);
	//int indexY = (position->y - startPosition.y) / boxHeight;
	int indexY = ((position->y - startPosition.y + sideLen) / boxHeight);

	//check if odd
	bool oddY = (indexY % 2) == 1;
	//int indexX = oddY ? ((position->x + boxHalfWidth - startPosition.x) / boxWidth) : ((position->x - startPosition.x) / boxWidth);
	int indexX = oddY ? ((position->x - startPosition.x ) / boxWidth) : ((position->x - startPosition.x + boxHalfWidth) / boxWidth);

 	float relativeY = (position->y - startPosition.y + sideLen) - (indexY * boxHeight);
	float relativeX = oddY ? (position->x - startPosition.x) - (indexX * boxWidth): (position->x - startPosition.x) - (indexX * boxWidth) + boxHalfWidth;
	const float m = 2 * HEX_RAD_TO_APOTHEM - HEX_APOTHEM_TO_RAD;
	float c = m * boxHalfWidth;

	//if y < mx + c the point lies under the line and it is in a different hexagon then the box would suggest
	if (relativeY < (-m * relativeX) + c)
	{
		indexY--;
		if (!oddY)
			indexX--;
	}
	else if (relativeY < (m * relativeX) - c)
	{
		indexY--;
		if (oddY)
			indexX++;
	}

	if (indexX >= sizeX || indexY >= sizeY || indexX < 0 || indexY < 0)
		return nullptr;
	else
		return nodes[indexX][indexY];
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
			if (nodes[x][y]->Type == PathfindingNode::Type::BLOCKED)
				c = &wall;
			else
				c = &regular;

			DrawPoly(nodes[x][y]->position, 6, nodeOffset / 2 * HEX_APOTHEM_TO_RAD * 0.95f, 0, *c);

			//draw centrepoint
			DrawCircleV(nodes[x][y]->position, 5, BLUE);

			//draw square grid (used for getting which hexagon we're in)
			//DrawRectangleLinesEx(Rectangle{ nodes[x][y]->position.x - (nodeOffset / 2),nodes[x][y]->position.y - (nodeOffset / 2 * HEX_APOTHEM_TO_RAD), nodeOffset,nodeOffset * HEX_RAD_TO_APOTHEM }, 4, BLACK);
			
			//draw lines
			/*for (int i = 0; i < 6; i++)
			{
				if (nodes[x][y]->neighbors[i])
					DrawLineEx(nodes[x][y]->neighbors[i]->position, nodes[x][y]->position, 5, regular);
			}*/
	
		}
	}
	
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
	//if (bool)target->Type is false, then target 
	if (start == nullptr || target == nullptr || target->Type == PathfindingNode::Type::BLOCKED || start == target)
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

			if (neighbor == nullptr || neighbor->Type == PathfindingNode::Type::BLOCKED || closedList[neighbor->indexX + neighbor->indexY * sizeX])
				continue;

			int newScore = current->gScore + (int)current->Type;
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
	vertices[0] = { (nodes[0][0]->position.x) / PHYSICS_UNIT_SCALE, -(nodes[0][0]->position.y) / PHYSICS_UNIT_SCALE };
	vertices[1] = { (nodes[sizeX - 1][0]->position.x) / PHYSICS_UNIT_SCALE, -(nodes[0][0]->position.y) / PHYSICS_UNIT_SCALE };
	vertices[2] = { (nodes[sizeX - 1][0]->position.x) / PHYSICS_UNIT_SCALE, (nodes[0][sizeY - 1]->position.y) / -PHYSICS_UNIT_SCALE };
	vertices[3] = { (nodes[0][0]->position.x) / PHYSICS_UNIT_SCALE, (nodes[0][sizeY - 1]->position.y) / -PHYSICS_UNIT_SCALE };
	bounds.CreateLoop((b2Vec2*)vertices, 4);
	fDef.shape = &bounds;

	b2Body* boundsBody = collision->getWorld()->CreateBody(&bDef);
	boundsBody->CreateFixture(&fDef);
}
