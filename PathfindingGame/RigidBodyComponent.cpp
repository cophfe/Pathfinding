#include "RigidBodyComponent.h"

#include "CollisionManager.h"
#include "GameObject.h"

void RigidBodyComponent::init(GameObject* connected, CollisionManager* collisionManager, b2BodyDef& bodyDef, b2FixtureDef& fixtureDef)
{
	//connect pointers
	transform = connected->getTransform();
	world = collisionManager->getWorld();
	
	//Creating body
	//first define body settings
	bodyDef.position.Set(transform->getPosition().x, transform->getPosition().y);
	bodyDef.angle = transform->getRotation();
	//this one ensures the pointer is always correct
	bodyDef.userData.pointer = reinterpret_cast<uintptr_t>(&body);
	//now create the body
	body = world->CreateBody(&bodyDef);
	//body still needs a fixture (holds shape and a lot of important things)
	//first define fixture settings
}

//as long as this is always called before sprites it will render fine
//anything using position should access it through rigidBodyComponent
void RigidBodyComponent::update()
{
	b2Vec2 pos = body->GetPosition();
	transform->setRotation(body->GetAngle());
	transform->setTransform({ pos.x * RENDER_SCALE, pos.y * RENDER_SCALE });
}

void RigidBodyComponent::onDisable()
{
	body->SetAwake(false);
}

void RigidBodyComponent::onEnable()
{
	body->SetAwake(true);
}

void RigidBodyComponent::unload()
{
	if (world != nullptr)
	{
		world->DestroyBody(body);
		body = nullptr;
	}
}

void RigidBodyComponent::applyForce(Vector2 force)
{
	body->ApplyForceToCenter(force, true);
}

void RigidBodyComponent::applyForce(float x, float y)
{
	body->ApplyForceToCenter({ x,y }, true);
}

void RigidBodyComponent::addVelocity(Vector2 velocity)
{
	body->SetLinearVelocity(body->GetLinearVelocity() + velocity);
}

void RigidBodyComponent::addVelocity(float x, float y)
{
	addVelocity({ x,y });
}

void RigidBodyComponent::setVelocity(Vector2 velocity)
{
	body->SetLinearVelocity(velocity);
}

void RigidBodyComponent::setVelocity(float x, float y)
{
	body->SetLinearVelocity({ x,y });
}

void RigidBodyComponent::setTransform(Vector2 position, float angle)
{
	body->SetTransform(position, angle);
}

void RigidBodyComponent::setTransform(float x, float y, float angle)
{
	body->SetTransform({ x,y }, angle);
}

void RigidBodyComponent::addImpulse(Vector2 impulse, Vector2 position)
{
}

b2BodyDef RigidBodyComponent::genBodyDef(b2BodyType type, Vector2 velocity, float angularVelocity, float angularDamping, float linearDamping, bool fixedRotation)
{
	b2BodyDef def;
	def.linearVelocity = velocity;
	def.angularVelocity = angularVelocity;
	def.angularDamping = angularDamping;
	def.linearDamping = linearDamping;
	def.fixedRotation = fixedRotation;
	def.type = type;
	return def;
}

b2FixtureDef RigidBodyComponent::genFixtureDef(b2Shape* shape, uint16 collisionCategory, uint16 collisionMask, bool isSensor, float friction, float restitution, float density, float restitutionThreshold)
{
	b2FixtureDef def;
	def.shape = shape;
	def.density = density;
	def.friction = friction;
	def.restitution = restitution;
	def.restitutionThreshold = restitutionThreshold;
	def.isSensor = isSensor;
	b2Filter filter;
	filter.categoryBits = collisionCategory;
	filter.maskBits = collisionMask;
	def.filter = filter;
	return def;
}
