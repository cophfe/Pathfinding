#pragma once
#include "Component.h"
#include "NecessaryHeaders.h"

class RigidBodyComponent : public Component
{
	RigidBodyComponent();

	void init(GameObject* connected, b2Shape shape, b2BodyType bodyType = b2_staticBody);
	void start();
	void update();
	void onDisable();
	void onEnable();
	void unload();
};

/// The body type.
/// static: zero mass, zero velocity, may be manually moved
/// kinematic: zero mass, non-zero velocity set by user, moved by solver
/// dynamic: positive mass, non-zero velocity determined by forces, moved by solver