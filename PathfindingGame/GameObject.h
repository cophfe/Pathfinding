#pragma once
#include "NecessaryHeaders.h"

#include "Sprite.h"

class GameObject
{
public:
	GameObject(Texture2D* texture);
	GameObject(Texture2D* texture, bool isDrawn, Vector2 position, float rotation, float scale);

	//copy
	GameObject(const GameObject&) = delete;
	GameObject& operator=(const GameObject&) = delete;
	//move
	GameObject(GameObject&&) = delete;
	GameObject& operator=(GameObject&&) = delete;

	virtual void update();
	virtual void draw();

protected:
	//constructors call this
	//has default values defined atop header
	virtual void init(Texture2D* texture, bool isDrawn, Vector2 position, float rotation, float scale);

	//gameobject hirarchy stuff
	//every GameObject is a child of another GameObject or a child of the scene.
	GameObject* parent;
	std::vector<GameObject*> children;

	//position, rotation and scale information
	Vector2 position;
	float rotation;
	float scale;
	Matrix globalTransform;
	Matrix localTransform;

	//drawing
	bool isDrawn;
	Sprite* sprite; //animatedSprite inherits from sprite

};

/*	main versions of gameobject:
	Gameobject
		UIObject
		PhysicsObject
			CollisionObject

*/

