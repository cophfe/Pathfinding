#include "Room.h"

#include "RoomManager.h"
#include "Game.h"
#include "GameObject.h"
#include "PathfinderComponent.h"
#include "AgentComponent.h"
#include "AgentDataComponent.h"
#include "PlayerComponent.h"
#include "DoorComponent.h"

void Room::load(RoomData* properties, RoomManager* manager, PlayerComponent* player, char enteredFrom)
{
	srand(properties->randomSeed);

	camera = new SmoothCamera({ 0,0 }, 0, 1, { 0 }, 10);
	GameObject::resetIdCounter();

	collisionManager = new CollisionManager();
	roomManager = manager;
	roomCoordinates = properties->coord;
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// 	   Create Pathfinder
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	pathfinderObject = (new GameObject(this, nullptr, false, { 0 }, 1, SORTING::BACKGROUND))->addComponent<PathfinderComponent>();
	
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// 	   Create Player
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	if (player == nullptr)
	{
		auto target = new GameObject(this, "beearBody", true, { 0, 0 }, 0, 1, SORTING::FOREGROUND);
		this->player = target->addComponent<PlayerComponent>();
		this->player->init(this);
		player = this->player;
	}
	else
	{
		this->player = player;
		addGameObject(player->getGameObject(), SORTING::FOREGROUND);
		player->generateAdditional(this);
	}

	PathfindingNode*** nodes = nullptr;
	AgentDataComponent* blackboard = nullptr;
	bool doorsOpen = true;
	int xSize;
	int ySize;
	
	switch (properties->type)
	{
	case RoomType::SPECIAL:
		pathfinderObject->init(this, 7, 6);
		break;
	case RoomType::BOSS:
		pathfinderObject->init(this, 7, 6);
		break;
	case RoomType::START:
		pathfinderObject->init(this, 7, 6);
		break;
	default:
		{
			pathfinderObject->init(this);
			nodes = pathfinderObject->getPathfinder()->getNodes();
			xSize = pathfinderObject->getPathfinder()->getSizeX();
			ySize = pathfinderObject->getPathfinder()->getSizeY();
			//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
			// 	   Create Wall
			//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
			for (int x = 2; x < xSize - 2; x++)
			{
				for (int y = 2; y < ySize - 2; y++)
				{
					if (rand() < RAND_MAX / 4)
					{
						nodes[x][y]->type = PathfindingNode::Type::BLOCKED;
					}
				}
			}
			//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
			// 	   Initialize Actors
			//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
			///actor blackboard is made
			blackboard = (new GameObject(this, nullptr, false, { 0 }, 0, 1, SORTING::BACKGROUND))->addComponent<AgentDataComponent>();
			blackboard->init(this->player->getGameObject(), pathfinderObject->getPathfinder());

			//then actors
			int agentCount;
			if (properties->explored)
				agentCount = 0;
			else
				agentCount = rand() * 8 / RAND_MAX;

			if (agentCount > 0)
			{
				doorsOpen = false;
			}
			while (agentCount > 0)
			{
				int agentXPos = rand() * (xSize - 2) / RAND_MAX + 2;
				int agentYPos = rand() * (ySize - 2) / RAND_MAX + 2;
				if (nodes[agentXPos][agentYPos]->type == PathfindingNode::Type::BLOCKED)
				{
					continue;
				}
				(new GameObject(this, "beeBody", true, nodes[agentXPos][agentYPos]->position, 0, 1.0f))->addComponent<AgentComponent>()->init(blackboard, collisionManager);
				agentCount--;
			}
		}
		break;
	}

	Pathfinder* pathfinder = pathfinderObject->getPathfinder();
	Rectangle bounds = pathfinder->getBounds();
	bounds = { bounds.x + GetScreenWidth()/2 * camera->getScale() , bounds.y + GetScreenHeight()/2 * camera->getScale(), bounds.width, bounds.height};
	//std::cout << GetScreenWidth() << ", " << GetScreenHeight() << '\n';
	camera->setBounds(bounds);
	nodes = pathfinder->getNodes();
	xSize = pathfinder->getSizeX();
	ySize = pathfinder->getSizeY();
	//now create doors
	float doorScale = pathfinder->getNodeOffset() / Game::getInstance().getTextureManager()->getTextureInfo("wall")->width * 1.03f;
	static int doorOffset = 70;
	if ((properties->doorState & NORTH) != 0)
	{
		//nodes[xSize / 2][0]->type = PathfindingNode::Type::REGULAR;
		Vector2 position = { nodes[0][0]->position.x + (xSize - 1.5f)* pathfinder->getNodeOffset() / 2 , nodes[xSize / 2][0]->position.y + doorOffset };
		(new GameObject(this, "door", true, position, 0, doorScale))->addComponent<DoorComponent>()->init(this, NORTH, doorsOpen, blackboard, enteredFrom);
	}
	if ((properties->doorState & SOUTH) != 0)
	{
		//nodes[xSize / 2][ySize]->type = PathfindingNode::Type::REGULAR;
		Vector2 position = { nodes[0][0]->position.x + ((float)xSize - 1.5f)* pathfinder->getNodeOffset() / 2 , nodes[xSize / 2][ySize - 1]->position.y - doorOffset };
		(new GameObject(this, "door", true, position, PI, doorScale))->addComponent<DoorComponent>()->init(this, SOUTH, doorsOpen, blackboard, enteredFrom);
	}
	if ((properties->doorState & EAST) != 0)
	{
		//nodes[xSize][ySize/2]->type = PathfindingNode::Type::REGULAR;
		Vector2 position = { nodes[xSize - 1][0]->position.x, nodes[0][0]->position.y + ((float)ySize - 1) * pathfinder->getNodeOffset() / 2 * HEX_APOTHEM_TO_RAD };
		position.x -= doorOffset * 0.7f + pathfinder->getNodeOffset()/2 * ((ySize / 2) % 2 != 0);
		(new GameObject(this, "door", true, position, 3*PI/2, doorScale))->addComponent<DoorComponent>()->init(this, EAST, doorsOpen, blackboard, enteredFrom);
	}
	if ((properties->doorState & WEST) != 0)
	{
		//nodes[0][ySize/2]->type = PathfindingNode::Type::REGULAR;
		Vector2 position = { nodes[0][0]->position.x, nodes[0][0]->position.y + ((float)ySize - 1.5f) * pathfinder->getNodeOffset() / 2 };
		position.x += doorOffset * 0.7f + pathfinder->getNodeOffset() / 2 * ((ySize / 2) % 2 == 0);
		(new GameObject(this, "door", true, position, PI / 2, doorScale))->addComponent<DoorComponent>()->init(this, WEST, doorsOpen, blackboard, enteredFrom);
	}
	//now generate physical walls from node data
	pathfinderObject->getPathfinder()->generateWalls(collisionManager, "wall", this);
	//mark as explored
	properties->explored = true;
	backgroundColor = BLACK;
}

