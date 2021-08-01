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
	void unload();
	//generate UI, swipe effect and rigidbody
	void generateAdditional(Room* scene);
	void fixedUpdate();
	void onTriggerEnter(RigidBodyComponent* collisionBody, b2Fixture* collisionFixture);
	void onTriggerExit(RigidBodyComponent* collisionBody, b2Fixture* collisionFixture);
	
	//called to attack
	void attack();
	//called when hit
	void hit(int damage, float knockback, const Vector2& position);

	//getters and setters
	void setHealth(int health);
	int getHealth();
	void setMaxHealth(int health);
	int getMaxHealth();
	inline void addFloor() { floorNumber++; }
	inline int getFloor() { return floorNumber; }
	inline bool hasMaskOn() { return inStealth; }
	inline bool isDying() { return dead; }
	inline float getDyingTimer() { return deathTimer; }
	inline RenderTexture2D* getDeathTexture() { return &deathTexture;  }
	inline PlayerUIComponent* getUI() { return UI; }
	
	//timy whimy stuff
	void pause() ;
	void resume();
private:

	//Sprite callback stuff
	int spritePause = ATTACK_HIT;
	static void dieCallback(void* pointer)
	{
		((PlayerComponent*)pointer)->armSprite->pauseAt(((PlayerComponent*)pointer)->spritePause);
		((PlayerComponent*)pointer)->pending = false;
		((PlayerComponent*)pointer)->armSprite->setCallback(nullptr, nullptr);
	}

	//Player state machine 
	enum PlayerState : char
	{
		ST_WALKING,
		ST_ATTACK_START,
		ST_ATTACK_END,
		ST_STEALTH_ENTER,
		ST_STEALTH,
		ST_STEALTH_LEAVE
	};

	//texture for death circle
	RenderTexture2D deathTexture;

	//list of all bees in attack distance at any moment
	std::vector<RigidBodyComponent*> attackableBees;

	//pointer to camera
	SmoothCamera* camera;
	//pointer to UI component
	PlayerUIComponent* UI;
	//self's rigidbody
	RigidBodyComponent* rigidBody;
	//the arm's sprite
	AnimatedSprite* armSprite;
	//the arm's object
	GameObject* armObject;
	//the swipe effect component
	SwipeComponent* swipeEffect;
	//the shader used for flashing white when damaged
	Shader* additiveShader;
	//the direction of input
	Vector2 direction;

	//attack cooldown
	float cooldownTimer = 0;
	//used for death effects
	float deathTimer = 0;
	//invincibility time
	float invincibilityTimer = 0;
	//movement data
	float speed = MAX_SPEED;
	float acceleration = MAX_ACCELERATION;
	//health data
	int health = MAX_HEALTH;
	int maxHealth = MAX_HEALTH;
	//the current floor
	int floorNumber = 1;

	//current state
	PlayerState armState = ST_WALKING;
	//if is invincible
	bool invincible = false;
	//if mask is protecting from damage
	bool maskProtected = false;
	//if is dead
	bool dead = false;
	//if mask is on
	bool inStealth = false;
	//if pause menu is open
	bool paused = false;
	//if waiting for arm animation
	bool pending = false;
	//if flashing from getting hit
	bool hitFlashing = false;
	//if sprites are flipped
	bool flipped = false;
};

