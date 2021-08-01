#include "PlayerComponent.h"
#include "GameObject.h"
#include "AgentComponent.h"
#include "Game.h"
#include "SmoothCamera.h"
#include "SwipeComponent.h"
#include "PlayerUIComponent.h"
#include "Room.h"

void PlayerComponent::init(Room* scene)
{
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// 	   Initialization
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//child settings
	armObject = new GameObject(gameObject, "beearAttack", true, { 2.5f,26 });
	armSprite = (AnimatedSprite*)armObject->getSprite();
	armSprite->setSettings(WALK_START, WALK_END, 0);
	//sprite settings
	gameObject->getSprite()->setDrawOffset(gameObject->getSprite()->getDestinationRectangle()->height / 2 - DRAW_OFFSET);
	armSprite->setDrawOffset(gameObject->getSprite()->getDrawOffset());
	armSprite->setTimePerFrame(1.0f/30);
	//load death circle
	deathTexture = LoadRenderTexture(GetScreenWidth(), GetScreenHeight());
	generateAdditional(scene);
	//disable exit key
	SetExitKey(0);

}

void PlayerComponent::update()
{
	//if pause menu is open do not attempt to do anything
	if (paused)
		return;

	float dT = Game::getDeltaTime();

	//if health is zero, do death effects (death timer used by UI component)
	if (health <= 0) 
	{
		deathTimer += Game::getUnscaledDeltaTime();
		if (!dead)
		{
			dead = true;
			Game::getInstance().setTimeScale(0.0f);
		}
		return;
	}

	//clear the input direction every frame
	direction = { 0 };

	//input
	if (IsKeyDown(KEY_D))
	{
		direction.x = 1;
	}
	if (IsKeyDown(KEY_A))
	{
		direction.x -= 1;
	}
	if (IsKeyDown(KEY_W))
	{
		direction.y = 1;
	}
	if (IsKeyDown(KEY_S))
	{
		direction.y -= 1;
	}
	//if direction is != 0
	if (direction.x != 0 || direction.y != 0)
	{
		//if need to flip, flip
		if (flipped == direction.x > 0 && direction.x != 0)
		{
			gameObject->getSprite()->flip();
			armSprite->flip();
			armObject->getTransform()->flipPositionX();
			flipped = !flipped;
		}

		//play walking animation
		((AnimatedSprite*)gameObject->getSprite())->play();
		//normalise direction
		direction.normalize();
	}
	else
	{
		//if not moving, stop walk animation
		((AnimatedSprite*)gameObject->getSprite())->pauseAt(0);
	}

	//Invincibility logic here 
	if (invincible)
	{
		invincibilityTimer += Game::getDeltaTime();

		//do white flash effect here
		if (invincibilityTimer <= PI/(2 * HIT_TINT_SPEED))
		{
			float invincibilityTintAmount = cosf(invincibilityTimer * HIT_TINT_SPEED);
			unsigned char invincibilityTint = cosf(invincibilityTimer * HIT_TINT_SPEED) * 0xFF;
			armSprite->setTint({ invincibilityTint,invincibilityTint,invincibilityTint,0xFF });
			gameObject->getSprite()->setTint({ invincibilityTint,invincibilityTint,invincibilityTint,0xFF });
		}
		else
		{
			//stop white flash
			if (hitFlashing)
			{
				hitFlashing = false;
				armSprite->clearShader();
				gameObject->getSprite()->clearShader();
			}
			//do invincibility alpha wobble here
			unsigned char invincibilityAlpha = 0xCF + copysignf(1.0f, sinf(invincibilityTimer * INVINCIBILITY_ALPHA_SPEED)) * 0x30;
			armSprite->setTint({ 0xFF,0xFF,0xFF,invincibilityAlpha });
			gameObject->getSprite()->setTint({ 0xFF,0xFF,0xFF,invincibilityAlpha });
		}

		if (invincibilityTimer >= INVINCIBILITY_TIME)
		{
			//at the end, stop invincibility and clear variabales
			invincible = false;
			invincibilityTimer = 0;
			armSprite->clearShader();
			gameObject->getSprite()->clearShader();
			gameObject->getSprite()->setTint({ 0xFF,0xFF,0xFF,0xFF });
			armSprite->setTint({ 0xFF,0xFF,0xFF,0xFF });
		}
	}
	//if the mask protected you from damage, do this logic
	else if (maskProtected)
	{
		invincibilityTimer += Game::getDeltaTime();

		//do flashing
		if (invincibilityTimer <= PI / (2 * HIT_TINT_SPEED))
		{
			float invincibilityTintAmount = cosf(invincibilityTimer * HIT_TINT_SPEED);
			unsigned char invincibilityTint = cosf(invincibilityTimer * HIT_TINT_SPEED) * 0xFF;
			unsigned char invincibilityTintLower = (unsigned char)(invincibilityTint * 0.2f);
			//SetShaderValue(*shader, shaderTintLocation, &invincibilityTintAmount, SHADER_UNIFORM_FLOAT);
			armSprite->setTint({ invincibilityTintLower,invincibilityTintLower,invincibilityTint,0xFF });
			gameObject->getSprite()->setTint({ invincibilityTintLower,invincibilityTintLower,invincibilityTint,0xFF });
		}
		else
		{
			maskProtected = false;
			invincibilityTimer = 0;
			armSprite->clearShader();
			gameObject->getSprite()->clearShader();
			gameObject->getSprite()->setTint({ 0xFF,0xFF,0xFF,0xFF });
			armSprite->setTint({ 0xFF,0xFF,0xFF,0xFF });
		}
	}
	
	//state machine here
	cooldownTimer -= Game::getDeltaTime();
	if (!pending)
	{
		switch (armState)
		{
		case ST_WALKING:
			//if walking align arm sprite animation with walking animation
			armSprite->setCurrentFrame(((AnimatedSprite*)gameObject->getSprite())->getCurrentFrame());
			break;
		case ST_ATTACK_START:
			//is the case in the middle of attack animation.
			if (!IsMouseButtonDown(MOUSE_BUTTON_LEFT))
			{
				//if mouse button is not down in this state, attack
				armState = ST_ATTACK_END;
				pending = true;
				armSprite->setSettings(ATTACK_HIT + 1, ATTACK_END, ATTACK_HIT + 1);
				armSprite->setCallback(dieCallback, this);
				spritePause = ATTACK_END;
				armSprite->play();
				cooldownTimer = ATTACK_COOLDOWN;
				attack();
			}
			break;
		case ST_ATTACK_END:
			//set animation
			armState = ST_WALKING;
			armSprite->setSettings(WALK_START, WALK_END, WALK_START);
			break;
		case ST_STEALTH_ENTER:
			//is the case at start of stealth, set variables to stealth
			armState = ST_STEALTH;
			armSprite->setSettings(STEALTH_WALK_START, STEALTH_WALK_END, STEALTH_WALK_START);
			armSprite->play();
			acceleration = MAX_ACCELERATION * STEALTH_SPEED_MULTIPLIER;
			speed = MAX_SPEED * STEALTH_ACCELERATION_MULTIPLIER;
			break;
		case ST_STEALTH:
			//if in stealth align arm sprite animation with walking animation
			inStealth = true;
			armSprite->setCurrentFrame(((AnimatedSprite*)gameObject->getSprite())->getCurrentFrame() + STEALTH_WALK_START - WALK_START);
			break;
		case ST_STEALTH_LEAVE:
			//is the case at end of stealth leave, set variables to normal
			speed = MAX_SPEED;
			inStealth = false;
			acceleration = MAX_ACCELERATION;
			armSprite->setCallback(nullptr, nullptr);
			armSprite->setSettings(WALK_START, WALK_END, WALK_START);
			armState = ST_WALKING;
			break;
		}

		if (armState == ST_WALKING && cooldownTimer <= 0 && IsMouseButtonDown(MOUSE_BUTTON_LEFT))
		{
			//if can attack, initiate attack
			pending = true;
			armState = ST_ATTACK_START;
			armSprite->setSettings(ATTACK_START, ATTACK_HIT, ATTACK_START);
			armSprite->play();
			armSprite->setCallback(dieCallback, this);
			spritePause = ATTACK_HIT;
		}
		else if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))
		{
			if (armState == ST_WALKING)
			{
				//enter stealth
				pending = true;
				armState = ST_STEALTH_ENTER;
				armSprite->setSettings(STEALTH_SWITCH_START, STEALTH_SWITCH_END, STEALTH_SWITCH_START);
				armSprite->play();
				armSprite->setCallback(dieCallback, this);
				spritePause = STEALTH_SWITCH_END;
			}
			else if (armState == ST_STEALTH)
			{
				//exit stealth
				pending = true;
				armState = ST_STEALTH_LEAVE;
				armSprite->setSettings(UNSTEALTH_SWITCH_START, UNSTEALTH_SWITCH_END, UNSTEALTH_SWITCH_START);
				armSprite->play();
				armSprite->setCallback(dieCallback, this);
				spritePause = UNSTEALTH_SWITCH_END;
			}
		}
	}
}

