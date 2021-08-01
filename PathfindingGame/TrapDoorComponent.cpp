#include "TrapDoorComponent.h"
#include "GameObject.h"
#include "PlayerComponent.h"
#include "Game.h"

void TrapDoorComponent::init(Room* scene)
{
	//add sensor
	b2FixtureDef fDef = RigidBodyComponent::genFixtureDef(RigidBodyComponent::BOUNDS, RigidBodyComponent::PLAYER, nullptr, true);
	b2CircleShape shape;
	shape.m_radius = 0.6f;
	fDef.shape = &shape;
	b2BodyDef bDef = RigidBodyComponent::genBodyDef(b2_staticBody, true);
	gameObject->addComponent<RigidBodyComponent>()->init(scene->getCollisionManager(), bDef, fDef);
	this->scene = scene;
}

void TrapDoorComponent::update()
{
	if (playerEntered)
	{
		//if player entered trap door switch to room on new floor
		scene->setDropTransition(true);
		playerEntered = false;
		auto player = scene->getPlayerComponent();
		player->addFloor();
		auto roomManager = Game::getInstance().getRoomManager();
		roomManager->generateNewMap(rand());
		player->getGameObject()->getComponentOfType<RigidBodyComponent>()->disableComponent();
		Room* newRoom = new Room();
		newRoom->setDropTransition(true);
		newRoom->load(roomManager->getCurrentRoom(), roomManager, player, 0);
		Game::getInstance().switchScene(newRoom);
	}
		
}

void TrapDoorComponent::onTriggerEnter(RigidBodyComponent* collisionBody, b2Fixture* collisionFixture)
{
	if (!collisionFixture->IsSensor() && collisionFixture->GetFilterData().categoryBits == RigidBodyComponent::PLAYER)
	{
		//anything to do with rigidbodies cannot be updated in here
		playerEntered = true;
	}
}
