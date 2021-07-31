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
	direction = { (float)(linkedDoor == EAST) - (linkedDoor == WEST), (float)(linkedDoor == NORTH) - (linkedDoor == SOUTH) };
	gameObject->getSprite()->setTint(PathfinderComponent::BACKGROUND_COLOR);
	b2FixtureDef fDef = RigidBodyComponent::genFixtureDef(RigidBodyComponent::BOUNDS, RigidBodyComponent::PLAYER, nullptr, true);
	b2PolygonShape shape;
	shape.SetAsBox(1.2f, 1.05f);
	fDef.shape = &shape;
	b2BodyDef bDef = RigidBodyComponent::genBodyDef(b2_staticBody, true);
	gameObject->addComponent<RigidBodyComponent>()->init(scene->getCollisionManager(), bDef, fDef);
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

	if (shouldSwitchScene && Vector2(-rb->getVelocity().x, -rb->getVelocity().y).normalised().dot(direction) > 0)
	{
		shouldSwitchScene = false;
		Game::getInstance().getRoomManager()->moveToNextRoom(currentRoom, linkedDoor);
	}
}

void DoorComponent::start()
{
	if (playerEnteredFrom == getDoorType())
	{
		Vector2 position = Vector2((transform->getPosition().x + 190 * ((playerEnteredFrom == WEST) - (playerEnteredFrom == EAST))),
			-(transform->getPosition().y + 210 * (playerEnteredFrom == NORTH) - 150 * (playerEnteredFrom == SOUTH)));

		currentRoom->getPlayerComponent()->getGameObject()->getComponentOfType<RigidBodyComponent>()
			->setPosition(position / PHYSICS_UNIT_SCALE);

		currentRoom->getCamera()->setPosition({ position.x, -position.y });
	}
}

void DoorComponent::onTriggerEnter(RigidBodyComponent* collisionBody, b2Fixture* collisionFixture)
{
	if (open && !collisionFixture->IsSensor() && collisionFixture->GetFilterData().categoryBits == RigidBodyComponent::PLAYER)
	{
		shouldSwitchScene = true;
		rb = collisionBody;
		//schedules move for end of game loop
	}
}

void DoorComponent::onTriggerExit(RigidBodyComponent* collisionBody, b2Fixture* collisionFixture)
{
	if (open && !collisionFixture->IsSensor() && collisionFixture->GetFilterData().categoryBits == RigidBodyComponent::PLAYER)
	{
		shouldSwitchScene = false;
		//schedules move for end of game loop
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
