#include "PlayerComponent.h"
#include "GameObject.h"
#include "Game.h"

void PlayerComponent::init(float maxSpeed, float maxAcceleration)
{
	this->maxAcceleration = maxAcceleration;
	this->maxSpeed = maxSpeed;
}

void PlayerComponent::start()
{
	rigidBody = gameObject->getComponentOfType<RigidBodyComponent>();
}

void PlayerComponent::update()
{
	float dT = Game::getDeltaTime();

	bool moved = false;
	direction = { 0 };

	if (IsKeyDown(KEY_D))
	{
		direction.x = 1;
		gameObject->getSprite()->setFlipped(false);
		moved = true;
	}
	if (IsKeyDown(KEY_A))
	{
		direction.x -= 1;
		gameObject->getSprite()->setFlipped(true);
		moved = true;
	}
	if (IsKeyDown(KEY_W))
	{
		direction.y = 1;
		moved = true;
	}
	if (IsKeyDown(KEY_S))
	{
		direction.y -= 1;
		moved = true;
	}
	if (moved)
	{
		//normalize() does nothing if it has a magnitude of zero
		direction.normalize();
	}
}

void PlayerComponent::fixedUpdate()
{
	Vector2 velocity = (direction * maxSpeed - rigidBody->getVelocity());
	if (velocity.magnitudeSquared() > maxAcceleration * maxAcceleration * PHYSICS_TIME_STEP * PHYSICS_TIME_STEP)
		velocity = velocity.normalised() * maxAcceleration * PHYSICS_TIME_STEP;

	if (direction.x == 0 && direction.y == 0)
		velocity = velocity / 2;

	rigidBody->addVelocity(velocity);
}
