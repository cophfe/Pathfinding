#pragma once
#include "NecessaryHeaders.h"
#include "Component.h"
#include "Sprite.h"

//used for the swipe effect after attacking
class SwipeComponent
	: public Component
{
public:
	static constexpr float effectTime = 0.5f;
	static constexpr float effectDistance = 200.0f;

	void start();
	virtual void update();
	void startEffect(Vector2 startPos, Vector2 endPos, Vector2 velocity);

private:
	Sprite* sprite;
	float effectTimer = 0;
	bool doingEffect = false;
	Vector2 startGoal;
	Vector2 endGoal;
};

