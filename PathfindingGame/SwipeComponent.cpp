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
		gameObject->setIsDrawn(true);
		//ease between the two positions
		float percentLeft = (effectTimer / effectTime);
		//quint easing
		transform->setPosition(Vector2::lerp(startGoal, endGoal, 1 - (percentLeft* percentLeft* percentLeft)));
		sprite->setTint({ 0xFF,0xFF,0xFF, (unsigned char)(0xFF * (percentLeft * percentLeft * percentLeft) )});
		effectTimer -= Game::getDeltaTime();
		if (effectTimer <= 0)
		{
			sprite->setTint({ 0xFF,0xFF,0xFF, 0 });
			doingEffect = false;
			gameObject->setIsDrawn(false);
			disableComponent();
		}
	}
}

void SwipeComponent::startEffect(Vector2 startPos, Vector2 direction, Vector2 velocity)
{
	//start effect
	//first calculate end goal (based on direction, start position and velocity)
	startGoal = startPos;
	velocity.y *= -1;
	endGoal = direction * effectDistance + startGoal + (velocity * PHYSICS_UNIT_SCALE/5);
	//then rotate gameObject to match direction angle
	transform->setRotation(direction.getAngle(Vector2{ 1,0 }));
	//then reset effect values
	effectTimer = effectTime;
	doingEffect = true;
	sprite->setTint({ 0xFF,0xFF,0xFF, 0 });
	//then enable everything
	enableComponent();
}
