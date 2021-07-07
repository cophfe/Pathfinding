#include "GameObject.h"

void GameObject::init(Sprite* sprite, bool isDrawn, RLVector2 position, float rotation, float scale)
{
}

GameObject::GameObject(Sprite* sprite)
{
}

GameObject::GameObject(Sprite* sprite, bool isDrawn, RLVector2 position, float rotation, float scale)
{
	
}

void GameObject::update()
{
}

void GameObject::addChild(GameObject* child)
{
}

void GameObject::setParent(GameObject* child)
{
}

void GameObject::removeChild(GameObject* child)
{
}

void GameObject::deleteSelf()
{
}

void GameObject::sortChildren()
{
}

void GameObject::updateTransforms()
{
}

void GameObject::addRotation(float rad)
{
}

void GameObject::addPosition(RLVector2* pos)
{
}

void GameObject::addScale(float scale)
{
}

void GameObject::setRotation(float rad)
{
}

void GameObject::setPosition(RLVector2* pos)
{
}

void GameObject::setScale(float scale)
{
}

RLVector2 GameObject::getGlobalPosition()
{
	return RLVector2();
}

float GameObject::getGlobalRotation()
{
	return 0.0f;
}

float GameObject::getGlobalScale()
{
	return 0.0f;
}

void GameObject::draw()
{
}
