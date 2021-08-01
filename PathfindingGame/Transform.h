#pragma once
#include "NecessaryHeaders.h"
#include <forward_list>

class GameObject;

//holds all of the position, rotation, scaling and object hirarchy data
class Transform
{
public:
	Transform(Vector2 position, float scale, float rotation, Transform* parent, GameObject* attachedGameObject);

	float getRotation() const { return rotation; }
	const Vector2& getPositionReference() const { return position; }
	Vector2 getPosition() const { return position; }
	float getScale() const { return scale; }

	void addRotation(float rad);
	void addPosition(Vector2 pos);
	void addScale(float scale);

	void setRotation(float rad);
	void setPosition(Vector2 pos);
	void setScale(float scale);

	Vector2& getGlobalPosition();
	float getGlobalRotation();
	float getGlobalScale();

	Matrix3& getGlobalTransform();
	Matrix3& getLocalTransform();
	//needs to be called whenever local transform changes
	void updateLocalTransform();
	//needs to be called whenever global transform changes
	void updateGlobalTransform();

	void flipPositionX();

	inline Transform* getParent() { return parent; }
	void addChild(Transform* child);
	void removeChild(Transform* child);

	const std::forward_list<Transform*>& getChildArray() { return children; }
	
	GameObject* getGameObject();
private:
	void setParent(Transform* parent);

	Matrix3 globalTransform;
	Matrix3 localTransform;
	
	std::forward_list<Transform*> children;
	Transform* parent;
	GameObject* gameObject;
	
	//global variables are stored seperately, not calculated every time they are accessed
	//idk if this was a wise decision, but it sure was a decision
	Vector2 globalPosition;
	Vector2 position;
	float globalRotation;
	float rotation;
	float globalScale;
	float scale;
};

