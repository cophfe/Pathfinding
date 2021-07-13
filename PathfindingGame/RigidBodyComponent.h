#pragma once
#include "Component.h"
#include "NecessaryHeaders.h"

class CollisionManager;
//PHYSICS OBJECT
//UNDEFINED BEHAVIOUR WHEN ON A CHILDED OBJECT
class RigidBodyComponent : public Component
{
public:
	RigidBodyComponent();

	void init(GameObject* connected, CollisionManager* collisionManager, b2BodyType bodyType = b2_staticBody);
	void start();
	void update();
	void onDisable();
	void onEnable();
	void unload();

private:
	b2Body* body;

	b2World* world;
};

/// The body type.
/// static: zero mass, zero velocity, may be manually moved
/// kinematic: zero mass, non-zero velocity set by user, moved by solver
/// dynamic: positive mass, non-zero velocity determined by forces, moved by solver