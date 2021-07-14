#pragma once
#include "NecessaryHeaders.h"
#include <forward_list>

class GameObject;

class Transform
{
public:
	Transform(Vector2 position, float scale, float rotation, Transform* parent, GameObject* attachedGameObject);

	float getRotation() { return rotation; }
	Vector2& getPosition() { return position; }
	float getScale() { return scale; }

	void addRotation(float rad);
	void addPosition(Vector2 pos);
	void addScale(float scale);

	void setRotation(float rad);
	void setTransform(Vector2 pos);
	void setScale(float scale);

	Vector2& getGlobalPosition();
	float getGlobalRotation();
	float getGlobalScale();

	Matrix3& getGlobalTransform();
	Matrix3& getLocalTransform();
	void updateLocalTransform();
	void updateGlobalTransform();

	inline Transform* getParent() { return parent; }
	void addChild(Transform* child);
	void removeChild(Transform* child);

	const std::forward_list<Transform*>& getChildArray() { return children; }
	
	GameObject* getGameObject();
private:
	void setParent(Transform* parent);

	Vector2 position;
	float rotation;
	float scale;

	Vector2 globalPosition;
	float globalRotation;
	float globalScale;
	
	Matrix3 globalTransform;
	Matrix3 localTransform;

	Transform* parent;
	std::forward_list<Transform*> children;
	
	GameObject* gameObject;
};

