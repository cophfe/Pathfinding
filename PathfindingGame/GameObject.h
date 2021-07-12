#pragma once
#include "NecessaryHeaders.h"
#include "Transform.h"
#include "Scene.h"
#include "Component.h"
#include "Sprite.h"
#include <forward_list>
#include <iostream>

class Game;

class GameObject
{
public:
	//		Constructors
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	GameObject(const char* spriteName);
	GameObject(const char* spriteName, GameObject* parent, bool isDrawn = true, Vector2 position = { 0,0 }, float rotation = 0, float scale = 1);
	~GameObject();
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//		Copy and Move constructors and assigners (deleted for now)
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
	inline Sprite& getSprite() { return sprite; };
	inline void setSprite(Sprite& sprite) { this->sprite = sprite; }
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// 	   Component stuff
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	void updateComponents();
	void startComponents();
	template<typename T>
	Component* addComponent();
	template<typename T>
	Component* getComponentOfType();
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//		Transform Stuff
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	Transform* getTransform() { return transform; }
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//		Other
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	virtual void deleteSelf();
	inline int getId() { return id; }

protected:
	//id system used to identify gameObjects
	//using an id counter ensures each gameObject has a unique id
	static int idCounter;
	int id;

	//constructors call this
	virtual void init(const char* spriteName, GameObject* parent, bool isDrawn, Vector2 position, float rotation, float scale);
	
	Transform* transform;
	Sprite sprite; 
	//to avoid object slicing this will need to be a pointer list
	std::forward_list<Component*> components;

	//drawing
	bool isDrawn;

	//sprite can access all protected shiz because it is easier that way
	friend Sprite;

};

template<typename T>
inline Component* GameObject::addComponent() //this will be broken if the type given does not inherit from component
{
	T* component = new T();
	components.push_front(component);
	return components.front();
}
template<typename T>
Component* GameObject::getComponentOfType()
{
	for (auto& component : components)
	{
		if (typeid(component) == typeid(T))
		{
			return component;
		}
	}
}


