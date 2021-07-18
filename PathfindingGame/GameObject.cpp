#include "GameObject.h"
#include "Game.h"
#include "Scene.h"

int GameObject::idCounter = 0;

void GameObject::init(const char* spriteName, GameObject* parent, bool isDrawn, Vector2 position, float rotation, float scale)
{
	this->isDrawn = isDrawn;

	TextureManager* tM = Game::getInstance().getTextureManager();
	if (parent == nullptr)
		transform = new Transform(position, scale, rotation, nullptr, this);
	else	
		transform = new Transform(position, scale, rotation, parent->getTransform(), this);
	if (isDrawn)
		sprite = tM->GenSprite(std::string(spriteName), this);
	else
		sprite = new NullSprite();
	

	id = idCounter;
	++idCounter;
}

GameObject::GameObject(Scene* parent, const char* spriteName, bool isDrawn, Vector2 position, float rotation, float scale, SORTING layer)
{
	init(spriteName, nullptr, isDrawn, position, rotation, scale);
	parent->addGameObject(this, layer);
}

GameObject::GameObject(GameObject* parent, const char* spriteName, bool isDrawn, Vector2 position, float rotation, float scale)
{
	init(spriteName, parent, isDrawn, position, rotation, scale);
}

GameObject::~GameObject()
{
	deleteSelf();
}

void GameObject::deleteSelf()
{
	if (transform->getParent() != nullptr)
		transform->getParent()->removeChild(transform);
	
	for (auto& component : components)
	{
		component->unload();
		delete component;
	}

	int count = std::distance(std::begin(transform->getChildArray()), std::end(transform->getChildArray()));
	for (int i = 0; i < count; ++i)
	{
		delete transform->getChildArray().front()->getGameObject();
	}

	delete transform;
	transform = nullptr;

	delete sprite;
	sprite = nullptr;
}

void GameObject::draw()
{
	sprite->Draw();

	for (auto& child : transform->getChildArray())
	{
		child->getGameObject()->draw();
	}

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

void GameObject::fixedUpdateComponents()
{
	for (auto& child : transform->getChildArray())
	{
		child->getGameObject()->fixedUpdateComponents();
	}
	for (auto& component : components)
	{
		component->fixedUpdate();
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
