#include "SwipeComponent.h"
#include "Game.h"
#include "GameObject.h"

void SwipeComponent::start()
{
	sprite = gameObject->getSprite();
}

void SwipeComponent::update()
{
	if (doingEffect)
	{
		float percentLeft = (effectTimer / effectTime);
		//quint easing
		transform->setPosition(Vector2::lerp(startGoal, endGoal, 1 - (percentLeft* percentLeft* percentLeft)));
		sprite->setTint({ 0xFF,0xFF,0xFF, (unsigned char)(0xFF * (percentLeft * percentLeft * percentLeft) )});
		effectTimer -= Game::getDeltaTime();
		if (effectTimer <= 0)
		{
			gameObject->setIsDrawn(false);
			sprite->setTint({ 0xFF,0xFF,0xFF, 0 });
			disableComponent();
			doingEffect = false;
		}
	}
}

void SwipeComponent::startEffect(Vector2 startPos, Vector2 direction, Vector2 velocity)
{
	startGoal = startPos;
	velocity.y *= -1;
	endGoal = direction * effectDistance + startGoal + (velocity * PHYSICS_UNIT_SCALE/5);
	transform->setRotation(direction.getAngle(Vector2{ 1,0 }));
	effectTimer = effectTime;
	doingEffect = true;
	gameObject->setIsDrawn(true);
	enableComponent();
}
