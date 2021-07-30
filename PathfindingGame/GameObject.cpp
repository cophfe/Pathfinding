#include "GameObject.h"
#include "Game.h"
#include "Scene.h"
#include "RigidBodyComponent.h"

int GameObject::idCounter = 0;

void GameObject::init(const char* spriteName, GameObject* parent, bool isDrawn, Vector2 position, float rotation, float scale)
{
	this->isDrawn = isDrawn;

	TextureManager* tM = Game::getInstance().getTextureManager();
	if (parent == nullptr)
		transform = new Transform(position, scale, rotation, nullptr, this);
	else	
		transform = new Transform(position, scale, rotation, parent->getTransform(), this);
	
	if (spriteName != nullptr)
		sprite = tM->genSprite(std::string(spriteName), this);
	else
		sprite = new NullSprite();
	

	id = idCounter;
	++idCounter;
}

GameObject::GameObject(Scene* parent, const char* spriteName, bool isDrawn, Vector2 position, float rotation, float scale, SORTING layer)
{
	this->isDrawn = isDrawn;

	TextureManager* tM = Game::getInstance().getTextureManager();
	transform = new Transform(position, scale, rotation, nullptr, this);

	if (spriteName != nullptr)
		sprite = tM->genSprite(std::string(spriteName), this);
	else
		sprite = new NullSprite();

	id = idCounter;
	++idCounter;

	parent->addGameObject(this, layer);
}

GameObject::GameObject(Scene* parent, Texture2D* texture, Vector2 position, float rotation, float scale, SORTING layer)
{
	isDrawn = true;

	transform = new Transform(position, scale, rotation, nullptr, this);

	if (texture == nullptr)
		sprite = new NullSprite();
	else
		sprite = new Sprite(texture, this);

	id = idCounter;
	++idCounter;

	parent->addGameObject(this, layer);
}

GameObject::GameObject(GameObject* parent, const char* spriteName, bool isDrawn, Vector2 position, float rotation, float scale)
{
	this->isDrawn = isDrawn;

	TextureManager* tM = Game::getInstance().getTextureManager();
	if (parent == nullptr)
		transform = new Transform(position, scale, rotation, nullptr, this);
	else
		transform = new Transform(position, scale, rotation, parent->getTransform(), this);
	
	if (spriteName != nullptr)
		sprite = tM->genSprite(std::string(spriteName), this);
	else
		sprite = new NullSprite();

	id = idCounter;
	++idCounter;
}

GameObject::GameObject(Texture2D* texture, Vector2 position, float rotation, float scale)
{
	isDrawn = true;

	TextureManager* tM = Game::getInstance().getTextureManager();
	transform = new Transform(position, scale, rotation, nullptr, this);

	if (texture == nullptr)
		sprite = new NullSprite();
	else
		sprite = new Sprite(texture, this);

	id = idCounter;
	++idCounter;
}

GameObject::~GameObject()
{
	deleteSelf();
}

void GameObject::deleteSelf()
{
	if (transform->getParent() == nullptr)
	{
		Game::getInstance().getScene()->removeGameObjectFromChildren(this);
	}
	else
	{
		transform->getParent()->removeChild(transform);
	}
	
	for (auto& component : components)
	{
		component->unload();
		delete component;
		component = nullptr;
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
	if (isDrawn)
		sprite->draw();

	for (auto& child : transform->getChildArray())
	{
		child->getGameObject()->draw();
	}
}

#ifdef DRAW_DEBUG
void GameObject::debugDraw()
{
	for (auto& component : components)
	{
		if (component->isEnabled())
			component->debugDraw();
	}
}
#endif

void GameObject::setSprite(Sprite* sprite)
{
	if (this->sprite != nullptr)
	{
		delete this->sprite;
	}
	this->sprite = sprite;
}

void GameObject::updateComponents()
{
	for (auto& child : transform->getChildArray())
	{
		child->getGameObject()->updateComponents();
	}
	for (auto& component : components)
	{
		if (component->isEnabled())
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
		if (component->isEnabled())
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

void GameObject::onCollisionEnterComponents(RigidBodyComponent* collisionBody, b2Fixture* collisionFixture)
{
	for (auto& child : transform->getChildArray())
	{
		child->getGameObject()->onCollisionEnterComponents(collisionBody, collisionFixture);
	}
	for (auto& component : components)
	{
		if (component->isEnabled())
			component->onCollisionEnter(collisionBody, collisionFixture);
	}
}

void GameObject::onCollisionExitComponents(RigidBodyComponent* collisionBody, b2Fixture* collisionFixture)
{
	for (auto& child : transform->getChildArray())
	{
		child->getGameObject()->onCollisionExitComponents(collisionBody, collisionFixture);
	}
	for (auto& component : components)
	{
		if (component->isEnabled())
			component->onCollisionExit(collisionBody, collisionFixture);
	}
}

void GameObject::onTriggerEnterComponents(RigidBodyComponent* collisionBody, b2Fixture* collisionFixture)
{
	for (auto& child : transform->getChildArray())
	{
		child->getGameObject()->onTriggerEnterComponents(collisionBody, collisionFixture);
	}
	for (auto& component : components)
	{
		if (component->isEnabled())
			component->onTriggerEnter(collisionBody, collisionFixture);
	}
}

void GameObject::onTriggerExitComponents(RigidBodyComponent* collisionBody, b2Fixture* collisionFixture)
{
	for (auto& child : transform->getChildArray())
	{
		child->getGameObject()->onTriggerExitComponents(collisionBody, collisionFixture);
	}
	for (auto& component : components)
	{
		if (component->isEnabled())
			component->onTriggerExit(collisionBody, collisionFixture);
	}
}