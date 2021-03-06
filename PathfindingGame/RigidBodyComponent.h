#pragma once
#include "Component.h"
#include "NecessaryHeaders.h"
#include "ContactListener.h"

class GameObject;
class CollisionManager;

//Used for physics. has a b2body 'attached'
class RigidBodyComponent : public Component
{
public:
	void init(CollisionManager* collisionManager, b2BodyDef& bodyDef, b2FixtureDef& fixtureDef, bool genShapeFromGameobject = false);

	void fixedUpdate();
	void onDisable();
	void onEnable();
	void unload();

	void applyTorque(float torque);
	void applyForce(const Vector2& force);
	void applyForce(float x, float y);
	void addVelocity(Vector2 velocity);
	void addVelocity(float x, float y);
	void setVelocity(const Vector2& velocity);
	void setVelocity(float x, float y);
	void setPosition(Vector2 position, float angle = 0);
	void setPosition(float x, float y, float angle = 0);
	void addImpulse(Vector2 impulse, Vector2 position);
	inline const Vector2& getVelocity() { return reinterpret_cast<const Vector2&>(body->GetLinearVelocity()); }

	//add another fixture to body
	b2Fixture* addFixture(b2FixtureDef& fixtureDef);
	inline b2Body* getBody() { return body; }
	//generate a body definition (needed to make a body)
	static b2BodyDef genBodyDef(b2BodyType type, bool fixedRotation = false, float angularDamping = 0, float linearDamping = 0);
	//generate a fixture definition (needed to make a fixture)
	static b2FixtureDef genFixtureDef(uint16 collisionCategory, uint16 collisionMask = ALL, b2Shape* shape = nullptr, bool isSensor = false, float friction = 0.0f, float restitution = 0.0f, float density = 1.0f, float restitutionThreshold = 0);
	
	enum CollisionCategories : uint16 //16 possible categories
	{
		PLAYER = 0b1,
		ENEMY = 0b10,
		BOUNDS = 0b100,
		WALL = 0b1000,
		ALL = 0b1111
	};
private:
	b2Body* body;
	b2World* world;

	//collision callbacks
	void OnCollisionEnter(RigidBodyComponent* collisionBody, b2Fixture* collisionFixture);
	void OnCollisionLeave(RigidBodyComponent* collisionBody, b2Fixture* collisionFixture);
	void OnTriggerEnter(RigidBodyComponent* collisionBody, b2Fixture* collisionFixture);
	void OnTriggerLeave(RigidBodyComponent* collisionBody, b2Fixture* collisionFixture);

	friend ContactListener;
};

// Body type meaning:
// static: zero mass, zero velocity, may be manually moved
// kinematic: zero mass, non-zero velocity set by user, moved by solver
// dynamic: positive mass, non-zero velocity determined by forces, moved by solver

// note: dynamic-static collisions are continuous 