void PlayerComponent::unload()
{
	UnloadRenderTexture(deathTexture);
}

void PlayerComponent::generateAdditional(Room* scene)
{
	//generate additional data
	//generate rigidBody
	b2BodyDef bDef = RigidBodyComponent::genBodyDef(b2_dynamicBody, true);
	b2FixtureDef fDef = RigidBodyComponent::genFixtureDef(RigidBodyComponent::PLAYER);
	b2CircleShape playerShape;
	playerShape.m_p.Set(0, DRAW_OFFSET / PHYSICS_UNIT_SCALE);
	playerShape.m_radius = 0.7f;
	fDef.shape = &playerShape;
	if (gameObject->getComponentOfType<RigidBodyComponent>() != nullptr)
	{
		gameObject->deleteComponentOfType<RigidBodyComponent>();
	}
	rigidBody = gameObject->addComponent<RigidBodyComponent>();
	rigidBody->init(scene->getCollisionManager(), bDef, fDef);
	//create sensor
	fDef.isSensor = true;
	fDef.filter.maskBits = RigidBodyComponent::ENEMY;
	playerShape.m_p.Set(0, (gameObject->getSprite()->getDestinationRectangle()->height / 2 - DRAW_OFFSET) / PHYSICS_UNIT_SCALE);
	playerShape.m_radius = ATTACK_DIST / PHYSICS_UNIT_SCALE;
	rigidBody->addFixture(fDef);
	//target camera to player
	camera = scene->getCamera();
	camera->target(transform);
	//create attack swipe object
	swipeEffect = (new GameObject(scene, "swipe", false))->addComponent<SwipeComponent>();
	swipeEffect->init();
	swipeEffect->disableComponent();
	//get shader
	additiveShader = Game::getInstance().getTextureManager()->getShader("additiveTint");
	//create UI object
	UI = (new GameObject(scene, nullptr, false, { 0 }, 1, SORTING::UI))->addComponent<PlayerUIComponent>();
	UI->init(maxHealth, health, scene, this);
}

