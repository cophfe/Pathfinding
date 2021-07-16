#include "PlayerComponent.h"
#include "GameObject.h"
#include "Game.h"

void PlayerComponent::init(float acceleration)
{
	this->acceleration = acceleration;
}

void PlayerComponent::start()
{
	rigidBody = gameObject->getComponentOfType<RigidBodyComponent>();
}

void PlayerComponent::update()
{
	float dT = Game::getDeltaTime();

	direction = { 0 };

	if (IsKeyDown(KEY_D))
	{
		direction.x = 1;
		gameObject->getSprite()->setFlipped(false);
	}
	if (IsKeyDown(KEY_A))
	{
		direction.x -= 1;
		gameObject->getSprite()->setFlipped(true);
	}
	if (IsKeyDown(KEY_W))
	{
		direction.y = 1;
	}
	if (IsKeyDown(KEY_S))
	{
		direction.y -= 1;
	}
	
}

void PlayerComponent::fixedUpdate()
{
	//normalize() does nothing if it has a magnitude of zero
	direction.normalize();
	Vector2 movement = direction - rigidBody->getVelocity();

	rigidBody->applyForce(direction * acceleration);
}
