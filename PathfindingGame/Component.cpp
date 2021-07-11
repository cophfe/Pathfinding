#include "Component.h"
#include "GameObject.h"

void Component::init(GameObject* connected)
{
	gameObject = connected;
	transform = gameObject->getTransform();
	enabled = true;
}

void Component::start()
{
}

void Component::update()
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

void Component::disableComponent()
{
}

void Component::enableComponent()
{
}
