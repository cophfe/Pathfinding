#include "GameObject.h"
#include "Game.h"

int GameObject::idCounter = 0;

void GameObject::init(const char* spriteName, Transform* parent, bool isDrawn, Vector2 position, float rotation, float scale)
{
	this->isDrawn = isDrawn;

	TextureManager* tM = Game::getInstance().getTextureManager();
	transform = new Transform(position, scale, rotation, parent, this);
	sprite = tM->GenSprite(std::string(spriteName), this);

	id = idCounter;
	++idCounter;
}

GameObject::GameObject(const char* spriteName)
{
	init(spriteName, nullptr, true, { 0 }, 0, 1);
}

GameObject::GameObject(const char* spriteName, Transform* parent, bool isDrawn, Vector2 position, float rotation, float scale)
{
	init(spriteName, parent, true, position, rotation, scale);
}

void GameObject::deleteSelf()
{
	for (auto& component : components)
	{
		component->unload();
	}

	transform->getParent()->removeChild(transform);
	
}

void GameObject::draw()
{
	for (auto& child : transform->getChildArray())
	{
		child->getGameObject()->draw();
	}

	sprite.Draw();
}

void GameObject::updateComponents()
{
	for (auto& child : transform->getChildArray())
	{
		child->getGameObject()->updateComponents();
	}
	for (auto& component : components)
	{
		component->update();
	}
}

void GameObject::startComponents()
{
	for (auto& child : transform->getChildArray())
	{
		child->getGameObject()->updateComponents();
	}
	for (auto& component : components)
	{
		component->start();
	}
}
