#pragma once
#include "NecessaryHeaders.h"
#include "Scene.h"

class RoomManager;
class PathfinderComponent;
class PlayerComponent;
typedef std::pair<int, int> RoomCoord;

//used to find direction of doors
enum Door : unsigned char
{
	NORTH = 0b1,
	SOUTH = 0b10,
	EAST = 0b100,
	WEST = 0b1000
};
//used to find type of room
enum class RoomType : unsigned char
{
	START,
	REGULAR,
	SPECIAL,
	EXIT
};

//holds all the data required to generate room
struct RoomData : public SceneProperties
{
	//what doors it has
	char doorState;
	RoomType type;
	RoomCoord coord;
	//if explored it will light up on the map and no bees will spawn in it on reentry
	bool explored = false;
	//in scene properties \/
	//int randomSeed;
	//random seed is important because it insures the room is generated in the same way every time it is entered

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
	//called after everything is initialized
	void start();
	void unload();
	//called before the room is switched out for a new scene
	void beforeDelete(Scene* nextScene);
	
	//getters
	Pathfinder* getPathfinder();
	inline RoomManager* getRoomManager() { return roomManager; }
	inline PlayerComponent* getPlayerComponent() { return player; }
	inline RoomCoord getCoord() { return roomCoordinates; }

	//tell the room to do a drop transition instead of a slide transition
	inline void setDropTransition(bool value) { roomDropTransition = value; }
private:
	friend RoomManager;

	//the coordinate of the room on the map
	RoomCoord roomCoordinates;
	//the player component
	PlayerComponent* player;
	//the pathfinder
	PathfinderComponent* pathfinderObject;
	//the roommanager (room manager is not specific to any room)
	RoomManager* roomManager;
	//what direction this room was entered from
	char enteredFrom;
	//if the room to do a drop transition instead of a slide transition
	bool roomDropTransition = false;
	
};