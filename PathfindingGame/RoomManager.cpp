#include "RoomManager.h"
#include "GameObject.h"
#include "PlayerComponent.h"
#include "Game.h"
#include <algorithm>

RoomManager::RoomManager(int randomSeed) : randomSeed(randomSeed)
{
	srand(randomSeed);
	miniMapTexture = LoadRenderTexture(mapSize, mapSize);
	generateMap();
}

void RoomManager::generateMap()
{
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
		if (getCoordNeighborCount(coord, &neighborDoors) == 1)
			specialRoomOptions.push_back(std::make_pair(coord, neighborDoors));
		coord.second -= 2;
		if (getCoordNeighborCount(coord, &neighborDoors) == 1)
			specialRoomOptions.push_back(std::make_pair(coord, neighborDoors));
		coord.second += 1;
		coord.first += 1;
		if (getCoordNeighborCount(coord, &neighborDoors) == 1)
			specialRoomOptions.push_back(std::make_pair(coord, neighborDoors));
		coord.first -= 2;
		if (getCoordNeighborCount(coord, &neighborDoors) == 1)
			specialRoomOptions.push_back(std::make_pair(coord, neighborDoors));
	}

	//boss room is the one furthest away from the middle
	int bossIndex = 0;
	float maxDist = 0;
	for (int i = 0; i < specialRoomOptions.size(); i++)
	{
		//square distance
		float dist = specialRoomOptions[i].first.first * specialRoomOptions[i].first.first + specialRoomOptions[i].first.second * specialRoomOptions[i].first.second;
		if (dist > maxDist)
		{
			maxDist = dist;
			bossIndex = i;
		}
	}
	bossRoom = specialRoomOptions[bossIndex].first;
	rooms[bossRoom] = RoomData(specialRoomOptions[bossIndex].second, RoomType::BOSS, bossRoom, randomSeed + rooms.size() * 277);
	specialRoomOptions.erase(specialRoomOptions.begin() + bossIndex);
	
	//special room is a random one of these coordinates
	int specialRoomIndex = -1;
	while(specialRoomOptions.size() > 0)
	{
		specialRoomIndex = rand() * ((float)specialRoomOptions.size() / RAND_MAX);

		//since we just modified the rooms we should recheck if it is acceptable
		if (getCoordNeighborCount(specialRoomOptions[specialRoomIndex].first, nullptr) == 1)
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
	float height = roomSize;
	float width = height * sizeRatio;
	Vector2 centrePosition = { mapSize / 2 - width/2, mapSize / 2 - height /2};

	BeginTextureMode(miniMapTexture);
	ClearBackground({ 0,0,0,0 });
	DrawRectangle(0, 0, mapSize, mapSize, mapColor);
	Color color;

	for (auto& room : rooms)
	{
		DrawRectangle(centrePosition.x + (width + offset * (sizeRatio)) * (room.first.first - currentRoom.first) - roomPadding, centrePosition.y + (height + offset) * (room.first.second - currentRoom.second) - roomPadding, width + roomPadding * 2, height + roomPadding * 2, BLACK);
	}
	for (auto& room : rooms)
	{
		switch (room.second.type)
		{
		case RoomType::REGULAR:
			color = roomColor;
			break;
		case RoomType::BOSS:
			color = bossRoomColor;
			break;
		case RoomType::SPECIAL:
			color = specialRoomColor;
			break;
		case RoomType::START:
			color = startRoomColor;
			break;
		}
		if (!room.second.explored)
		{
			color.r -= 0x40;
			color.g -= 0x40;
			color.b -= 0x40;
		}
		DrawRectangle(centrePosition.x + (width + offset * sizeRatio) * (room.first.first - currentRoom.first), centrePosition.y + (height + offset) * (room.first.second - currentRoom.second), width, height, color);
	}
	DrawRectangle(centrePosition.x, centrePosition.y, width, height, { 0xFF,0xFF,0xFF, 0xFF });
	DrawRectangleLinesEx({0,0, mapSize, mapSize }, LINE_THICKNESS, BLACK);
	EndTextureMode();
}

Room* RoomManager::createFirstRoom()
{
	Room* room = new Room();
	room->load(&rooms[currentRoom], this, nullptr);
	return room;
}

void RoomManager::moveToNextRoom(Room* currentRoom, char enteredFrom)
{
	RoomCoord newCoord = GET_COORD(this->currentRoom.first - ((enteredFrom & EAST) == EAST) + ((enteredFrom & WEST) == WEST),
		this->currentRoom.second - ((enteredFrom & NORTH) == NORTH) + ((enteredFrom & SOUTH) == SOUTH));

	//check if coord has roomData
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
	//since there is a limited amount of rooms, this should stop it from generating a room toward a specific direction
	
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
		if (roomCounter < maxRegularRooms - 1 && (rand() < (RAND_MAX * doorChance)))
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
	char neighbourCount = 0;
	char neighborDoors = 0;
	if (rooms.find(GET_COORD(coord.first + 1, coord.second)) != rooms.end())
	{
		neighbourCount = 1;
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
