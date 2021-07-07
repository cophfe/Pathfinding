#pragma once
#include "NecessaryHeaders.h"

class GameObject
{
public:
	//Constructors
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	GameObject(Sprite* texture);
	GameObject(Sprite* texture, bool isDrawn, RLVector2 position, float rotation, float scale);
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

	//update function, played every frame
	virtual void update();

	//Object Hirarchy stuff
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	virtual void addChild(GameObject* child);
	virtual void setParent(GameObject* child);
	virtual void removeChild(GameObject* child);
	virtual void deleteSelf();
	void sortChildren();
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	
	//Transform-related info
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	virtual void updateTransforms();
	inline RLMatrix getGlobalTransform() { return globalTransform;  }
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

	//Position and rotation getting and setting
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	float getRotation() { return rotation; }
	RLVector2* getPosition() { return &position; }
	float getScale() { return scale; }

	void addRotation(float rad);
	void addPosition(RLVector2* pos);
	void addScale (float scale);
	
	void setRotation(float rad);
	void setPosition(RLVector2* pos);
	void setScale(float scale);

	RLVector2 getGlobalPosition();
	float getGlobalRotation();
	float getGlobalScale();
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	
	//Rendering
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	virtual void draw();
	inline Sprite* getSprite() { return sprite; };
	inline void setSprite(Sprite* sprite) { this->sprite = sprite; }// sprite->setGameObject(this); }
	inline Color* getTint() { return &tint; }
	inline void setTint(Color tint) { this->tint = tint; }
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

protected:
	//constructors call this
	//has default values defined atop header
	virtual void init(Sprite* texture, bool isDrawn, RLVector2 position, float rotation, float scale );

	//gameobject hirarchy stuff
	//every GameObject is a child of another GameObject or a child of the scene.
	GameObject* parent;
	std::vector<GameObject*> children;

	//position, rotation and scale information
	RLVector2 position;
	float rotation;
	float scale;
	RLMatrix globalTransform;
	RLMatrix localTransform;

	//drawing
	Color tint;
	Sprite* sprite; //animatedSprite inherits from sprite

	//sprite can access all protected shiz because it is easier that way
	friend Sprite;

};

/*	main versions of gameobject:
	Gameobject
		UIObject
		PhysicsObject
			CollisionObject

*/

