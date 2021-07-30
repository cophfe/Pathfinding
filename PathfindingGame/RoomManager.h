#pragma once
#include "NecessaryHeaders.h"
#include "Room.h"
#include <random>

#define GET_COORD(x, y) std::make_pair(x, y)

class RoomManager
{
public:
	RoomManager(int randomSeed);
	~RoomManager();
	static constexpr float doorChance = 0.5f;
	static constexpr int maxRegularRooms = 10;
	void generateMap();
	void generateMiniMap();
	Room* createFirstRoom();
	void moveToNextRoom(Room* currentRoom, char enteredFrom);
	//mini map stuff
	static constexpr float mapSize = 150;
	static constexpr float roomSize = 18;
	static constexpr float sizeRatio = 1.6f;
	static constexpr Color mapColor = { 0,0,0, 0x90 };
	static constexpr Color roomColor = { 0x80, 0x80, 0x80, 0xFF };
	static constexpr Color bossRoomColor = { 0xFF,0xA0,0xA0, 0xFF };
	static constexpr Color startRoomColor = { 0xA0, 0xA0, 0xFF, 0xFF };
	static constexpr Color specialRoomColor = { 0xFF,0xFF,0x80, 0xFF };
	static constexpr int LINE_THICKNESS = 8;
	static constexpr float offset = 3;
	static constexpr float roomPadding = offset * 2;

	inline RoomCoord getCurrentRoomCoord() { return currentRoom; }
	inline RenderTexture2D* getMiniMap() { return &miniMapTexture; }
	inline RenderTexture2D* getTransitionTexture1() { return &transitionBuffer[0]; }
	inline RenderTexture2D* getTransitionTexture2() { return &transitionBuffer[1]; }
	
private:
	bool generateRoom(RoomCoord coord);
	int getCoordNeighborCount(RoomCoord coord, char* neighborDoorsInfo);

	//I decided this is the best way to store the rooms
	//it allows it to be sparse and potentially infinitely big, and it allows for easy access through coordinate
	// using std::pair instead of a Vector2i because pair has comparing built in
	std::map<RoomCoord, RoomData> rooms;


	RenderTexture2D miniMapTexture;
	RenderTexture2D transitionBuffer[2];
	RoomCoord currentRoom;
	RoomCoord startRoom;
	RoomCoord bossRoom;
	RoomCoord specialRoom;

	int randomSeed;
	int roomCounter = 0;
};

