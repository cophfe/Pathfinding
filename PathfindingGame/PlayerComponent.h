#pragma once
#include "NecessaryHeaders.h"
#include "RigidBodyComponent.h"
#include "Component.h"

class GameObject;

class PlayerComponent : public Component
{
public:
	void init(float acceleration);
	void start();
	void update();
	void fixedUpdate();

private:
	float acceleration;

	RigidBodyComponent* rigidBody;
	
	Vector2 direction;
};

