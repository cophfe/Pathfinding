#include "Component.h"
#include "GameObject.h"

Component::Component() {}

void Component::init()
{
	onEnable();
}

void Component::start()
{
}

void Component::update()
{
}

void Component::fixedUpdate()
{
}

void Component::onDisable()
{
}

void Component::onEnable()
{
}

void Component::unload()
{
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
