#pragma once
#include "NecessaryHeaders.h"
#include "RigidBodyComponent.h"
#include "Component.h"
#include <iostream>
#include "Sprite.h"

class GameObject;
class SmoothCamera;
class Scene;
class SwipeComponent;

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
	//other settings
	static constexpr float maxSpeed = 5;
	static constexpr float maxAcceleration = 90;
	static constexpr float stealthSpeedMultiplier = 0.5f;
	static constexpr float stealthAccelerationMultiplier = 0.8f;
	static constexpr float attackDistance = 250.0f;
	static constexpr float drawOffset = 40.0f;
	//attack settings
	static constexpr int healthMax = 3;
	static constexpr float invincibilityTime = 0.9f;
	static constexpr float invincibilityTintSpeed = 5.0f * PI;
	static constexpr float invincibilityAlphaSpeed = 15.0f * PI;
	static constexpr float knockback = 10;
	static constexpr int damage = 1;
	static constexpr float minDot = 0.6f;
	static constexpr float cooldown = 0.5f;
	
	void init(Scene* scene);
	void update();
	void fixedUpdate();
	void onTriggerEnter(RigidBodyComponent* collisionBody, b2Fixture* collisionFixture);
	void onTriggerExit(RigidBodyComponent* collisionBody, b2Fixture* collisionFixture);
	void attack();
	void hit(int damage, float knockback, const Vector2& position);
private:

	int spritePause = attackMiddle;
	static void spriteCallback(void* pointer)
	{
		((PlayerComponent*)pointer)->armSprite->pauseAt(((PlayerComponent*)pointer)->spritePause);
		((PlayerComponent*)pointer)->pending = false;
		((PlayerComponent*)pointer)->armSprite->setCallback(nullptr, nullptr);
	}

	enum PlayerState
	{
		ST_WALKING,
		ST_ATTACK_START,
		ST_ATTACK_END,
		ST_STEALTH_ENTER,
		ST_STEALTH,
		ST_STEALTH_LEAVE
	};

	bool inStealth = false;
	bool pending = false;
	std::vector<RigidBodyComponent*> attackableBees;
	SmoothCamera* camera;
	float cooldownTimer = 0;
	float invincibilityTimer = 0;
	bool invincible = false;
	bool hitFlashing = false;
	PlayerState armState = ST_WALKING;
	
	float speed = maxSpeed;
	float acceleration = maxAcceleration;
	int health = 3;
	RigidBodyComponent* rigidBody;
	
	Vector2 direction;
	AnimatedSprite* armSprite;
	GameObject* armObject;
	bool flipped = false;
	SwipeComponent* swipeEffect;
	Shader* additiveShader;
	int shaderTintLocation;
};

