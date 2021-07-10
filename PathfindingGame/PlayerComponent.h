#pragma once
#include "NecessaryHeaders.h"
#include "Component.h"

class PlayerComponent : public Component
{
public:
	void update();

private:
	float speed = 4;
};

