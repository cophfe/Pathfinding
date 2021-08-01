#include "RoomManager.h"
#include "GameObject.h"
#include "PlayerComponent.h"
#include "Game.h"
#include <algorithm>

RoomManager::RoomManager(int randomSeed) : randomSeed(randomSeed)
{
	srand(randomSeed);
	miniMapTexture = LoadRenderTexture(MAP_SIZE, MAP_SIZE);
	//transition is done using these buffers
	transitionBuffer[0] = LoadRenderTexture(GetScreenWidth(), GetScreenHeight());
	transitionBuffer[1] = LoadRenderTexture(GetScreenWidth(), GetScreenHeight());
}

RoomManager::~RoomManager()
{
	UnloadRenderTexture(miniMapTexture);
	UnloadRenderTexture(transitionBuffer[0]);
	UnloadRenderTexture(transitionBuffer[1]);
}

void RoomManager::generateMap()
{
	mapCreatedBefore = true;
	currentRoom = GET_COORD(0, 0);
	generateRoom(currentRoom);
	rooms[currentRoom].type = RoomType::START;
	startRoom = currentRoom;
	std::vector<std::pair<RoomCoord, char>> specialRoomOptions;

	//this is pretty slow but it happens once so I think it's fine.
	//retrieve all of the options for special room coordinates
	char neighborDoors;
	for (auto& room : rooms)
	{
		//this is guaranteed to return true a bunch of times
		RoomCoord coord = GET_COORD(room.first.first + 1, room.first.second);
		if (rooms.find(coord) == rooms.end() && getCoordNeighborCount(coord, &neighborDoors) == 1)
			specialRoomOptions.push_back(std::make_pair(coord, neighborDoors));
		coord = GET_COORD(room.first.first - 1, room.first.second);
		if (rooms.find(coord) == rooms.end() && getCoordNeighborCount(coord, &neighborDoors) == 1)
			specialRoomOptions.push_back(std::make_pair(coord, neighborDoors));
		coord = GET_COORD(room.first.first, room.first.second + 1);
		if (rooms.find(coord) == rooms.end() && getCoordNeighborCount(coord, &neighborDoors) == 1)
			specialRoomOptions.push_back(std::make_pair(coord, neighborDoors));
		coord = GET_COORD(room.first.first, room.first.second - 1);
		if (rooms.find(coord) == rooms.end() && getCoordNeighborCount(coord, &neighborDoors) == 1)
			specialRoomOptions.push_back(std::make_pair(coord, neighborDoors));
	}

	//exit room is the one furthest away from the middle
	int exitIndex = 0;
	float maxDist = 0;
	//get the one furthest away
	for (int i = 0; i < specialRoomOptions.size(); i++)
	{
		float dist = specialRoomOptions[i].first.first * specialRoomOptions[i].first.first + specialRoomOptions[i].first.second * specialRoomOptions[i].first.second;
		if (dist > maxDist)
		{
			maxDist = dist;
			exitIndex = i;
		}
	}
	//generate exit room there
	bossRoom = specialRoomOptions[exitIndex].first;
	rooms[bossRoom] = RoomData(specialRoomOptions[exitIndex].second, RoomType::EXIT, bossRoom, randomSeed + rooms.size() * 277);
	//delete it from special room options array
	specialRoomOptions.erase(specialRoomOptions.begin() + exitIndex);
	
	//special room is a random one of these coordinates
	int specialRoomIndex = -1;
	while(specialRoomOptions.size() > 0)
	{
		specialRoomIndex = rand() * ((float)specialRoomOptions.size() / RAND_MAX);

		//since we just modified the rooms when we added the exit room we should recheck if it is acceptable
		if (getCoordNeighborCount(specialRoomOptions[specialRoomIndex].first, nullptr) == 1 && rooms.find(specialRoomOptions[specialRoomIndex].first) == rooms.end())
		{
			//if so, accept it and generate the special room
			rooms[specialRoomOptions[specialRoomIndex].first] = RoomData(specialRoomOptions[specialRoomIndex].second, RoomType::SPECIAL, specialRoomOptions[specialRoomIndex].first, randomSeed + rooms.size() * 277);
			specialRoom = specialRoomOptions[specialRoomIndex].first;
			break;
		}
		//else erase this from the list and move on
		specialRoomOptions.erase(specialRoomOptions.begin() + specialRoomIndex);
	}

	//now connect rooms together
	for (auto& room : rooms)
	{
		char doorState;
		getCoordNeighborCount(room.first, &doorState);
		room.second.doorState = doorState;
	}
}

