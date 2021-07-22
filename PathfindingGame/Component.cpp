#include "Component.h"
#include "GameObject.h"
#include "RigidBodyComponent.h"

void Component::init()
{
	onEnable();
}

void Component::setGameObject(GameObject* connected)
{
	gameObject = connected;
	transform = gameObject->getTransform();
	enabled = true;
}

void Component::disableComponent()
{
	enabled = false;
	onDisable();
}

void Component::enableComponent()
{
	enabled = true;
	onEnable();
}
