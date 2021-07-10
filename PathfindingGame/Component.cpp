#include "Component.h"
#include "GameObject.h"

void Component::init(GameObject* connected)
{
	gameObject = connected;
	transform = gameObject->getTransform();
}

void Component::Start()
{
}

void Component::Update()
{
}

void Component::OnDisable()
{
}

void Component::OnEnable()
{
}

void Component::Unload()
{
}

void Component::disableComponent()
{
}

void Component::enableComponent()
{
}
