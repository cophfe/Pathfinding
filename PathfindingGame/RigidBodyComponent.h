#pragma once
#include "Component.h"
#include "NecessaryHeaders.h"

#define RENDER_SCALE 10

class GameObject;
class CollisionManager;

//PHYSICS OBJECT
//UNDEFINED BEHAVIOUR WHEN ON A CHILDED OBJECT
//ALSO MIGHT BREAK WHEN NOT THE FIRST COMPONENT IN THE COMPONENT LIST (add it last)
class RigidBodyComponent : public Component
{
public:
	void init(GameObject* connected, CollisionManager* collisionManager, b2BodyDef& bodyDef, b2FixtureDef& fixtureDef);

	void update();
	void onDisable();
	void onEnable();
	void unload();

	void applyForce(Vector2 force);
	void applyForce(float x, float y);
	void addVelocity(Vector2 velocity);
	void addVelocity(float x, float y);
	void setVelocity(Vector2 velocity);
	void setVelocity(float x, float y);
	void setTransform(Vector2 position, float angle);
	void setTransform(float x, float y, float angle);
	void addImpulse(Vector2 impulse, Vector2 position);

	static b2BodyDef genBodyDef(b2BodyType type, Vector2 velocity, float angularVelocity, float angularDamping = 0, float linearDamping = 0, bool fixedRotation = true);
	static b2FixtureDef genFixtureDef(b2Shape* shape, uint16 collisionCategory, uint16 collisionMask, bool isSensor, float friction, float restitution, float density, float restitutionThreshold);
	
	enum CollisionCategories : uint16 //16 total
	{
		PLAYER = 0b1,
		ENEMY = 0b10,
		BOUNDS = 0b100,
		WALL = 0b1000
	};
private:
	b2Body* body;

	b2World* world;

	Transform* transform;

	//collision callbacks should only influence game logic through the update function
	std::forward_list<b2ContactListener> collisionCallback;
};

/// The body type.
/// static: zero mass, zero velocity, may be manually moved
/// kinematic: zero mass, non-zero velocity set by user, moved by solver
/// dynamic: positive mass, non-zero velocity determined by forces, moved by solver