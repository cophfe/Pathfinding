#pragma once
#include "NecessaryHeaders.h"
#include <random>

#define GET_COORD(x, y) std::make_pair(x, y)
typedef std::pair<int,int> RoomCoord;

enum Door : unsigned char
{
	NORTH = 0b1,
	SOUTH = 0b10,
	EAST = 0b100,
	WEST = 0b1000
};
enum class RoomType : unsigned char
{
	START,
	REGULAR,
	SPECIAL,
	BOSS
};

struct RoomData
{
	char doorState;
	RoomType type;
	RoomCoord coord;
	int randomSeed;

	RoomData(char doorState, RoomType type, RoomCoord coord, int randomSeed) : 
		doorState(doorState), type(type), coord(coord), randomSeed(randomSeed)	{}

	RoomData() {}
};

class RoomManager
{
public:
	RoomManager(int randomSeed) : randomSeed(randomSeed) {}

	static constexpr float doorChance = 0.5f;
	static constexpr int maxRegularRooms = 10;
	int roomCounter = 0;
	void generateMap();

private:
	bool generateRoom(RoomCoord coord, char fromDoor);
	int getCoordNeighborCount(RoomCoord coord, char* neighborDoorsInfo);

	//I decided this is the best way to store the rooms
	//it allows it to be sparse and potentially infinitely big, and it allows for easy access through coordinate
	// using std::pair instead of a Vector2i because pair has comparing built in
	std::map<RoomCoord, RoomData> rooms;

	int randomSeed;
};