void Room::draw()
{
	BeginDrawing();
	ClearBackground(backgroundColor);

	camera->StartCamera();

	pathfinderObject->drawBackground();
	for (int i = 0; i < (int)SORTING::COUNT - 1; ++i)
	{
		if (i == (int)SORTING::MIDGROUND) 
		{
			pathfinderObject->drawBoundWalls();
		}

		for (auto gameObject : sortingLayers[i])
		{
			gameObject->draw();
		}
	}
	
	//		DEBUG RENDERING
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#ifdef DRAW_DEBUG
	for (int i = 0; i < (int)SORTING::COUNT; ++i)
	{
		for (auto gameObject : sortingLayers[i])
		{
			gameObject->debugDraw();
		}
	}
	collisionManager->debugDraw();
#endif // DRAW_DEBUG
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

	camera->EndCamera();
	for (auto gameObject : sortingLayers[(int)SORTING::UI])
	{
		gameObject->draw();
	}
	DrawFPS(0, 0);
	EndDrawing();
}

void Room::unload()
{
	Scene::unload();
}

void Room::beforeDelete(Scene* nextScene)
{
	if (nextScene != nullptr)
	{
		if (typeid(*nextScene) == typeid(Room))
		{
			removeGameObjectFromChildren(player->getGameObject());
		}
	}
}

Pathfinder* Room::getPathfinder()
{
	return pathfinderObject->getPathfinder();
}
