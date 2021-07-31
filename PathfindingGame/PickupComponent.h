#pragma once
#include "Component.h"
#include "Room.h"
#include "PlayerComponent.h"
#include "RigidbodyComponent.h"

class PickupComponent : public Component
{
public:
	void init(Room* room);
	void onTriggerEnter(RigidBodyComponent* collisionBody, b2Fixture* collisionFixture);

private:
	Room* room;
	bool pickedUp = false;
};

