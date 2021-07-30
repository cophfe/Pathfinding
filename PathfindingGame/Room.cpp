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

	this->enteredFrom = enteredFrom;
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
	Rectangle cameraBounds = pathfinder->getBounds();
	cameraBounds = { cameraBounds.x + GetScreenWidth() , cameraBounds.y + GetScreenHeight() , cameraBounds.width - GetScreenWidth() * 2, cameraBounds.height - GetScreenHeight() * 2};
//	cameraBounds = { -200000000, -2000000000, 200000000, 20000000000 };
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//std::cout << GetScreenWidth() << ", " << GetScreenHeight() << '\n';
	camera->setBounds(cameraBounds);
	nodes = pathfinder->getNodes();
	xSize = pathfinder->getSizeX();
	ySize = pathfinder->getSizeY();
	//now create doors
	float doorScale = pathfinder->getNodeOffset() / Game::getInstance().getTextureManager()->getTextureInfo("wall")->width * 1.03f;
	static int doorOffset = 70;
	Rectangle rect = pathfinder->getBounds();

	if ((properties->doorState & NORTH) != 0)
	{
		//nodes[xSize / 2][0]->type = PathfindingNode::Type::REGULAR;
		Vector2 position = { rect.x + rect.width/2, nodes[xSize / 2][0]->position.y + doorOffset };
		(new GameObject(this, "door", true, position, 0, doorScale))->addComponent<DoorComponent>()->init(this, NORTH, doorsOpen, blackboard, enteredFrom);
	}
	if ((properties->doorState & SOUTH) != 0)
	{
		//nodes[xSize / 2][ySize]->type = PathfindingNode::Type::REGULAR;
		Vector2 position = { rect.x + rect.width / 2 , nodes[xSize / 2][ySize - 1]->position.y - doorOffset };
		(new GameObject(this, "door", true, position, PI, doorScale))->addComponent<DoorComponent>()->init(this, SOUTH, doorsOpen, blackboard, enteredFrom);
	}
	if ((properties->doorState & EAST) != 0)
	{
		//nodes[xSize][ySize/2]->type = PathfindingNode::Type::REGULAR;
		Vector2 position = { nodes[xSize - 1][0]->position.x, rect.y + rect.height / 2 };
		position.x -= doorOffset * 0.7f + pathfinder->getNodeOffset()/2 * ((ySize / 2) % 2 != 0);
		(new GameObject(this, "door", true, position, PI / 2, doorScale))->addComponent<DoorComponent>()->init(this, EAST, doorsOpen, blackboard, enteredFrom);
	}
	if ((properties->doorState & WEST) != 0)
	{
		//nodes[0][ySize/2]->type = PathfindingNode::Type::REGULAR;
		Vector2 position = { nodes[0][0]->position.x, rect.y + rect.height / 2 };
		position.x += doorOffset * 0.7f + pathfinder->getNodeOffset() / 2 * ((ySize / 2) % 2 == 0);
		(new GameObject(this, "door", true, position, 3 * PI / 2, doorScale))->addComponent<DoorComponent>()->init(this, WEST, doorsOpen, blackboard, enteredFrom);
	}
	//now generate physical walls from node data
	pathfinderObject->getPathfinder()->generateWalls(collisionManager, "wall", this);
	//mark as explored
	properties->explored = true;
	backgroundColor = BLACK;
}

void Room::draw()
{
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
}

void Room::start()
{
	Scene::start();
	camera->restrictToBounds();
	
	//transition
	if (enteredFrom == 0)
		return;

	//simulate a game loop (update) (deltaTime will be the same as it was for the last frame, the time this takes will not impact deltaTime)
	fixedUpdate();
	update();
	//then capture a frame to the 2nd render texture
	BeginTextureMode(*roomManager->getTransitionTexture2());
	//unfortunately we can't just call the draw function because the UI have to be drawn seperately
	//MINI DRAW CALL
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
	
		camera->EndCamera();
	EndTextureMode();

	//temporarily pause time (for UI rendering)
	Game::getInstance().setTimeScale(0);

	// now can transition between captured frames
	//negative represents time before transitioning, which makes it less confusing what is happening
	float transitionTime = -0.2f;
	
	Vector2i direction = { (EAST == enteredFrom) - (WEST == enteredFrom), (SOUTH == enteredFrom) - (NORTH == enteredFrom) };
	Color c = { PathfinderComponent::BACKGROUND_COLOR.r - 30, PathfinderComponent::BACKGROUND_COLOR.g - 30 , PathfinderComponent::BACKGROUND_COLOR.b - 30 , 0xFF};
	
	Rectangle srcRect = { 0,0, (float)GetScreenWidth() , -(float)GetScreenHeight() };
	Rectangle destRect = { 0,0, (float)GetScreenWidth() , (float)GetScreenHeight() };
	while (transitionTime < 1)
	{
		std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();

		if (transitionTime > 0)
		{
			Vector2 movement = { direction.x * transitionTime, direction.y * transitionTime };

			BeginDrawing();

			ClearBackground(c);
			//first draw the 2 transition frames
			destRect.x = movement.x * (GetScreenWidth() + 100);
			destRect.y = movement.y * (GetScreenHeight() + 100);
			DrawTexturePro(roomManager->getTransitionTexture1()->texture, srcRect, destRect, { 0,0 }, 0, WHITE);
			destRect.x = movement.x * (GetScreenWidth() + 100) - (GetScreenWidth() + 100) * direction.x;
			destRect.y = movement.y * (GetScreenHeight() + 100) - (GetScreenHeight() + 100) * direction.y;
			DrawTexturePro(roomManager->getTransitionTexture2()->texture, srcRect, destRect, { 0,0 }, 0, WHITE);

			//then draw UI on top
			for (auto gameObject : sortingLayers[(int)SORTING::UI])
			{
				gameObject->draw();
			}
			EndDrawing();
		}

		std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();
		transitionTime += (float)std::chrono::duration_cast<std::chrono::duration<double>>(end - start).count() / 0.2f;
	}

	Game::getInstance().setTimeScale(1);
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
			//create transition frame
			BeginTextureMode(*roomManager->getTransitionTexture1());
			//MINI DRAW CALL
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

				camera->EndCamera();
			EndTextureMode();

			removeGameObjectFromChildren(player->getGameObject());
		}
	}
}

Pathfinder* Room::getPathfinder()
{
	return pathfinderObject->getPathfinder();
}
