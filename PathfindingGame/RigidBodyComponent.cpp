#include "RigidBodyComponent.h"

#include "CollisionManager.h"
#include "GameObject.h"

void RigidBodyComponent::init(CollisionManager* collisionManager, b2BodyDef& bodyDef, b2FixtureDef& fixtureDef, bool genShapeFromGameobject)
{
	//connect pointers
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
	b2PolygonShape shape;
	if (genShapeFromGameobject || fixtureDef.shape == nullptr)
	{
		Rectangle* destRect = gameObject->getSprite()->getDestinationRectangle();
		shape.SetAsBox(destRect->width / 200, destRect->height / 200);
		fixtureDef.shape = &shape;
	}
	body->CreateFixture(&fixtureDef);
}

//as long as this is always called before sprites it will render fine
//anything using position should access it through rigidBodyComponent
void RigidBodyComponent::fixedUpdate()
{
	b2Vec2 pos = body->GetPosition();
	transform->setRotation(-body->GetAngle());
	transform->setTransform({ pos.x * PHYSICS_UNIT_SCALE, pos.y * -PHYSICS_UNIT_SCALE });
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

void RigidBodyComponent::applyTorque(float torque)
{
	body->ApplyTorque(torque, true);
}

void RigidBodyComponent::applyForce(const Vector2& force)
{
	body->ApplyForceToCenter(reinterpret_cast<const b2Vec2&>(force), true);
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

void RigidBodyComponent::setVelocity(const Vector2& velocity)
{
	body->SetLinearVelocity(reinterpret_cast<const b2Vec2&>(velocity));
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
	body->ApplyLinearImpulse(impulse, position, true);
}

b2BodyDef RigidBodyComponent::genBodyDef(b2BodyType type, bool fixedRotation, Vector2 velocity, float angularVelocity, float angularDamping, float linearDamping)
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

b2FixtureDef RigidBodyComponent::genFixtureDef(uint16 collisionCategory, uint16 collisionMask, b2Shape* shape, bool isSensor, float friction, float restitution, float density, float restitutionThreshold)
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
