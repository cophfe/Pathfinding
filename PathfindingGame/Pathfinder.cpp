#include "Pathfinder.h"
#include "RigidBodyComponent.h"
#include "CollisionManager.h"
#include "Scene.h"
#include "GameObject.h"
#include "Game.h"

Pathfinder::Pathfinder(int amountX, int amountY, float nodeOffset, Vector2 startPosition)
{
	//construct pathfinder from data

	this->startPosition = startPosition;
	this->nodeOffset = nodeOffset;
	length = amountX * amountY;
	sizeX = amountX;
	sizeY = amountY;
	closedList = new bool[length];
	
	nodes = new PathfindingNode ** [amountX];

	//initiate all pathfinding nodes
	for (int i = 0; i < amountX; i++)
	{
		nodes[i] = new PathfindingNode*[amountY];
	}
	for (int x = 0; x < amountX; x++)
	{
		for (int y = 0; y < amountY; y++)
		{
			//set position, type, and index
			nodes[x][y] = new PathfindingNode();
			nodes[x][y]->type = PathfindingNode::Type::REGULAR;
			//walls generated around the sides
			if (y == 0 || y == amountY - 1 || x == 0 || x == amountX - 1)
			{
				nodes[x][y]->type = PathfindingNode::Type::BLOCKED;
			}
			
			nodes[x][y]->indexX = x;
			nodes[x][y]->indexY = y;
			if (y % 2 != 0)
				nodes[x][y]->position = Vector2(nodes[x][y]->indexX, (float)nodes[x][y]->indexY * HEX_RAD_TO_APOTHEM) * nodeOffset + startPosition;
			else	
				nodes[x][y]->position = Vector2(nodes[x][y]->indexX + 0.5f, (float)nodes[x][y]->indexY * HEX_RAD_TO_APOTHEM) * nodeOffset + startPosition;
			
		}
	}
	//connect to neighbors
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

			if ((y % 2 != 0))
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

	float boxWidth = nodeOffset;
	float boxHalfWidth = nodeOffset/2;
	float boxHeight = nodeOffset * HEX_RAD_TO_APOTHEM;
	float sideLen = (nodeOffset / 2 * HEX_APOTHEM_TO_RAD);

	int indexY = y / boxHeight;
	
	//check if odd
	bool evenY = (indexY % 2) == 0;
	//int indexX = evenY ? ((position->x + boxHalfWidth - startPosition.x) / boxWidth) : ((position->x - startPosition.x) / boxWidth);
	int indexX = evenY ? ((x - startPosition.x) / boxWidth) : ((x - startPosition.x + boxHalfWidth) / boxWidth);

	float relativeY = (y - startPosition.y + sideLen) - (indexY * boxHeight);
	float relativeX = evenY ? (x - startPosition.x) - (indexX * boxWidth) : (x - startPosition.x) - (indexX * boxWidth) + boxHalfWidth;
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
		if (!evenY)
			indexX--;
	}
	//same if the point lies under this opposite line
	else if (relativeY < (m * relativeX) - c)
	{
		indexY--;
		if (evenY)
			indexX++;
	}

	//now check if it is in the bounds of what makes sense
	if (indexX >= sizeX || indexY >= sizeY || indexX < 0 || indexY < 0)
		return nullptr;
	else
		return nodes[indexX][indexY];
}