void PlayerComponent::fixedUpdate()
{
	//move velocity toward direction
	Vector2 velocity = (direction * speed - rigidBody->getVelocity());
	if (velocity.magnitudeSquared() > acceleration * acceleration * PHYSICS_TIME_STEP * PHYSICS_TIME_STEP)
		velocity = velocity.normalised() * acceleration * PHYSICS_TIME_STEP;

	if (direction.x == 0 && direction.y == 0)
		velocity = velocity / 2;

	rigidBody->addVelocity(velocity);
}

void PlayerComponent::onTriggerEnter(RigidBodyComponent* collisionBody, b2Fixture* collisionFixture)
{
	if (!collisionFixture->IsSensor() && collisionFixture->GetFilterData().categoryBits == RigidBodyComponent::ENEMY)
	{
		//add any colliding bees to attack list
		attackableBees.push_back(collisionBody);
	}
}

void PlayerComponent::onTriggerExit(RigidBodyComponent* collisionBody, b2Fixture* collisionFixture)
{
	if (!collisionFixture->IsSensor() && collisionFixture->GetFilterData().categoryBits == RigidBodyComponent::ENEMY)
	{
		//if in bee list, delete it from list
		auto beeIter = std::find(attackableBees.begin(), attackableBees.end(), collisionBody);
		if (beeIter != attackableBees.end())
			attackableBees.erase(beeIter);
	}
}

void PlayerComponent::attack()
{
	Vector2 mousePos = camera->getCameraMousePosition();
	Vector2 startPos = transform->getPosition() + Vector2{0, -gameObject->getSprite()->getDrawOffset()};
	Vector2 direction = (mousePos - startPos).normalised();
	swipeEffect->startEffect(direction * (ATTACK_DIST/5) + startPos, direction, rigidBody->getVelocity());

	if (attackableBees.size() > 0)
	{
		AgentComponent* beeComponent;
		Vector2 playerPos = transform->getPosition();
		Vector2 deltaCamera = (playerPos - mousePos);

		for (int i = 0; i < attackableBees.size(); i++)
		{
			Vector2 beePos = attackableBees[i]->getTransform()->getPosition();
			Vector2 deltaBee = (playerPos - beePos);
			
			if (Vector2::dot(deltaCamera.normalised(), deltaBee.normalised()) > MIN_ATTACK_DOT)
			{
				beeComponent = attackableBees[i]->getGameObject()->getComponentOfType<AgentComponent>();
				if (beeComponent != nullptr)
				{
					beeComponent->hit(DAMAGE, KNOCKBACK, transform->getPosition());
				}
			}
		}
	}
}

void PlayerComponent::hit(int damage, float knockback, const Vector2& position)
{
	//if invincible or dead cancel hiot
	if (invincible || health <= 0)
		return;

	//if health is at one, this will kill the character. therefore do not do hit flash effect as it will make the character white for the entire death transition
	if (health != 1)
	{
		armSprite->setShader(additiveShader);
		gameObject->getSprite()->setShader(additiveShader);
	}

	// if wearing mask you have a 50% chance of not getting damaged
	if (inStealth && rand() % 2 == 0)
	{
		maskProtected = true;
		invincibilityTimer = 0;
		return;
	}

	//take damage and become invincible temporarily
	health -= damage;
	UI->hit(damage);
	Vector2 knockbackVector = (position - transform->getPosition()).normalised() * knockback;
	knockbackVector.x *= -1;
	rigidBody->addVelocity(knockbackVector);
	invincibilityTimer = 0;
	invincible = true;
	hitFlashing = true;
}

void PlayerComponent::setHealth(int health)
{
	this->health = health;
}

int PlayerComponent::getHealth()
{
	return health;
}

void PlayerComponent::setMaxHealth(int health)
{
	maxHealth = health;
}

int PlayerComponent::getMaxHealth()
{
	return maxHealth;
}

void PlayerComponent::pause()
{	paused = true; Game::getInstance().setTimeScale(0);		}

void PlayerComponent::resume()
{	paused = false; Game::getInstance().setTimeScale(1);	}
