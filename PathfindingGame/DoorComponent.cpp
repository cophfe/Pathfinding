#include "DoorComponent.h"
#include "GameObject.h"
#include "Game.h"
#include "RigidBodyComponent.h"
#include "PathfinderComponent.h"
#include "PlayerComponent.h"

void DoorComponent::init(Room* scene, char doorType, bool openByDefault, AgentDataComponent* agentComponent, char enteredFrom)
{
	blackboard = agentComponent;
	if (openByDefault || blackboard == nullptr)
	{
		((AnimatedSprite*)gameObject->getSprite())->pauseAt(open);
		open = true;
	}
	else
	{
		((AnimatedSprite*)gameObject->getSprite())->setSettings(OPEN_STATIC, SHUT_STATIC, OPEN_STATIC);
		((AnimatedSprite*)gameObject->getSprite())->setCallback(shutCallback, gameObject->getSprite());
	}
	
	currentRoom = scene;
	switch (doorType)
	{
	case NORTH:
		linkedDoor = SOUTH;
		break;
	case SOUTH:
		linkedDoor = NORTH;
		break;
	case EAST:
		linkedDoor = WEST;
		break;
	case WEST:
		linkedDoor = EAST;
		break;
	}
	gameObject->getSprite()->setTint(PathfinderComponent::BACKGROUND_COLOR);
	b2FixtureDef fDef = RigidBodyComponent::genFixtureDef(RigidBodyComponent::BOUNDS, RigidBodyComponent::PLAYER, nullptr, true);
	b2BodyDef bDef = RigidBodyComponent::genBodyDef(b2_staticBody, true);
	gameObject->addComponent<RigidBodyComponent>()->init(scene->getCollisionManager(), bDef, fDef, true);
	playerEnteredFrom = enteredFrom;
}

void DoorComponent::update()
{
	timer -= Game::getDeltaTime();
	if (!open && blackboard->noAgentsLeft())
	{
		((AnimatedSprite*)gameObject->getSprite())->play();
		((AnimatedSprite*)gameObject->getSprite())->setSettings(OPENING_START, OPENING_END, OPENING_START);
		((AnimatedSprite*)gameObject->getSprite())->setCallback(openCallback, gameObject->getSprite());
		open = true;
	}

	if (shouldSwitchScene)
	{
		shouldSwitchScene = false;
		Game::getInstance().getRoomManager()->moveToNextRoom(currentRoom, linkedDoor);
	}
}

void DoorComponent::start()
{
	if (playerEnteredFrom == getDoorType())
	{
		static int moveAmount = 150;

		Vector2 position = Vector2((transform->getPosition().x + moveAmount * ((playerEnteredFrom == WEST) - (playerEnteredFrom == EAST))),
			-(transform->getPosition().y + moveAmount * ((playerEnteredFrom == NORTH) - (playerEnteredFrom == SOUTH))));
		currentRoom->getPlayerComponent()->getGameObject()->getComponentOfType<RigidBodyComponent>()
			->setPosition(position / PHYSICS_UNIT_SCALE);
		currentRoom->getCamera()->setPosition({ position.x - 900 * ((playerEnteredFrom == WEST) - (playerEnteredFrom == EAST)), position.y - 900 * ((playerEnteredFrom == NORTH) - (playerEnteredFrom == SOUTH)) });
	}
}

void DoorComponent::onTriggerEnter(RigidBodyComponent* collisionBody, b2Fixture* collisionFixture)
{
	if (open && timer <= 0 && !collisionFixture->IsSensor() && collisionFixture->GetFilterData().categoryBits == RigidBodyComponent::PLAYER)
	{
		//schedules move for end of game loop
		shouldSwitchScene = true;
	}
}

char DoorComponent::getDoorType()
{
	switch (linkedDoor)
	{
	case NORTH:
		return SOUTH;
	case SOUTH:
		return NORTH;
	case EAST:
		return WEST;
	case WEST:
		return EAST;
	}
}
