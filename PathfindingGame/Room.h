#pragma once
#include "NecessaryHeaders.h"
#include "Scene.h"

class RoomManager;
class PathfinderComponent;
class PlayerComponent;
typedef std::pair<int, int> RoomCoord;

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

struct RoomData : public SceneProperties
{
	char doorState;
	RoomType type;
	RoomCoord coord;
	bool explored = false;
	//in scene properties \/
	//int randomSeed;

	RoomData(char doorState, RoomType type, RoomCoord coord, int randomSeed) :
		doorState(doorState), type(type), coord(coord)
	{	randomSeed = randomSeed;	}

	RoomData() {}
};

class Room :
    public Scene
{
public:

	void load(RoomData* properties, RoomManager* manager, PlayerComponent* player = nullptr, char enteredFrom = 0);
	void draw();
	void start();
	void unload();
	void beforeDelete(Scene* nextScene);

	Pathfinder* getPathfinder();
	inline RoomManager* getRoomManager() { return roomManager; }
	inline PlayerComponent* getPlayerComponent() { return player; }
	inline RoomCoord getCoord() { return roomCoordinates; }

private:
	friend RoomManager;

	char enteredFrom;
	RoomCoord roomCoordinates;
	PlayerComponent* player;
	PathfinderComponent* pathfinderObject;
	RoomManager* roomManager;
};