#include "RigidBodyComponent.h"

#include "CollisionManager.h"

RigidBodyComponent::RigidBodyComponent()
{
}

void RigidBodyComponent::init(GameObject* connected, CollisionManager* collisionManager, b2BodyType bodyType)
{
	b2BodyDef bodyDef;
	bodyDef.type = bodyType;
	bodyDef.position.Set(0.0f, 4.0f);

	world = collisionManager->getWorld();
	body = world->CreateBody(&bodyDef);

	
	//ok honestly this needs to be updated before any other component in any gameobject and idk how tf to do that without putting it in an array in collisionManager
	//it should replace some parts of the transform component completely but that breaks stuff fundamentally and I don't have time to redo all this stuff and still make a functioning game
	//so it be going into an array
}

void RigidBodyComponent::start()
{
}

void RigidBodyComponent::update()
{
}

void RigidBodyComponent::onDisable()
{
}

void RigidBodyComponent::onEnable()
{
}

void RigidBodyComponent::unload()
{
}
