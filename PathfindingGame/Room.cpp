#include "Room.h"

#include "RoomManager.h"
#include "Game.h"
#include "GameObject.h"
#include "PathfinderComponent.h"
#include "AgentComponent.h"
#include "AgentDataComponent.h"
#include "PlayerComponent.h"
#include "DoorComponent.h"
#include "TrapDoorComponent.h"
#include "PickupComponent.h"

void Room::load(RoomData* properties, RoomManager* manager, PlayerComponent* player, char enteredFrom)
{
	//seed rand()
	srand(properties->randomSeed);

	//set all variables and initialize stuff
	camera = new SmoothCamera({ 0,0 }, 0, 1, { 0 }, 10);
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
	if (player == nullptr) //<- then this is the first room
	{
		//create the player
		auto target = new GameObject(this, "beearBody", true, { 0, 0 }, 0, 1, SORTING::FOREGROUND);
		this->player = target->addComponent<PlayerComponent>();
		this->player->init(this);
		player = this->player;
	}
	else
	{
		//retrieve the player and regenerate UI
		this->player = player;
		addGameObject(player->getGameObject(), SORTING::FOREGROUND);
		player->generateAdditional(this);
	}

	//get info necessary for door creation && room generation
	PathfindingNode*** nodes = nullptr;
	AgentDataComponent* blackboard = nullptr;
	bool doorsOpen = true;
	int xSize;
	int ySize;
	
	switch (properties->type)
	{
	case RoomType::SPECIAL:
		{
			//Special room contains a pickup (only heart because of time) and is small
			pathfinderObject->init(this, 7, 6);
			Rectangle rect = pathfinderObject->getPathfinder()->getBounds();
			(new GameObject(this, "heart", true, {rect.x + rect.width / 2, rect.y + rect.height / 2}, 0, 3.0f))->addComponent<PickupComponent>()->init(this);
		}
		break;
	case RoomType::EXIT:
		{
			//exit room contains a trap door (no boss because of time) and is small
			pathfinderObject->init(this, 7, 6);
			Rectangle rect = pathfinderObject->getPathfinder()->getBounds();
			(new GameObject(this, "trapDoor", true, { rect.x + rect.width / 2, rect.y + rect.height / 2 }, 0, 0.5f))->addComponent<TrapDoorComponent>()->init(this);
		}
		break;
	case RoomType::START:
		//start room contains nothing and is small
		pathfinderObject->init(this, 7, 6);
		break;
	default:
		{
			//default rooms are bigger and contain walls and bees
			pathfinderObject->init(this);
			nodes = pathfinderObject->getPathfinder()->getNodes();
			xSize = pathfinderObject->getPathfinder()->getSizeX();
			ySize = pathfinderObject->getPathfinder()->getSizeY();
			//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
			// 	   Create Walls
			//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
			//walls do not generate at the edges so that there is always a way around
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
			//if this room was previously explored do not generate actors
			if (properties->explored)
				agentCount = 0;
			else //generate more actors on further down floors
				agentCount = rand() % (5 + player->getFloor() * 2);

			//doors initiate close if there are any actors at all
			if (agentCount > 0)
				doorsOpen = false;

			//create actors at a random node
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

	//set the bounds of the camera
	Pathfinder* pathfinder = pathfinderObject->getPathfinder();
	Rectangle cameraBounds = pathfinder->getBounds();
	cameraBounds = { cameraBounds.x + GetScreenWidth() , cameraBounds.y + GetScreenHeight() , cameraBounds.width - GetScreenWidth() * 2, cameraBounds.height - GetScreenHeight() * 2};
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
		Vector2 position = { rect.x + rect.width/2, nodes[xSize / 2][0]->position.y + doorOffset };
		(new GameObject(this, "door", true, position, 0, doorScale))->addComponent<DoorComponent>()->init(this, NORTH, doorsOpen, blackboard, enteredFrom);
	}
	if ((properties->doorState & SOUTH) != 0)
	{
		Vector2 position = { rect.x + rect.width / 2 , nodes[xSize / 2][ySize - 1]->position.y - doorOffset };
		(new GameObject(this, "door", true, position, PI, doorScale))->addComponent<DoorComponent>()->init(this, SOUTH, doorsOpen, blackboard, enteredFrom);
	}
	if ((properties->doorState & EAST) != 0)
	{
		Vector2 position = { nodes[xSize - 1][0]->position.x, rect.y + rect.height / 2 };
		position.x -= doorOffset * 0.7f + pathfinder->getNodeOffset()/2 * ((ySize / 2) % 2 != 0);
		(new GameObject(this, "door", true, position, PI / 2, doorScale))->addComponent<DoorComponent>()->init(this, EAST, doorsOpen, blackboard, enteredFrom);
	}
	if ((properties->doorState & WEST) != 0)
	{
		Vector2 position = { nodes[0][0]->position.x, rect.y + rect.height / 2 };
		position.x += doorOffset * 0.7f + pathfinder->getNodeOffset() / 2 * ((ySize / 2) % 2 == 0);
		(new GameObject(this, "door", true, position, 3 * PI / 2, doorScale))->addComponent<DoorComponent>()->init(this, WEST, doorsOpen, blackboard, enteredFrom);
	}
	//now generate physical walls from node data
	pathfinderObject->getPathfinder()->generateWalls(collisionManager, "wall", this);
	//mark this room as explored
	properties->explored = true;
	backgroundColor = BLACK;
}

void Room::draw()
{
	ClearBackground(backgroundColor);

	camera->startCamera();

	//draw the background tiles
	pathfinderObject->drawBackground();
	for (int i = 0; i < (int)SORTING::COUNT - 1; ++i)
	{
		if (i == (int)SORTING::MIDGROUND) 
		{
			//draw walls after drawing background
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
	pathfinderObject->getPathfinder()->drawDebug();
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

	camera->endCamera();
	for (auto gameObject : sortingLayers[(int)SORTING::UI])
	{
		gameObject->draw();
	}

#ifdef DRAW_DEBUG
	DrawFPS(0, 0);
#endif // DRAW_DEBUG
}

void Room::start()
{
	//call the last start
	Scene::start();
	//restrict camera to bounds
	camera->restrictToBounds();

	//then do the transition effect
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//check if no transition should be used
	if (enteredFrom == 0 && !roomDropTransition)
	{
		return;
	}
	//check if should use the drop transition
	if (roomDropTransition)
	{
		//update once to ensure everything is in the right place
		update();
		//set camera to player position
		camera->getCamera()->target = player->getTransform()->getPosition();
		camera->restrictToBounds();
		//then capture a frame to the 2nd render texture
		BeginTextureMode(*roomManager->getTransitionTexture2());
		//unfortunately we can't just call the draw function because the UI have to be drawn seperately
		//MINI DRAW CALL
		ClearBackground(backgroundColor);

			camera->startCamera();
			pathfinderObject->drawBackground();
			for (int i = 0; i < (int)SORTING::COUNT - 2; ++i)
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

			camera->endCamera();
		EndTextureMode();

		//now transition between the two screens
		float transitionTime = 0;
		//have to use DrawTexturePro as the images need to be flipped on the Y axis for some reasion
		Rectangle srcRect = { 0,0, (float)GetScreenWidth() , -(float)GetScreenHeight() };
		//freeze time temporarily
		Game::getInstance().setTimeScale(0);

		while (transitionTime < 1)
		{
			std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();

			if (transitionTime > 0)
			{
				float x = transitionTime * transitionTime * transitionTime;
				BeginDrawing();
				ClearBackground(BLACK);
				float scale = x;
				Rectangle destRect = { 0,0, GetScreenWidth() * scale, GetScreenHeight() * scale };
				destRect.x += GetScreenWidth()  - destRect.width  /2;
				destRect.y += GetScreenHeight()  - destRect.height / 2;
				DrawTexturePro(roomManager->getTransitionTexture2()->texture, srcRect, destRect, { GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f }, 0, { 0xFF,0xFF,0xFF,(unsigned char)(0xFF * (x)) });
				scale = 1 + x * 5;
				destRect = { 0,0, GetScreenWidth() * scale, GetScreenHeight() * scale };
				destRect.x += GetScreenWidth() - destRect.width / 2;
				destRect.y += GetScreenHeight() - destRect.height / 2;
				DrawTexturePro(roomManager->getTransitionTexture1()->texture, srcRect, destRect, { GetScreenWidth() / 2.0f, GetScreenHeight()/2.0f }, 0, { 0xFF,0xFF,0xFF, (unsigned char)(std::fmax(0, 0xFF * (1 - transitionTime * 4))) });
				
				camera->startCamera();
				for (auto gameObject : sortingLayers[(int)SORTING::FOREGROUND])
				{
					gameObject->draw();
				}
				camera->endCamera();
				EndDrawing();
			}

			std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();
			//delta time is calculated
			transitionTime += (float)std::chrono::duration_cast<std::chrono::duration<double>>(end - start).count() / 0.5f;
		}
		Game::getInstance().setTimeScale(1);
		roomDropTransition = false;
	}
	else
	{
		//do room switch transition
		//simulate a game loop (update) (deltaTime will be the same as it was for the last frame, the time this takes will not impact deltaTime)
		update();
		//then capture a frame to the 2nd render texture
		BeginTextureMode(*roomManager->getTransitionTexture2());
		//unfortunately we can't just call the draw function because the UI has to be drawn seperately
		//MINI DRAW CALL
			ClearBackground(backgroundColor);

			camera->startCamera();

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

			camera->endCamera();
		EndTextureMode();

		//temporarily pause time (for UI rendering as sprite animation is tied to deltaTime)
		Game::getInstance().setTimeScale(0);

		// now can transition between captured frames
		//negative represents time before transitioning, which makes it less confusing what is happening
		float transitionTime = -0.05f;

		//set variables
		Vector2i direction = { (EAST == enteredFrom) - (WEST == enteredFrom), (SOUTH == enteredFrom) - (NORTH == enteredFrom) };
		Color c = { PathfinderComponent::BACKGROUND_COLOR.r - 30, PathfinderComponent::BACKGROUND_COLOR.g - 30 , PathfinderComponent::BACKGROUND_COLOR.b - 30 , 0xFF };
		Rectangle srcRect = { 0,0, (float)GetScreenWidth() , -(float)GetScreenHeight() };
		Rectangle destRect = { 0,0, (float)GetScreenWidth() , (float)GetScreenHeight() };
		//transition between rooms
		while (transitionTime < 1)
		{
			std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();

			if (transitionTime > 0)
			{
				Vector2 movement = { direction.x * (1 - (1 - transitionTime) * (1 - transitionTime)), direction.y * (1 - (1 - transitionTime) * (1 - transitionTime)) };

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
		//reset time scale
		Game::getInstance().setTimeScale(1);
	}
	
}

void Room::unload()
{
	Scene::unload();
}

void Room::beforeDelete(Scene* nextScene)
{
	if (nextScene != nullptr)
	{
		//if the next scene is a room then a transition will be played
		if (typeid(*nextScene) == typeid(Room))
		{
			//create transition frame
			BeginTextureMode(*roomManager->getTransitionTexture1());
			//MINI DRAW CALL
				ClearBackground(backgroundColor);

				camera->startCamera();

				pathfinderObject->drawBackground();
				//if doing the room drop transition then don't render the player to the texture (player is on SORTING::FOREGROUND)
				for (int i = 0; i < (int)SORTING::COUNT - 1 - roomDropTransition; ++i)
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

				camera->endCamera();
			EndTextureMode();

			//remove the player from children so that it doesn't get deleted when this scene does
			removeGameObjectFromChildren(player->getGameObject());
		}
	}
}

Pathfinder* Room::getPathfinder()
{
	return pathfinderObject->getPathfinder();
}
