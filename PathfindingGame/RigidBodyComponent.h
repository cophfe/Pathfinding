#pragma once
#include "Component.h"
#include "NecessaryHeaders.h"
#include "ContactListener.h"

class GameObject;
class CollisionManager;

//PHYSICS OBJECT
//UNDEFINED BEHAVIOUR WHEN ON A CHILDED OBJECT
//ALSO MIGHT BREAK WHEN NOT THE FIRST COMPONENT IN THE COMPONENT LIST (add it last)
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
	void setPosition(Vector2 position, float angle);
	void setPosition(float x, float y, float angle);
	void addImpulse(Vector2 impulse, Vector2 position);
	inline const Vector2& getVelocity() { return reinterpret_cast<const Vector2&>(body->GetLinearVelocity()); }

	static b2BodyDef genBodyDef(b2BodyType Type, bool fixedRotation = false, float angularDamping = 0, float linearDamping = 0);
	static b2FixtureDef genFixtureDef(uint16 collisionCategory, uint16 collisionMask = ALL, b2Shape* shape = nullptr, bool isSensor = false, float friction = 0.0f, float restitution = 0.0f, float density = 1.0f, float restitutionThreshold = 0);
	enum CollisionCategories : uint16 //16 total
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
	void OnCollisionEnter(RigidBodyComponent* collisionBody, b2Manifold* manifold);
	void OnCollisionLeave(RigidBodyComponent* collisionBody, b2Manifold* manifold);

	friend ContactListener;
};

/// The body Type.
/// static: zero mass, zero velocity, may be manually moved
/// kinematic: zero mass, non-zero velocity set by user, moved by solver
/// dynamic: positive mass, non-zero velocity determined by forces, moved by solver