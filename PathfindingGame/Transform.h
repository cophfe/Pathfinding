#pragma once
#include "NecessaryHeaders.h"

class GameObject;

class Transform
{
public:
	Transform();
	Transform(Vector2 position, float scale, float rotation, Transform* parent, GameObject* attachedGameObject);

	float getRotation() { return rotation; }
	Vector2& getPosition() { return position; }
	float getScale() { return scale; }

	void addRotation(float rad);
	void addPosition(Vector2* pos);
	void addScale(float scale);

	void setRotation(float rad);
	void setPosition(Vector2* pos);
	void setScale(float scale);

	Vector2& getGlobalPosition();
	float getGlobalRotation();
	float getGlobalScale();

	Matrix3& getGlobalTransform();
	Matrix3& getLocalTransform();
	void updateTransforms();

	void addChild(Transform* child);
	void setParent(Transform* parent);
	void removeChild(Transform* child);

	const std::vector<Transform*>& getChildArray() { return children; }
	
	GameObject* getGameObject();
private:
	Vector2 position;
	float rotation;
	float scale;

	Vector2 globalPosition;
	float globalRotation;
	float globalScale;
	
	Matrix3 globalTransform;
	Matrix3 localTransform;

	Transform* parent;
	std::vector<Transform*> children;
	
	GameObject* gameObject;
};

