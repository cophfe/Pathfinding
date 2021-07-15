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

	movement = { 0 };

	if (IsKeyDown(KEY_D))
	{
		movement.x = 1;
		gameObject->getSprite()->setFlipped(false);
	}
	if (IsKeyDown(KEY_A))
	{
		movement.x -= 1;
		gameObject->getSprite()->setFlipped(true);
	}
	if (IsKeyDown(KEY_W))
	{
		movement.y = 1;
	}
	if (IsKeyDown(KEY_S))
	{
		movement.y -= 1;
	}
	
}

void PlayerComponent::fixedUpdate()
{
	movement.normalize();
	movement = movement * acceleration;

	rigidBody->applyForce(movement);
}