PathfindingNode* Pathfinder::getNodeFromPoint(const Vector2* position)
{
	//more detailed comments in other version of this function
	float boxWidth = nodeOffset;
	float boxHalfWidth = nodeOffset / 2;
	float boxHeight = nodeOffset * HEX_RAD_TO_APOTHEM;

	float sideLen = (nodeOffset / 2 * HEX_APOTHEM_TO_RAD);
	int indexY = ((position->y - startPosition.y + sideLen) / boxHeight);

	//check if odd
	bool evenY = (indexY % 2) == 0;
	int indexX = evenY ? ((position->x - startPosition.x ) / boxWidth) : ((position->x - startPosition.x + boxHalfWidth) / boxWidth);

 	float relativeY = (position->y - startPosition.y + sideLen) - (indexY * boxHeight);
	float relativeX = evenY ? (position->x - startPosition.x) - (indexX * boxWidth): (position->x - startPosition.x) - (indexX * boxWidth) + boxHalfWidth;
	const float m = 2 * HEX_RAD_TO_APOTHEM - HEX_APOTHEM_TO_RAD;
	float c = m * boxHalfWidth;

	//if y < mx + c the point lies under the line and it is in a different hexagon then the box would suggest
	if (relativeY < (-m * relativeX) + c)
	{
		indexY--;
		if (!evenY)
			indexX--;
	}
	else if (relativeY < (m * relativeX) - c)
	{
		indexY--;
		if (evenY)
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

PathfindingNode* Pathfinder::getRandomUnblockedNode()
{
	//randomly find a node, if it is not blocked return it, if it is loop until you find a unblocked one
	do
	{
		PathfindingNode* randomNode = nodes[rand() * sizeX / RAND_MAX][rand() * sizeY / RAND_MAX];
		if (randomNode->type != PathfindingNode::Type::BLOCKED)
		{
			return randomNode;
		}
	} while (true);
}

void Pathfinder::initDraw(const char* tilingTexture, float textureScale, Color wallColor)
{
	//just sets data requiried for drawing
	if (tilingTexture == nullptr)
	{
		backgroundTexture = nullptr;
		return;
	}
	backgroundTexture = Game::getInstance().getTextureManager()->getTextureInfo(tilingTexture);
	srcRect = Rectangle{ 0,0, (float)backgroundTexture->width, (float)backgroundTexture->height };
	destRect = Rectangle{ startPosition.x, startPosition.y, ((sizeX -1)* nodeOffset), (sizeY -1)* HEX_RAD_TO_APOTHEM * nodeOffset };
	texScale = textureScale;
	this->wallColor = wallColor;
}

void Pathfinder::drawBack()
{
	//draw tiled texture in background
	if (backgroundTexture != nullptr)
	{
		DrawTextureTiled(*backgroundTexture, srcRect, destRect, { 0,0 }, 0, texScale, WHITE);
	}
}

void Pathfinder::drawWalls()
{
	//draw bounding walls
	if (backgroundTexture != nullptr)
	{
		//made out of a bunch of rectangles in various positions
		unsigned char c = 30;
		
		static Color darkerColor = Color{ (unsigned char)(wallColor.r - c), (unsigned char)(wallColor.g - c), (unsigned char)(wallColor.b - c), wallColor.a };
		static Vector2 edgeAddition = Vector2(nodeOffset / 2 * 1.03f, nodeOffset / 2 * HEX_APOTHEM_TO_RAD * 1.03f);

		//top
		DrawRectangle(destRect.x - WALL_HEIGHT + edgeAddition.x * 2, destRect.y - WALL_HEIGHT + edgeAddition.y, destRect.width + WALL_HEIGHT * 2 - edgeAddition.x * 3, WALL_HEIGHT, wallColor);
		//left
		DrawRectangle(destRect.x - WALL_HEIGHT + edgeAddition.x * 2, destRect.y + edgeAddition.y, WALL_HEIGHT, destRect.height - edgeAddition.y * 2, wallColor);
		//right
		DrawRectangle(destRect.x + destRect.width - edgeAddition.x, destRect.y + edgeAddition.y, WALL_HEIGHT, destRect.height - edgeAddition.y * 2, wallColor);
		//down
		DrawRectangle(destRect.x - WALL_HEIGHT + edgeAddition.x * 2, destRect.y + destRect.height - edgeAddition.y, destRect.width + WALL_HEIGHT * 2 - edgeAddition.x * 3, WALL_HEIGHT, wallColor);
		//lines
		DrawLineEx({ destRect.x + edgeAddition.x * 2, destRect.y + edgeAddition.y}, { destRect.x - WALL_HEIGHT + edgeAddition.x * 2, destRect.y - WALL_HEIGHT + edgeAddition.y}, LINE_THICKNESS, darkerColor);
		DrawLineEx({ destRect.x + destRect.width - edgeAddition.x, destRect.y + edgeAddition.y }, { destRect.x + destRect.width + WALL_HEIGHT - edgeAddition.x, destRect.y - WALL_HEIGHT + edgeAddition.y }, LINE_THICKNESS, darkerColor);
		DrawLineEx({ destRect.x + edgeAddition.x * 2, destRect.y + destRect.height - edgeAddition.y}, { destRect.x - WALL_HEIGHT + edgeAddition.x * 2, destRect.y + destRect.height + WALL_HEIGHT - edgeAddition.y }, LINE_THICKNESS, darkerColor);
		DrawLineEx({ destRect.x + destRect.width - edgeAddition.x, destRect.y + destRect.height - edgeAddition.y }, { destRect.x + destRect.width + WALL_HEIGHT - edgeAddition.x, destRect.y + destRect.height + WALL_HEIGHT - edgeAddition.y }, LINE_THICKNESS, darkerColor);

		//wall top rectangles
		DrawRectangle(destRect.x - WALL_HEIGHT - WALL_THICKNESS + edgeAddition.x * 2, destRect.y - WALL_HEIGHT + edgeAddition.y, WALL_THICKNESS, destRect.height + WALL_HEIGHT * 2 - edgeAddition.y, darkerColor);
		DrawRectangle(destRect.x + WALL_HEIGHT + destRect.width - edgeAddition.x, destRect.y - WALL_HEIGHT - WALL_THICKNESS + edgeAddition.y, WALL_THICKNESS, destRect.height + WALL_THICKNESS * 2 + WALL_HEIGHT * 2 - edgeAddition.y * 2, darkerColor);
		DrawRectangle(destRect.x - WALL_HEIGHT - WALL_THICKNESS + edgeAddition.x * 2, destRect.y - WALL_HEIGHT - WALL_THICKNESS + edgeAddition.y, destRect.width + WALL_THICKNESS * 2 + WALL_HEIGHT * 2 - edgeAddition.x * 3, WALL_THICKNESS, darkerColor);
		DrawRectangle(destRect.x - WALL_HEIGHT - WALL_THICKNESS + edgeAddition.x * 2, destRect.y + destRect.height + WALL_HEIGHT - edgeAddition.y, destRect.width + WALL_THICKNESS * 2 + WALL_HEIGHT * 2 - edgeAddition.x * 3, WALL_THICKNESS, darkerColor);
		
	}
}

#ifdef DRAW_DEBUG
void Pathfinder::drawDebug()
{
	Color regular = { 130,130,130,0x60 };

	for (int x = 0; x < sizeX; x++)
	{
		for (int y = 0; y < sizeY; y++)
		{
			//draw square grid (used for getting which hexagon we're in)
			DrawRectangleLinesEx(Rectangle{ nodes[x][y]->position.x - (nodeOffset / 2),nodes[x][y]->position.y - (nodeOffset / 2 * HEX_APOTHEM_TO_RAD), nodeOffset,nodeOffset * HEX_RAD_TO_APOTHEM }, 4, regular);
			
			//draw lines
			for (int i = 0; i < 6; i++)
			{
				if (nodes[x][y]->neighbors[i])
					DrawLineEx(nodes[x][y]->neighbors[i]->position, nodes[x][y]->position, 5, regular);
			}
	
		}
	}
}
#endif

int Pathfinder::getHeuristic(PathfindingNode* node, PathfindingNode* end)
{
	//excellent resource for hex grid:
	//http://www-cs-students.stanford.edu/~amitp/Articles/HexLOS.html
	//gives exact heuristic value

	int dx = end->indexX - node->indexX;
	int dy = end->indexY - node->indexY;

	//if signs are the same
	if (((dx > 0) - (dx < 0)) == ((dy > 0) - (dy < 0)))
		return std::max(abs(dx), abs(dy));
	else
		return abs(dx) + abs(dy);

	return (int)PathfindingNode::Type::REGULAR * (dx + dy);
}

int Pathfinder::AStarPath(PathfindingNode* start, PathfindingNode* target, std::vector<Vector2>* finalPath)
{
	//if any of these return true there is no path to the end	
	if (start == nullptr || target == nullptr || target->type == PathfindingNode::Type::BLOCKED || start == target)
		return 0;

	//reset variables
	openList.clear();
	memset(closedList, 0, sizeof(bool) * length);
	start->gScore = 0;
	start->previous = nullptr;

	//start with the start node
	openList.push(start);
	while (openList.size() > 0)
	{
		//take node off heap and add to closed list
		PathfindingNode* current = openList.pop();
		closedList[current->indexX + current->indexY * sizeX] = true;

		//if current is equal to the target goal the pathfinding was a success
		if (current == target)
		{
			//set final path to the path to get here
			finalPath->clear();
			finalPath->push_back(current->position);
			static int pathLength = 0;
			pathLength = current->gScore;
			while (current->previous != nullptr)
			{
				finalPath->push_back( current->position );
				current = current->previous;
			}
			//return the cost of the path
			return pathLength;
		}

		//iterate through all neighbors
		for (int n = 0; n < 6; n++)
		{
			PathfindingNode* neighbor = current->neighbors[n];

			//if any of these are true than do not check it
			if (neighbor == nullptr || neighbor->type == PathfindingNode::Type::BLOCKED || closedList[neighbor->indexX + neighbor->indexY * sizeX])
				continue;
			
			int newScore = current->gScore + (int)current->type;
			if (openList.contains(neighbor))
			{
				//if neighbor has been added to list before check if the new score is more then the old
				if (neighbor->gScore > newScore)
				{	
					//if so change values an update node position in the heap
					neighbor->previous = current;
					neighbor->gScore = newScore;
					neighbor->fScore = neighbor->gScore + getHeuristic(neighbor, target);
					openList.updateNode(neighbor);
				}
			}
			else
			{
				//if not in open list add to open list and set values
				neighbor->previous = current;
				neighbor->gScore = newScore;
				neighbor->fScore = neighbor->gScore + getHeuristic(neighbor, target);
				openList.push(neighbor);
			}
		}
	}
	return 0;
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

	//Rectangle bounds
	Vector2 vertices[4];
	vertices[0] = { (nodes[0][1]->position.x + nodeOffset) / PHYSICS_UNIT_SCALE, -(nodes[0][0]->position.y + nodeOffset / 2 * HEX_APOTHEM_TO_RAD) / PHYSICS_UNIT_SCALE };
	vertices[1] = { (nodes[sizeX - 1][1]->position.x -nodeOffset / 2) / PHYSICS_UNIT_SCALE, -(nodes[0][0]->position.y + nodeOffset / 2 * HEX_APOTHEM_TO_RAD) / PHYSICS_UNIT_SCALE };
	vertices[2] = { (nodes[sizeX - 1][1]->position.x - nodeOffset / 2) / PHYSICS_UNIT_SCALE, (nodes[0][sizeY - 1]->position.y - nodeOffset / 2 * HEX_APOTHEM_TO_RAD) / -PHYSICS_UNIT_SCALE };
	vertices[3] = { (nodes[0][1]->position.x + nodeOffset) / PHYSICS_UNIT_SCALE, (nodes[0][sizeY - 1]->position.y - nodeOffset / 2 * HEX_APOTHEM_TO_RAD) / -PHYSICS_UNIT_SCALE };
	this->bounds = Rectangle{ vertices[0].x, vertices[0].y, vertices[2].x - vertices[0].x, vertices[2].y - vertices[0].y };
	bounds.CreateLoop((b2Vec2*)vertices, 4);

	////hexagon bounds
	//float apothem = sizeX * nodeOffset/2/PHYSICS_UNIT_SCALE;
	//float radius = apothem * HEX_APOTHEM_TO_RAD;
	//float posX = startPosition.x / PHYSICS_UNIT_SCALE; //leftmost point X
	//float posY = -radius / 2 - (nodeOffset / 4 * -HEX_APOTHEM_TO_RAD + startPosition.y) / PHYSICS_UNIT_SCALE; //centre point Y (I admit it's messed up, its just easier this way)

	//Vector2 vertices[6] = { 
	//						{posX, posY - radius / 2},
	//						{posX, posY + radius / 2} ,
	//						{posX + apothem, posY + radius} ,
	//						{posX + apothem * 2, posY + radius / 2} ,
	//						{posX + apothem * 2, posY - radius / 2},
	//						{posX + apothem, posY - radius}
	//					  };
	//bounds.CreateLoop((b2Vec2*)vertices, 6);
	
	fDef.shape = &bounds;

	b2Body* boundsBody = collision->getWorld()->CreateBody(&bDef);
	boundsBody->CreateFixture(&fDef);
}

Rectangle Pathfinder::getBounds()
{
	//get the bounds of the total shape including bounding walls
	static Vector2 edgeAddition = Vector2(nodeOffset / 2 * 1.03f, nodeOffset / 2 * HEX_APOTHEM_TO_RAD * 1.03f);
	Vector2 topLeft = Vector2(destRect.x - WALL_HEIGHT - WALL_THICKNESS + edgeAddition.x * 2, destRect.y - WALL_HEIGHT - WALL_THICKNESS + edgeAddition.y);
	return { topLeft.x, topLeft.y, destRect.width + WALL_THICKNESS * 2 + WALL_HEIGHT * 2 - edgeAddition.x * 3, destRect.height + WALL_THICKNESS * 2 + WALL_HEIGHT * 2 - edgeAddition.y * 2 };
}

void Pathfinder::generateWalls(CollisionManager* collision, const char* textureName, Scene* scene)
{
	//generate wall gameObjects for every blocked node
	Texture2D* texture = Game::getInstance().getTextureManager()->getTextureInfo(textureName);
	float radius = (nodeOffset / 2 * HEX_APOTHEM_TO_RAD) / PHYSICS_UNIT_SCALE;
	float apothem = nodeOffset / 2 / PHYSICS_UNIT_SCALE;
	//scale has to match with node size
	float textureScale = nodeOffset/texture->width * 1.03f;
	GameObject* wall;
	//set physics settings
	RigidBodyComponent* rigidBody;
	b2BodyDef bDef = RigidBodyComponent::genBodyDef(b2_staticBody);
	b2FixtureDef fDef = RigidBodyComponent::genFixtureDef(RigidBodyComponent::WALL, RigidBodyComponent::ALL);
	b2PolygonShape shape;
	b2Vec2 polygonVertices[6] = {	{0, radius},
									{-apothem, radius/2},		{apothem, radius/2},
									{-apothem, -radius/2},	{apothem, -radius/2},
									{0, -radius} };
	shape.Set(polygonVertices, 6);
	fDef.shape = &shape;

	for (int x = 0; x < sizeX; x++)
	{
		for (int y = 0; y < sizeY; y++)
		{
			switch (nodes[x][y]->type)
			{
			case PathfindingNode::Type::BLOCKED:
			{
				//create walls
				wall = new GameObject(scene, texture, nodes[x][y]->position, 0, textureScale, SORTING::BACKGROUND);
				bDef.position = wall->getTransform()->getPosition();
				rigidBody = wall->addComponent<RigidBodyComponent>();
				rigidBody->init(collision, bDef, fDef);
			}
			break;
			}
		}
	}
}
