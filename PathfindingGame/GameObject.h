#pragma once
#include "NecessaryHeaders.h"
#include "Transform.h"
#include "Scene.h"
#include "Component.h"
#include "Sprite.h"
#include <forward_list>
#include <iostream>
#include <typeinfo>

class Game;
class Scene;
class RigidBodyComponent;

class GameObject
{
public:
	//		Constructors
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//you cannot initialize a gameobject with nullptr parent except through specific constructor
	GameObject(Scene* parent, const char* spriteName = "missing", bool isDrawn = true, Vector2 position = {0,0}, float rotation = 0, float scale = 1, SORTING layer = SORTING::MIDGROUND);
	GameObject( GameObject* parent, const char* spriteName = "missing", bool isDrawn = true, Vector2 position = {0,0}, float rotation = 0, float scale = 1);
	GameObject(Scene* parent, Texture2D* texture, Vector2 position = { 0,0 }, float rotation = 0, float scale = 1, SORTING layer = SORTING::MIDGROUND);
	//no parent (NEEDS PARENT SET MANUALLY!!!)
	GameObject(Sprite* sprite, Vector2 position = { 0,0 }, float rotation = 0, float scale = 1);
	//destructor
	~GameObject();
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//		Copy and Move constructors and assigners 
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//copy
	GameObject(const GameObject&) = delete;
	GameObject& operator=(const GameObject&) = delete;
	//move
	GameObject(GameObject&&) = delete;
	GameObject& operator=(GameObject&&) = delete;
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//		Rendering
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	void draw();
#ifdef DRAW_DEBUG
	//calls debug draw on all components
	void debugDraw();
#endif
	inline Sprite* getSprite() { return sprite; };
	void setSprite(Sprite* sprite);
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// 	   Component stuff
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	void updateComponents();
	void fixedUpdateComponents();
	void startComponents();
	template<typename T>
	T* addComponent();
	template<typename T>
	T* getComponentOfType();
	template<typename T>
	T* deleteComponentOfType();
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//		Transform Stuff
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	Transform* getTransform() { return transform; }
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//		Other
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//disables all components and hides self
	void disableAndHide();
	//enables all components and shows self
	void enableAndShow();
	//deletes self
	void deleteSelf();
	//get the value
	inline int getId() { return id; }
	inline void setIsDrawn(bool value) { isDrawn = value; }
	inline bool getIsDrawn() { return isDrawn; }

protected:

	//holds all components
	std::forward_list<Component*> components;
	//controls position and children
	Transform* transform;
	//to avoid object slicing these will need to be a pointers \/
	//holds all sprites
	Sprite* sprite; 
	//id system could be used to identify gameObjects
	//using an id counter ensures each gameObject has a unique id
	static int idCounter;
	int id;
	//drawing
	bool isDrawn = true;

	//these are called by the rigidbody component on specific events. These call their respective functions in all components of the gameObject
	friend RigidBodyComponent;
	void onCollisionExitComponents(RigidBodyComponent* collisionBody, b2Fixture* collisionFixture);
	void onCollisionEnterComponents(RigidBodyComponent* collisionBody, b2Fixture* collisionFixture);
	void onTriggerExitComponents(RigidBodyComponent* collisionBody, b2Fixture* collisionFixture);
	void onTriggerEnterComponents(RigidBodyComponent* collisionBody, b2Fixture* collisionFixture);
};

template<typename T>
inline T* GameObject::addComponent() //this will be broken if the type given does not inherit from component
{
	T* component = new T();
	((Component*)component)->setGameObject(this);
	components.push_front(component);
	return (T*)components.front();
}
template<typename T>
inline T* GameObject::getComponentOfType()
{
	for (auto& component : components)
	{
		if (typeid(*component) == typeid(T))
		{
			return (T*)component;
		}
	}
	return nullptr;
}
template<typename T>
inline T* GameObject::deleteComponentOfType()
{
	for (auto& component : components)
	{
		if (typeid(*component) == typeid(T))
		{
			component->unload();
			delete component;
			components.remove(component);
			break;
		}
	}
	return nullptr;
}


