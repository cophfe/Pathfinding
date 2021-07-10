#pragma once
#include "NecessaryHeaders.h"
#include "Transform.h"
#include "Scene.h"
#include "Component.h"
#include "Game.h"

class GameObject
{
public:
	//Constructors
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	GameObject(std::string spriteName);
	GameObject(std::string spriteName, Transform* parent, bool isDrawn, Vector2 position, float rotation, float scale);
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

	//Copy and Move constructors and assigners (deleted for now)
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//copy
	GameObject(const GameObject&) = delete;
	GameObject& operator=(const GameObject&) = delete;
	//move
	GameObject(GameObject&&) = delete;
	GameObject& operator=(GameObject&&) = delete;
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		
	virtual void deleteSelf();
	//Rendering
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
	T* addComponent();
	template<typename T>
	T* getComponentOfType();
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//		Transform Stuff
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	Transform& getTransform() { return transform; }

protected:
	//constructors call this
	//has default values defined atop header
	virtual void init(std::string spriteName, Transform* parent, bool isDrawn, Vector2 position, float rotation, float scale);
	
	Transform transform;
	Sprite sprite; 
	std::vector<Component*> components;

	//drawing
	bool isDrawn;

	//sprite can access all protected shiz because it is easier that way
	friend Sprite;

};

/*	main versions of gameobject:
	Gameobject
		UIObject
		PhysicsObject
			CollisionObject

*/

