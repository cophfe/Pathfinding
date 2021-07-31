#pragma once
#include "Component.h"
#include "Room.h"
#include "RigidBodyComponent.h"

class TrapDoorComponent :
    public Component
{
public:
	void init(Room* scene);

	void update();
	void onTriggerEnter(RigidBodyComponent* collisionBody, b2Fixture* collisionFixture);
private:
	Room* scene;
	bool playerEntered = false;
};

