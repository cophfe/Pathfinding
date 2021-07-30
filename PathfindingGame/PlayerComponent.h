#pragma once
#include "NecessaryHeaders.h"
#include "RigidBodyComponent.h"
#include "Component.h"
#include <iostream>
#include "Sprite.h"

class GameObject;
class SmoothCamera;
class Scene;
class Room;
class SwipeComponent;
class PlayerUIComponent;

class PlayerComponent : public Component
{
public:
	//frames:
	static constexpr int WALK_START = 0;
	static constexpr int WALK_END = 11;
	static constexpr int ATTACK_START = 12;
	static constexpr int ATTACK_HIT = 15;
	static constexpr int ATTACK_END = 19;
	static constexpr int STEALTH_SWITCH_START = 20;
	static constexpr int STEALTH_SWITCH_END = 26;
	static constexpr int STEALTH_WALK_START = 27;
	static constexpr int STEALTH_WALK_END = 38;
	static constexpr int UNSTEALTH_SWITCH_START = 39;
	static constexpr int UNSTEALTH_SWITCH_END = 45;
	//other settings
	static constexpr float MAX_SPEED = 5;
	static constexpr float MAX_ACCELERATION = 90;
	static constexpr float STEALTH_SPEED_MULTIPLIER = 0.5f;
	static constexpr float STEALTH_ACCELERATION_MULTIPLIER = 0.8f;
	static constexpr float ATTACK_DIST = 250.0f;
	static constexpr float DRAW_OFFSET = 40.0f;
	//attack settings
	static constexpr int MAX_HEALTH = 3;
	static constexpr float INVINCIBILITY_TIME = 0.9f;
	static constexpr float HIT_TINT_SPEED = 2.5f * PI;
	static constexpr float INVINCIBILITY_ALPHA_SPEED = 15.0f * PI;
	static constexpr float KNOCKBACK = 10;
	static constexpr int DAMAGE = 1;
	static constexpr float MIN_ATTACK_DOT = 0.6f;
	static constexpr float ATTACK_COOLDOWN = 0.5f;
	
	void init(Room* scene);
	void update();
	void generateAdditional(Room* scene);
	void fixedUpdate();
	void onTriggerEnter(RigidBodyComponent* collisionBody, b2Fixture* collisionFixture);
	void onTriggerExit(RigidBodyComponent* collisionBody, b2Fixture* collisionFixture);
	void attack();
	void hit(int damage, float knockback, const Vector2& position);
	inline bool hasMaskOn() { return inStealth; }

private:

	int spritePause = ATTACK_HIT;
	static void dieCallback(void* pointer)
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
	
	float speed = MAX_SPEED;
	float acceleration = MAX_ACCELERATION;
	int health = 3;
	PlayerUIComponent* UI;
	RigidBodyComponent* rigidBody;

	Vector2 direction;
	AnimatedSprite* armSprite;
	GameObject* armObject;
	bool flipped = false;
	SwipeComponent* swipeEffect;
	Shader* additiveShader;
};

