#include "PlayerComponent.h"
#include "GameObject.h"
#include "Game.h"

void PlayerComponent::init(float speed, float rotationalSpeed, GameObject* connected)
{
	this->speed = speed;
	this->rotationalSpeed = rotationalSpeed;
	Component::init(connected);
}

void PlayerComponent::update()
{
	Vector2 movement = { 0 };
	float dT = Game::getDeltaTime();
	bool isMoving = false;

	if (IsKeyDown(KEY_D))
	{
		isMoving = true;
		movement.x = speed;
	}
	if (IsKeyDown(KEY_A))
	{
		isMoving = true;
		movement.x -= speed;
	}
	if (IsKeyDown(KEY_W))
	{
		isMoving = true;
		movement.y = -speed;
	}
	if (IsKeyDown(KEY_S))
	{
		isMoving = true;
		movement.y += speed;
	}
	if (IsKeyDown(KEY_Q))
	{
		transform->addRotation(-rotationalSpeed * Game::getDeltaTime());
	}
	if (IsKeyDown(KEY_E))
	{
		transform->addRotation(rotationalSpeed * Game::getDeltaTime());
	}
	if (IsKeyPressed(KEY_UP))
	{
		transform->addScale(0.125f);
	}
	if (IsKeyPressed(KEY_DOWN))
	{
		transform->addScale(-0.125f);
	}
	if (IsKeyPressed(KEY_F))
	{
		gameObject->getSprite().flip();
	}
	if (isMoving)
		transform->addPosition(movement * Game::getDeltaTime());
}
