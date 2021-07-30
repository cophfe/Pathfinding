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
	armObject = new GameObject(gameObject, "beearAttack", true, { 3,24 });
	armSprite = (AnimatedSprite*)armObject->getSprite();
	armSprite->setSettings(WALK_START, WALK_END, 0);
	//sprite settings
	gameObject->getSprite()->setDrawOffset(gameObject->getSprite()->getDestinationRectangle()->height / 2 - DRAW_OFFSET);
	armSprite->setDrawOffset(gameObject->getSprite()->getDrawOffset());
	armSprite->setTimePerFrame(1.0f/30);

	generateAdditional(scene);
}

void PlayerComponent::update()
{
	float dT = Game::getDeltaTime();

	direction = { 0 };

	//
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

	if (direction.x != 0 || direction.y != 0)
	{
		if (flipped == direction.x > 0 && direction.x != 0)
		{
			gameObject->getSprite()->flip();
			armSprite->flip();
			armObject->getTransform()->flipPositionX();
			flipped = !flipped;
		}
		((AnimatedSprite*)gameObject->getSprite())->play();
		direction.normalize();
	}
	else
	{
		((AnimatedSprite*)gameObject->getSprite())->pauseAt(0);
	}

	//Invincibility logic here 
	if (invincible)
	{
		invincibilityTimer += Game::getDeltaTime();
				
		if (invincibilityTimer <= PI/(2 * HIT_TINT_SPEED))
		{
			float invincibilityTintAmount = cosf(invincibilityTimer * HIT_TINT_SPEED);
			unsigned char invincibilityTint = cosf(invincibilityTimer * HIT_TINT_SPEED) * 0xFF;
			//SetShaderValue(*shader, shaderTintLocation, &invincibilityTintAmount, SHADER_UNIFORM_FLOAT);
			armSprite->setTint({ invincibilityTint,invincibilityTint,invincibilityTint,0xFF });
			gameObject->getSprite()->setTint({ invincibilityTint,invincibilityTint,invincibilityTint,0xFF });
		}
		else
		{
			if (hitFlashing)
			{
				hitFlashing = false;
				armSprite->clearShader();
				gameObject->getSprite()->clearShader();
			}
			unsigned char invincibilityAlpha = 0xCF + copysignf(1.0f, sinf(invincibilityTimer * INVINCIBILITY_ALPHA_SPEED)) * 0x30;
			armSprite->setTint({ 0xFF,0xFF,0xFF,invincibilityAlpha });
			gameObject->getSprite()->setTint({ 0xFF,0xFF,0xFF,invincibilityAlpha });
		}

		if (invincibilityTimer >= INVINCIBILITY_TIME)
		{
			invincible = false;
			invincibilityTimer = 0;
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
			armSprite->setCurrentFrame(((AnimatedSprite*)gameObject->getSprite())->getCurrentFrame());
			break;
		case ST_ATTACK_START:
			if (!IsMouseButtonDown(MOUSE_BUTTON_LEFT))
			{
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
			armState = ST_WALKING;
			armSprite->setSettings(WALK_START, WALK_END, WALK_START);
			break;
		case ST_STEALTH_ENTER:
			armState = ST_STEALTH;
			armSprite->setSettings(STEALTH_WALK_START, STEALTH_WALK_END, STEALTH_WALK_START);
			armSprite->play();
			acceleration = MAX_ACCELERATION * STEALTH_SPEED_MULTIPLIER;
			speed = MAX_SPEED * STEALTH_ACCELERATION_MULTIPLIER;
			break;
		case ST_STEALTH:
			inStealth = true;
			armSprite->setCurrentFrame(((AnimatedSprite*)gameObject->getSprite())->getCurrentFrame() + STEALTH_WALK_START - WALK_START);
			break;
		case ST_STEALTH_LEAVE:
			speed = MAX_SPEED;
			acceleration = MAX_ACCELERATION;
			armSprite->setCallback(nullptr, nullptr);
			armSprite->setSettings(WALK_START, WALK_END, WALK_START);
			armState = ST_WALKING;
			break;
		}

		if (armState == ST_WALKING && cooldownTimer <= 0 && IsMouseButtonDown(MOUSE_BUTTON_LEFT))
		{
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
				pending = true;
				armState = ST_STEALTH_ENTER;
				armSprite->setSettings(STEALTH_SWITCH_START, STEALTH_SWITCH_END, STEALTH_SWITCH_START);
				armSprite->play();
				armSprite->setCallback(dieCallback, this);
				spritePause = STEALTH_SWITCH_END;
			}
			else if (armState == ST_STEALTH)
			{
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
	camera->Target(transform);
	//create attack swipe object
	swipeEffect = (new GameObject(scene, "swipe", false))->addComponent<SwipeComponent>();
	swipeEffect->init();
	swipeEffect->disableComponent();
	//get shader
	additiveShader = Game::getInstance().getTextureManager()->getShader("additiveTint");
	//create UI object
	UI = (new GameObject(scene, nullptr, false, { 0 }, 1, SORTING::UI))->addComponent<PlayerUIComponent>();
	UI->init(MAX_HEALTH, health, scene);
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

void PlayerComponent::onTriggerEnter(RigidBodyComponent* collisionBody, b2Fixture* collisionFixture)
{
	if (!collisionFixture->IsSensor() && collisionFixture->GetFilterData().categoryBits == RigidBodyComponent::ENEMY)
	{
		attackableBees.push_back(collisionBody);
	}
}

void PlayerComponent::onTriggerExit(RigidBodyComponent* collisionBody, b2Fixture* collisionFixture)
{
	if (!collisionFixture->IsSensor() && collisionFixture->GetFilterData().categoryBits == RigidBodyComponent::ENEMY)
	{
		auto beeIter = std::find(attackableBees.begin(), attackableBees.end(), collisionBody);
		if (beeIter != attackableBees.end())
			attackableBees.erase(beeIter);
	}
}

void PlayerComponent::attack()
{
	Vector2 mousePos = camera->GetCameraMousePosition();
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
	if (invincible)
	{
		return;
	}
	armSprite->setShader(additiveShader);
	gameObject->getSprite()->setShader(additiveShader);
	health -= damage;
	UI->hit(damage);
	Vector2 knockbackVector = (position - transform->getPosition()).normalised() * knockback;
	knockbackVector.x *= -1;
	rigidBody->addVelocity(knockbackVector);
	invincibilityTimer = 0;
	invincible = true;
	hitFlashing = true;
}
