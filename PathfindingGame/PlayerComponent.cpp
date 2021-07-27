#include "PlayerComponent.h"
#include "GameObject.h"
#include "AgentComponent.h"
#include "Game.h"
#include "SmoothCamera.h"
#include "SwipeComponent.h"

void PlayerComponent::init(Scene* scene)
{
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// 	   Initialization
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//child settings
	armObject = new GameObject(gameObject, "beearAttack", true, { 3,24 });
	armSprite = (AnimatedSprite*)armObject->getSprite();
	armSprite->setSettings(walkStart, walkEnd, 0);
	//sprite settings
	gameObject->getSprite()->setDrawOffset(gameObject->getSprite()->getDestinationRectangle()->height / 2 - drawOffset);
	armSprite->setDrawOffset(gameObject->getSprite()->getDrawOffset());
	armSprite->setTimePerFrame(1.0f/30);
	//generate rigidBody
	b2BodyDef bDef = RigidBodyComponent::genBodyDef(b2_dynamicBody, true);
	b2FixtureDef fDef = RigidBodyComponent::genFixtureDef(RigidBodyComponent::PLAYER);
	b2CircleShape playerShape;
	playerShape.m_p.Set(0, drawOffset / PHYSICS_UNIT_SCALE);
	playerShape.m_radius = 0.7f;
	fDef.shape = &playerShape;
	rigidBody = gameObject->addComponent<RigidBodyComponent>();
	rigidBody->init(scene->getCollisionManager(), bDef, fDef);
	//create sensor
	fDef.isSensor = true;
	fDef.filter.maskBits = RigidBodyComponent::ENEMY;
	playerShape.m_radius = attackDistance / PHYSICS_UNIT_SCALE;
	rigidBody->addFixture(fDef);
	//target camera to player
	camera = scene->getCamera();
	camera->Target(transform);
	//create attack swipe object
	swipeEffect = (new GameObject(scene, "swipe", false))->addComponent<SwipeComponent>();
	swipeEffect->init();
	swipeEffect->disableComponent();
	//get shader
	additiveShader = Game::getInstance().getTextureManager()->getShader("additiveBlend");
	if (additiveShader != nullptr)
	{
		shaderTintLocation = GetShaderLocation(*additiveShader, "amount");
	}
}

void PlayerComponent::update()
{
	float dT = Game::getDeltaTime();

	direction = { 0 };

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
		if (flipped != direction.x < 0)
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

	cooldownTimer -= Game::getDeltaTime();
	if (invincible)
	{
		invincibilityTimer += Game::getDeltaTime();
		
		
		
		if (invincibilityTimer <= PI/(2 * invincibilityTintSpeed))
		{
			float invincibilityTintAmount = cosf(invincibilityTimer * invincibilityTintSpeed);
			SetShaderValue(*additiveShader, shaderTintLocation, &invincibilityTintAmount, SHADER_UNIFORM_FLOAT);
			std::cout << invincibilityTintAmount;
		}
		else
		{
			if (hitFlashing)
			{
				hitFlashing = false;
				armSprite->clearShader();
				gameObject->getSprite()->clearShader();
			}
			unsigned char invincibilityAlpha = 0xCF + copysignf(1.0f, sinf(invincibilityTimer * invincibilityAlphaSpeed)) * 0x30;
			armSprite->setTint({ 0xFF,0xFF,0xFF,invincibilityAlpha });
			gameObject->getSprite()->setTint({ 0xFF,0xFF,0xFF,invincibilityAlpha });
		}

		if (invincibilityTimer >= invincibilityTime)
		{
			invincible = false;
			invincibilityTimer = 0;
			gameObject->getSprite()->setTint({ 0xFF,0xFF,0xFF,0xFF });
			armSprite->setTint({ 0xFF,0xFF,0xFF,0xFF });
		}
	}
	
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
				armSprite->setSettings(attackMiddle + 1, attackEnd, attackMiddle + 1);
				armSprite->setCallback(spriteCallback, this);
				spritePause = attackEnd;
				armSprite->play();
				cooldownTimer = cooldown;
				attack();
			}
			break;
		case ST_ATTACK_END:
			armState = ST_WALKING;
			armSprite->setSettings(walkStart, walkEnd, walkStart);
			break;
		case ST_STEALTH_ENTER:
			armState = ST_STEALTH;
			armSprite->setSettings(stealthWalkStart, stealthWalkEnd, stealthWalkStart);
			armSprite->play();
			acceleration = maxAcceleration * stealthSpeedMultiplier;
			speed = maxSpeed * stealthAccelerationMultiplier;
			break;
		case ST_STEALTH:
			inStealth = true;
			armSprite->setCurrentFrame(((AnimatedSprite*)gameObject->getSprite())->getCurrentFrame() + stealthWalkStart - walkStart);
			break;
		case ST_STEALTH_LEAVE:
			speed = maxSpeed;
			acceleration = maxAcceleration;
			armSprite->setCallback(nullptr, nullptr);
			armSprite->setSettings(walkStart, walkEnd, walkStart);
			armState = ST_WALKING;
			break;
		}

		if (armState == ST_WALKING && cooldownTimer <= 0 && IsMouseButtonDown(MOUSE_BUTTON_LEFT))
		{
			pending = true;
			armState = ST_ATTACK_START;
			armSprite->setSettings(attackStart, attackMiddle, attackStart);
			armSprite->play();
			armSprite->setCallback(spriteCallback, this);
			spritePause = attackMiddle;
		}
		else if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))
		{
			if (armState == ST_WALKING)
			{
				pending = true;
				armState = ST_STEALTH_ENTER;
				armSprite->setSettings(stealthSwitchStart, stealthSwitchEnd, stealthSwitchStart);
				armSprite->play();
				armSprite->setCallback(spriteCallback, this);
				spritePause = stealthSwitchEnd;
			}
			else if (armState == ST_STEALTH)
			{
				pending = true;
				armState = ST_STEALTH_LEAVE;
				armSprite->setSettings(unstealthSwitchStart, unstealthSwitchEnd, unstealthSwitchStart);
				armSprite->play();
				armSprite->setCallback(spriteCallback, this);
				spritePause = unstealthSwitchEnd;

			}
			
		}
	}
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
	swipeEffect->startEffect(direction * (attackDistance/5) + startPos, direction);

	if (attackableBees.size() > 0)
	{
		AgentComponent* beeComponent;
		Vector2 playerPos = transform->getPosition();
		Vector2 deltaCamera = (playerPos - mousePos);

		for (int i = 0; i < attackableBees.size(); i++)
		{
			Vector2 beePos = attackableBees[i]->getTransform()->getPosition();
			Vector2 deltaBee = (playerPos - beePos);
			
			if (Vector2::dot(deltaCamera.normalised(), deltaBee.normalised()) > minDot)
			{
				beeComponent = attackableBees[i]->getGameObject()->getComponentOfType<AgentComponent>();
				if (beeComponent != nullptr)
				{
					beeComponent->hit(damage, knockback, transform->getPosition());
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
	rigidBody->addVelocity((position - transform->getPosition()).normalised() * Vector2 { -knockback, knockback });
	invincibilityTimer = 0;
	invincible = true;
	hitFlashing = true;
}
