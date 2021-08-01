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
	//generate map
	void generateMap();
	//regenerate map
	void generateNewMap(int newSeed, bool newFloor = true);
	//draw mini map to mini map texture
	void generateMiniMap();
	//create the first room
	Room* createFirstRoom();
	//move to the next room
	void moveToNextRoom(Room* currentRoom, char enteredFrom);

	//map stuff
	static constexpr float DOOR_CHANCE = 0.5f;
	static constexpr int MAX_ROOMS_START = 5;
	//mini map stuff
	static constexpr float MAP_SIZE = 220;
	static constexpr float ROOM_SIZE = 18;
	static constexpr float SIZE_RATIO = 1.6f;
	static constexpr Color MAP_COLOR = { 0,0,0, 0x90 };
	static constexpr Color ROOM_COLOR = { 0x80, 0x80, 0x80, 0xFF };
	static constexpr Color EXIT_ROOM_COLOR = { 0xFF,0xA0,0xA0, 0xFF };
	static constexpr Color START_ROOM_COLOR = { 0xA0, 0xA0, 0xFF, 0xFF };
	static constexpr Color ITEM_ROOM_COLOR = { 0xFF,0xFF,0x80, 0xFF };
	static constexpr int LINE_THICKNESS = 8;
	static constexpr float MAP_ROOM_OFFSET = 3;
	static constexpr float MAP_ROOM_PADDING = MAP_ROOM_OFFSET * 2;

	void setPickupPickedUp(bool value) { pickUpPickedUp = value; }
	//getters
	inline RoomCoord getCurrentRoomCoord() { return currentRoom; }
	inline RenderTexture2D* getMiniMap() { return &miniMapTexture; }
	inline RenderTexture2D* getTransitionTexture1() { return &transitionBuffer[0]; }
	inline RenderTexture2D* getTransitionTexture2() { return &transitionBuffer[1]; }
	inline RoomData* getCurrentRoom() { return &rooms[currentRoom]; }
	bool checkPickupPickedUp() { return pickUpPickedUp; }
	inline bool checkMapHasBeenGenerated() { return mapCreatedBefore; }

private:
	//generate a room and it's connected rooms
	bool generateRoom(RoomCoord coord);
	//gets info on the neighbors of a coordinate
	int getCoordNeighborCount(RoomCoord coord, char* neighborDoorsInfo);

	//the render texture the minimap is written to
	RenderTexture2D miniMapTexture;
	//the render textures used for transitioning between two scenes
	RenderTexture2D transitionBuffer[2];
	//I decided this is the best way to store the rooms
	//it allows it to be sparse and potentially infinitely big, and it allows for easy access through coordinate
	// using std::pair instead of a Vector2i because pair has comparing built in
	std::map<RoomCoord, RoomData> rooms;
	//info on where various rooms are
	//current room is used for getting next room and generating mini map
	//the rest are just used for the mini map
	RoomCoord currentRoom;
	RoomCoord startRoom;
	RoomCoord bossRoom;
	RoomCoord specialRoom;
	//the initial random seed inputted
	int randomSeed;
	//the current amount of rooms on a floor
	int roomCounter = 0;
	//the max amount of rooms (changes between floors)
	int maxRooms = MAX_ROOMS_START;
	//used to check if the map has been generated previously or not
	bool mapCreatedBefore = false;
	//used to check if a floor's pickup has been taken or not
	bool pickUpPickedUp = false;
};

