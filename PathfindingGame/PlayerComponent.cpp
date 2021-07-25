#include "PlayerComponent.h"
#include "GameObject.h"
#include "Game.h"

void PlayerComponent::init(GameObject* arm)
{
	armSprite = (AnimatedSprite*)arm->getSprite();
	armSprite->setTimePerFrame(1.0f/30);
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
		moved = true;
	}
	if (IsKeyDown(KEY_A))
	{
		direction.x -= 1;
		gameObject->getSprite()->setFlipped(true);
		armSprite->setFlipped(true);
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
		if (flipped == direction.x > 0)
		{
			gameObject->getSprite()->flip();
			armSprite->flip();
			armObject->getTransform()->flipPositionX();
			flipped = !flipped;
		}
		((AnimatedSprite*)gameObject->getSprite())->play();
		//normalize() does nothing if it has a magnitude of zero
		direction.normalize();
	}
	else
	{
		((AnimatedSprite*)gameObject->getSprite())->pauseAt(0);
	}

	cooldownTimer -= Game::getDeltaTime();

	if (!pending)
	{
		switch (armState)
		{
		case WALKING:
			armSprite->setCurrentFrame(((AnimatedSprite*)gameObject->getSprite())->getCurrentFrame());
			break;
		case ATTACK_START:
			if (!IsMouseButtonDown(MOUSE_BUTTON_LEFT))
			{
				armState = ATTACK_END;
				pending = true;
				armSprite->setSettings(attackMiddle + 1, attackEnd, attackMiddle + 1);
				armSprite->setCallback(spriteCallback, this);
				spritePause = attackEnd;
				armSprite->play();
			}
			break;
		case ATTACK_END:
			armState = WALKING;
			armSprite->setSettings(walkStart, walkEnd, walkStart);
			break;
		case STEALTH_ENTER:
			armState = STEALTH;
			armSprite->setSettings(stealthWalkStart, stealthWalkEnd, stealthWalkStart);
			armSprite->play();
			acceleration = maxAcceleration * stealthSpeedMultiplier;
			speed = maxSpeed * stealthAccelerationMultiplier;
			break;
		case STEALTH:
			inStealth = true;
			armSprite->setCurrentFrame(((AnimatedSprite*)gameObject->getSprite())->getCurrentFrame() + stealthWalkStart - walkStart);
			break;
		case STEALTH_LEAVE:
			speed = maxSpeed;
			acceleration = maxAcceleration;
			armSprite->setCallback(nullptr, nullptr);
			armSprite->setSettings(walkStart, walkEnd, walkStart);
			armState = WALKING;
			break;
		}

		if (armState == WALKING && cooldownTimer <= 0 && IsMouseButtonDown(MOUSE_BUTTON_LEFT))
		{
			pending = true;
			armState = ATTACK_START;
			armSprite->setSettings(attackStart, attackMiddle, attackStart);
			armSprite->play();
			armSprite->setCallback(spriteCallback, this);
			spritePause = attackMiddle;
		}
		else if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))
		{
			if (armState == WALKING)
			{
				pending = true;
				armState = STEALTH_ENTER;
				armSprite->setSettings(stealthSwitchStart, stealthSwitchEnd, stealthSwitchStart);
				armSprite->play();
				armSprite->setCallback(spriteCallback, this);
				spritePause = stealthSwitchEnd;
			}
			else if (armState == STEALTH)
			{
				pending = true;
				armState = STEALTH_LEAVE;
				armSprite->setSettings(unstealthSwitchStart, unstealthSwitchEnd, unstealthSwitchStart);
				armSprite->play();
				armSprite->setCallback(spriteCallback, this);
				spritePause = unstealthSwitchEnd;

			}
			
		}
	}
}

void PlayerComponent::fixedUpdate()
{
	Vector2 velocity = (direction * speed - rigidBody->getVelocity());
	if (velocity.magnitudeSquared() > acceleration * acceleration * PHYSICS_TIME_STEP * PHYSICS_TIME_STEP)
		velocity = velocity.normalised() * acceleration * PHYSICS_TIME_STEP;

	if (direction.x == 0 && direction.y == 0)
		velocity = velocity / 2;

	rigidBody->addVelocity(velocity);
}

void PlayerComponent::hit(const Vector2& position)
{
	static float knockback = 15;
	health--;
	rigidBody->addVelocity((position - transform->getPosition()).normalised() * Vector2 { -knockback, knockback });
}
