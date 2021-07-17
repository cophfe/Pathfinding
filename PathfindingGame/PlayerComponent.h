#pragma once
#include "NecessaryHeaders.h"
#include "RigidBodyComponent.h"
#include "Component.h"

#include <iostream>

class GameObject;

class PlayerComponent : public Component
{
public:
	void init(float maxSpeed, float maxAcceleration);
	void start();
	void update();
	void fixedUpdate();

private:
	float maxSpeed;
	float maxAcceleration;
	RigidBodyComponent* rigidBody;
	
	Vector2 direction;
};