void RoomManager::generateMiniMap()
{
	float height = ROOM_SIZE;
	float width = height * SIZE_RATIO;
	Vector2 centrePosition = { MAP_SIZE / 2 - width/2, MAP_SIZE / 2 - height /2};

	//draw a mini map to the render texture
	BeginTextureMode(miniMapTexture);
	ClearBackground({ 0,0,0,0 });
	//draw the background
	DrawRectangle(0, 0, MAP_SIZE, MAP_SIZE, MAP_COLOR);
	Color color;

	for (auto& room : rooms)
	{
		//draw the outline rectangle for all the rooms
		DrawRectangle(centrePosition.x + (width + MAP_ROOM_OFFSET * (SIZE_RATIO)) * (room.first.first - currentRoom.first) - MAP_ROOM_PADDING, centrePosition.y + (height + MAP_ROOM_OFFSET) * (room.first.second - currentRoom.second) - MAP_ROOM_PADDING, width + MAP_ROOM_PADDING * 2, height + MAP_ROOM_PADDING * 2, BLACK);
	}
	for (auto& room : rooms)
	{
		switch (room.second.type)
		{
		case RoomType::REGULAR:
			color = ROOM_COLOR;
			break;
		case RoomType::EXIT:
			color = EXIT_ROOM_COLOR;
			break;
		case RoomType::SPECIAL:
			color = ITEM_ROOM_COLOR;
			break;
		case RoomType::START:
			color = START_ROOM_COLOR;
			break;
		}
		if (!room.second.explored)
		{
			color.r -= 0x40;
			color.g -= 0x40;
			color.b -= 0x40;
		}
		//draw the room
		DrawRectangle(centrePosition.x + (width + MAP_ROOM_OFFSET * SIZE_RATIO) * (room.first.first - currentRoom.first), centrePosition.y + (height + MAP_ROOM_OFFSET) * (room.first.second - currentRoom.second), width, height, color);
	}
	//draw current room as white
	DrawRectangle(centrePosition.x, centrePosition.y, width, height, { 0xFF,0xFF,0xFF, 0xFF });
	//draw border
	DrawRectangleLinesEx({0,0, MAP_SIZE, MAP_SIZE }, LINE_THICKNESS, BLACK);
	EndTextureMode();
}

Room* RoomManager::createFirstRoom()
{
	//only difference is that the player pointer is set to nullptr
	Room* room = new Room();
	room->load(&rooms[currentRoom], this, nullptr);
	return room;
}

void RoomManager::moveToNextRoom(Room* currentRoom, char enteredFrom)
{
	//get the coordinate of the next room
	RoomCoord newCoord = GET_COORD(this->currentRoom.first - ((enteredFrom & EAST) == EAST) + ((enteredFrom & WEST) == WEST),
		this->currentRoom.second - ((enteredFrom & NORTH) == NORTH) + ((enteredFrom & SOUTH) == SOUTH));

	//check if coord is valid
	auto iter = rooms.find(newCoord);
	if (iter != rooms.end())
	{
		Room* room = new Room();
		//load new room
		PlayerComponent* player = currentRoom->getPlayerComponent();
		player->getGameObject()->getComponentOfType<RigidBodyComponent>()->disableComponent();
		this->currentRoom = newCoord;
		room->load(&(*iter).second, this, player, enteredFrom);
		//delete old room
		auto& game = Game::getInstance();
		game.switchScene(room);
	}
}

void RoomManager::generateNewMap(int newSeed, bool newFloor)
{
	//just resets all variables before generating map
	rooms.clear();
	pickUpPickedUp = false;
	randomSeed = newSeed;
	srand(randomSeed);
	roomCounter = 0;
	//if moved to the next floor, add to the max number of rooms
	if (newFloor)
		maxRooms += MAX_ROOMS_START / 2;
	generateMap();
}

bool RoomManager::generateRoom(RoomCoord coord)
{
	//cancel if a room already exists here
	if (rooms.find(coord) != rooms.end() )
	{
		return false;
	}

	//generate room data
	RoomData data;
	data.type = RoomType::REGULAR;
	data.randomSeed = randomSeed + rooms.size() * 277;
	data.coord = coord;
	data.doorState = 0;
	rooms[coord] = data;

	//rooms should be generated in a random order, not north, south, east, west
	//this should stop it from generating a room toward a specific direction
	
	RoomCoord coordinates[4] = {
		GET_COORD(coord.first, coord.second + 1),
		GET_COORD(coord.first, coord.second - 1),
		GET_COORD(coord.first + 1, coord.second),
		GET_COORD(coord.first - 1, coord.second)
	};

	std::shuffle(coordinates, coordinates + 4, std::default_random_engine(rand()));

	//randomly generate rooms
	for (char i = 0; i < 4; i++)
	{
		if (roomCounter < maxRooms - 1 && (rand() < (RAND_MAX * DOOR_CHANCE)))
		{
			roomCounter++;
			if (!generateRoom(coordinates[i]))
			{
				roomCounter--;
			}
		}
	}

	return true;
}

int RoomManager::getCoordNeighborCount(RoomCoord coord, char* neighborDoorsInfo)
{
	//check all coords around this coordinate to find which rooms are next to it
	char neighbourCount = 0;
	char neighborDoors = 0;
	if (rooms.find(GET_COORD(coord.first + 1, coord.second)) != rooms.end())
	{
		neighbourCount++;
		neighborDoors = EAST;
	}
	if (rooms.find(GET_COORD(coord.first - 1, coord.second)) != rooms.end())
	{
		neighbourCount++;
		neighborDoors |= WEST;
	}
	if (rooms.find(GET_COORD(coord.first, coord.second + 1)) != rooms.end())
	{
		neighbourCount++;
		neighborDoors |= NORTH;
	}
	if (rooms.find(GET_COORD(coord.first, coord.second - 1)) != rooms.end())
	{
		neighbourCount++;
		neighborDoors |= SOUTH;
	}

	if (neighborDoorsInfo != nullptr)
		*neighborDoorsInfo = neighborDoors;

	return neighbourCount;
}
