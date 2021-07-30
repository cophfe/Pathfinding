#pragma once
#include "NecessaryHeaders.h"
#include "Component.h"
#include "RoomManager.h"
#include "AgentDataComponent.h"
#include "Sprite.h"

class Room;
class RigidBodyComponent;

class DoorComponent :
    public Component
{
public:
	//sprite info:
	static constexpr int OPEN_STATIC = 0;
	static constexpr int SHUT_STATIC = 7;
	static constexpr int OPENING_START = 8;
	static constexpr int OPENING_END = 14;


	void init(Room* scene, char doorType, bool openByDefault, AgentDataComponent* agentComponent, char enteredFrom);
	void update();
	void start();
	//has trigger collider
	virtual void onTriggerEnter(RigidBodyComponent* collisionBody, b2Fixture* collisionFixture);
	virtual void onTriggerExit(RigidBodyComponent* collisionBody, b2Fixture* collisionFixture);

private:
	static void shutCallback(void* pointer)	{	((AnimatedSprite*)(pointer))->pauseAt(SHUT_STATIC);   }
	static void openCallback(void* pointer)	{	((AnimatedSprite*)(pointer))->pauseAt(OPEN_STATIC);   }
	char getDoorType();

	char linkedDoor;
	char playerEnteredFrom;
	float timer = 0.5f;
	Room* currentRoom;
	Vector2 direction;
	AgentDataComponent* blackboard;
	RigidBodyComponent* rb;
	bool open = false;
	bool shouldSwitchScene = false;
};

