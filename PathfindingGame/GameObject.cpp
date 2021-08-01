#include "GameObject.h"
#include "Game.h"
#include "Scene.h"
#include "RigidBodyComponent.h"

int GameObject::idCounter = 0;

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	CONSTRUCTORS
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

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

GameObject::GameObject(Sprite* sprite, Vector2 position, float rotation, float scale)
{
	isDrawn = true;

	TextureManager* tM = Game::getInstance().getTextureManager();
	transform = new Transform(position, scale, rotation, nullptr, this);

	if (sprite == nullptr)
		this->sprite = new NullSprite();
	else
		this->sprite = sprite;

	id = idCounter;
	++idCounter;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

GameObject::~GameObject()
{
	deleteSelf();
}

void GameObject::deleteSelf()
{
	//if no parent that means parent is scene (unless something has gone wrong). delete self from parent
	if (transform->getParent() == nullptr)
	{
		Game::getInstance().getScene()->removeGameObjectFromChildren(this);
	}
	else
	{
		transform->getParent()->removeChild(transform);
	}
	
	//unload all components then delete them
	for (auto& component : components)
	{
		component->unload();
		delete component;
		component = nullptr;
	}

	//delete all children now too
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
	//update children and self
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
	//update children and self
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
	//start children and self
	for (auto& child : transform->getChildArray())
	{
		child->getGameObject()->updateComponents();
	}
	for (auto& component : components)
	{
		component->start();
	}
}

void GameObject::disableAndHide()
{
	isDrawn = false;
	for (auto& component : components)
	{
		if (component->isEnabled())
			component->disableComponent();
	}

	for (auto& child : transform->getChildArray())
	{
		child->getGameObject()->disableAndHide();
	}
}

void GameObject::enableAndShow()
{
	isDrawn = true;
	for (auto& component : components)
	{
		if (!component->isEnabled())
			component->enableComponent();
	}

	for (auto& child : transform->getChildArray())
	{
		child->getGameObject()->enableAndShow();
	}
}

void GameObject::onCollisionEnterComponents(RigidBodyComponent* collisionBody, b2Fixture* collisionFixture)
{
	//call all collision enter functions
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
	//call all collision exit functions
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
	//call all trigger exit functions
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
	//call all trigger exit functions
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