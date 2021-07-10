#include "GameObject.h"

void GameObject::init(std::string spriteName, Transform* parent, bool isDrawn, Vector2 position, float rotation, float scale)
{
	this->isDrawn = isDrawn;

	TextureManager* tM = Game::getInstance().getTextureManager();
	sprite = tM->GenSprite(spriteName, this);

	transform = Transform(position, scale, rotation, parent, this);

	if (parent != nullptr)
		transform.setParent(parent);

}

GameObject::GameObject(std::string spriteName)
{
	init(spriteName, nullptr, true, { 0 }, 0, 1);
}

void GameObject::deleteSelf()
{
	for (auto& component : components)
	{
		component->unload();
	}
}

void GameObject::draw()
{
	for (auto& child : transform.getChildArray())
	{
		child->getGameObject()->draw();
	}

	sprite.Draw();
}

void GameObject::updateComponents()
{
	for (auto& child : transform.getChildArray())
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
	for (auto& child : transform.getChildArray())
	{
		child->getGameObject()->updateComponents();
	}

	for (auto& component : components)
	{
		component->update();
	}
}
