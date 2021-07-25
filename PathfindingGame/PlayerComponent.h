#pragma once
#include "NecessaryHeaders.h"
#include "RigidBodyComponent.h"
#include "Component.h"
#include <iostream>
#include "Sprite.h"

class GameObject;

class PlayerComponent : public Component
{
public:
	//frames:
	static constexpr int walkStart = 0;
	static constexpr int walkEnd = 11;
	static constexpr int attackStart = 12;
	static constexpr int attackMiddle = 15;
	static constexpr int attackEnd = 19;
	static constexpr int stealthSwitchStart = 20;
	static constexpr int stealthSwitchEnd = 26;
	static constexpr int stealthWalkStart = 27;
	static constexpr int stealthWalkEnd = 38;
	static constexpr int unstealthSwitchStart = 39;
	static constexpr int unstealthSwitchEnd = 45;

	static constexpr float maxSpeed = 5;
	static constexpr float maxAcceleration = 90;
	static constexpr float stealthSpeedMultiplier = 0.5f;
	static constexpr float stealthAccelerationMultiplier = 0.8f;

	void init(GameObject* arm);
	void start();
	void update();
	void fixedUpdate();
	void hit(const Vector2& position);
private:

	int spritePause = attackMiddle;
	static void spriteCallback(void* pointer)
	{
		((PlayerComponent*)pointer)->armSprite->pauseAt(((PlayerComponent*)pointer)->spritePause);
		((PlayerComponent*)pointer)->pending = false;
		((PlayerComponent*)pointer)->armSprite->setCallback(nullptr, nullptr);
	}

	enum ArmState
	{
		WALKING,
		ATTACK_START,
		ATTACK_END,
		STEALTH_ENTER,
		STEALTH,
		STEALTH_LEAVE
	};
	bool inStealth = false;
	bool pending = false;
	const float cooldown = 1;
	float cooldownTimer = 0;
	ArmState armState = WALKING;

	float speed = maxSpeed;
	float acceleration = maxAcceleration;
	int health = 3;
	RigidBodyComponent* rigidBody;
	
	Vector2 direction;
	AnimatedSprite* armSprite;
	GameObject* armObject;
	bool flipped = false;
};

