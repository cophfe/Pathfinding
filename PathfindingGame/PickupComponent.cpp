#include "PickupComponent.h"
#include "PlayerUIComponent.h"
#include "GameObject.h"

void PickupComponent::init(Room* room)
{
	//check if pickup has been picked up before on this floor
	this->room = room;
	if (room->getRoomManager()->checkPickupPickedUp())
	{
		gameObject->setIsDrawn(false);
		return;
	}

	//start animation
	((AnimatedSprite*)gameObject->getSprite())->setSettings(PlayerUIComponent::beatStart, PlayerUIComponent::beatEnd, PlayerUIComponent::beatStart);
	gameObject->getSprite()->setTint({ PlayerUIComponent::extraHeartColorDifference , PlayerUIComponent::extraHeartColorDifference * 2 , 0xFF, 0xFF });

	//create rigibody
	b2FixtureDef fDef = RigidBodyComponent::genFixtureDef(RigidBodyComponent::BOUNDS, RigidBodyComponent::PLAYER, nullptr, true);
	b2CircleShape shape;
	shape.m_radius = 0.4f;
	fDef.shape = &shape;
	b2BodyDef bDef = RigidBodyComponent::genBodyDef(b2_staticBody, true);
	gameObject->addComponent<RigidBodyComponent>()->init(room->getCollisionManager(), bDef, fDef);
}

void PickupComponent::onTriggerEnter(RigidBodyComponent* collisionBody, b2Fixture* collisionFixture)
{
	if (!pickedUp && !collisionFixture->IsSensor() && collisionFixture->GetFilterData().categoryBits == RigidBodyComponent::PLAYER)
	{
		pickedUp = true;
		//add heart to player
		auto player = room->getPlayerComponent();
		auto UI = player->getUI();
		UI->addHeart(1);
		UI->heal(5000);
		gameObject->setIsDrawn(false);
		room->getRoomManager()->setPickupPickedUp(true);
	}
}
