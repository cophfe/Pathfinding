#include "PlayerComponent.h"
#include "GameObject.h"
#include "Game.h"
#include "Sprite.h"

void PlayerComponent::init(float maxSpeed, float maxAcceleration, GameObject* arm)
{
	this->maxAcceleration = maxAcceleration;
	this->maxSpeed = maxSpeed;
	armSprite = (AnimatedSprite*)arm->getSprite();
	armObject = arm;
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
		if (flipped)
		{
			gameObject->getSprite()->flip();
			armSprite->flip();
			armObject->getTransform()->flipPositionX();
			flipped = false;
		}
		moved = true;
	}
	if (IsKeyDown(KEY_A))
	{
		direction.x -= 1;
		gameObject->getSprite()->setFlipped(true);
		armSprite->setFlipped(true);
		if (!flipped)
		{
			gameObject->getSprite()->flip();
			armSprite->flip();
			armObject->getTransform()->flipPositionX();
			flipped = true;
		}
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
		((AnimatedSprite*)gameObject->getSprite())->play();
		armSprite->play();
		direction.normalize();
	}
	else
	{
		((AnimatedSprite*)gameObject->getSprite())->pauseAt(0);
		armSprite->pauseAt(0);
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
