#pragma once
#include "NecessaryHeaders.h"
class GameObject;
#include "Transform.h"
class RigidBodyComponent;

//The base component class
class Component
{
public:
	Component() {}
	virtual ~Component() = default;
	//every component has a unique init function. it's usually not overloaded, instead replaced by a new init function with parameters
	virtual void init() {}
	//the start function is run no matter if a component is enabled or disabled after all components are initialized in the scene
	virtual void start() {}
	//this is run every frame
	virtual void update() {}
	//this is run every fixed time interval (PHYSICS_TIME_STEP)
	virtual void fixedUpdate() {}
	//this is run after disabling a component
	virtual void onDisable() {}
	//this is run after enabling a component
	virtual void onEnable() {}
	//this is run on every component before it is deleted. it is an alternative to the destructor
	virtual void unload() {}

	//These functions only run if a RigidBodyComponent is attached
	//They are pretty self explanitory
	//DO NOT MODIFY PHYSICS STUFF INSIDE OF THESE
	virtual void onCollisionEnter(RigidBodyComponent* collisionBody, b2Fixture* collisionFixture) {}
	virtual void onCollisionExit(RigidBodyComponent* collisionBody, b2Fixture* collisionFixture) {}
	virtual void onTriggerEnter(RigidBodyComponent* collisionBody, b2Fixture* collisionFixture) {}
	virtual void onTriggerExit(RigidBodyComponent* collisionBody, b2Fixture* collisionFixture) {}

#ifdef DRAW_DEBUG
	//if a component needs to draw something, this is the only way to do it.
	virtual void debugDraw() {};
#endif // DRAW_DEBUG

	//this is run by the attached GameObject once after initiation
	void setGameObject(GameObject* connected);

	//these are used to disable and enable the component
	void disableComponent();
	void enableComponent();

	//getter functions
	inline bool isEnabled() { return enabled; }
	inline Transform* getTransform() { return transform; }
	inline GameObject* getGameObject() { return gameObject; }

protected:

	GameObject* gameObject;
	Transform* transform;
	bool enabled;
};

