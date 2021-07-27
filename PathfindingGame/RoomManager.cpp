#include "RoomManager.h"
#include <algorithm>

void RoomManager::generateMap()
{
	srand(time(0));

	generateRoom(GET_COORD(0,0), 0);
	rooms[GET_COORD(0,0)].type = RoomType::START;

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
	int bossIndex;
	float minDist= 99999999999;
	for (int i = 0; i < specialRoomOptions.size(); i++)
	{
		//square distance
		float dist = specialRoomOptions[i].first.first * specialRoomOptions[i].first.first + specialRoomOptions[i].first.second * specialRoomOptions[i].first.second;
		if (dist < minDist)
		{
			minDist = dist;
			bossIndex = i;
		}
	}
	RoomCoord bossCoord = specialRoomOptions[bossIndex].first;
	rooms[bossCoord] = RoomData(specialRoomOptions[bossIndex].second, RoomType::BOSS, bossCoord, randomSeed + rooms.size() * 277);
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
			break;
		}
		//else erase this from the list and move on
		specialRoomOptions.erase(specialRoomOptions.begin() + specialRoomIndex);
	}

	if (specialRoomIndex >= 0)
		rooms[specialRoomOptions[specialRoomIndex].first] = RoomData(specialRoomOptions[specialRoomIndex].second, RoomType::SPECIAL, specialRoomOptions[specialRoomIndex].first, randomSeed + rooms.size() * 277);

}

static int randomFunction(int i)
{
	return rand() % i;
}

bool RoomManager::generateRoom(RoomCoord coord, char fromDoor)
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

	switch (fromDoor)
	{
	case NORTH:
		data.doorState = data.doorState | SOUTH;
		break;
	case SOUTH:
		data.doorState = data.doorState | NORTH;
		break;
	case EAST:
		data.doorState = data.doorState | WEST;
		break;
	case WEST:
		data.doorState = data.doorState | EAST;
		break;
	}

	//randomly create doors on each side
	for (char i = 0; i < 4; i++)
	{
		data.doorState |= (rand() > (RAND_MAX * doorChance)) << i;
	}

	rooms[coord] = data;

	//rooms should be generated in a random order, not north, south, east, west
	//since there is a limited amount of rooms, this should stop it from generating a room toward a specific direction

	int shuff[4] = { 1,2,3,4 };

	std::shuffle(shuff, shuff + 4, std::default_random_engine(rand()));
	
	RoomCoord coordinates[4] = {
		GET_COORD(coord.first, coord.second + 1),
		GET_COORD(coord.first, coord.second - 1),
		GET_COORD(coord.first + 1, coord.second),
		GET_COORD(coord.first - 1, coord.second)
	};
	
	for (int i = 0; i < 4; i++)
	{
		char doorState = 1u << shuff[i];
		
		if ((data.doorState & doorState) != 0 && roomCounter < maxRegularRooms - 1)
		{
			//room counter should be iterated here in order to stop it turning into a single, long chain of rooms 
			roomCounter++;
			if (!generateRoom(coordinates[i], doorState))
			{
				roomCounter--;
				data.doorState &= ~doorState;
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
