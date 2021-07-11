#pragma once
#include "NecessaryHeaders.h"
#include "Component.h"

class GameObject;

class PlayerComponent : public Component
{
public:
	void init(float speed, float rotationalSpeed, GameObject* connected);
	void update();

private:
	float speed;
	float rotationalSpeed;
};

