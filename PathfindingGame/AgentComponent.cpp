#include "AgentComponent.h"
#include "Game.h"

//	Behaviour tree
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#include "Behaviour.h"
#include "BooleanBehaviour.h"
#include "SelectorBehaviour.h"
#include "SequenceBehaviour.h"
#include "ParallelBehaviour.h"
#include "AgentBehaviours.h"
#include "Behaviours.h"

//~~~~~~~~~~~~~~~~~~~~~~~~
//	Question functions
//~~~~~~~~~~~~~~~~~~~~~~~~

static BehaviourResult checkIfAlerted(AgentComponent* agent)
{
	return (BehaviourResult)(agent->getDataComponent()->targetIsFound() && !agent->checkTargettingPlayer());
}

static BehaviourResult shouldTurn(AgentComponent* agent)
{
	return (BehaviourResult)agent->checkShouldTurn();
}

static BehaviourResult checkGenPath(AgentComponent* agent)
{
	return (BehaviourResult)!agent->checkShouldGeneratePath();
}

static BehaviourResult checkDistToPlayer(AgentComponent* agent)
{
	return (BehaviourResult)((agent->getTransform()->getPosition() - *agent->getDataComponent()->getTargetPosition()).magnitudeSquared() < AgentComponent::MIN_FINAL_DIST * AgentComponent::MIN_FINAL_DIST);
}

static BehaviourResult checkWithinDist(AgentComponent* agent)
{
	if (agent->checkTargettingPlayer())
	{
		return checkDistToPlayer(agent);
	}
	else
	{
		return (BehaviourResult)((agent->getTransform()->getPosition() - agent->getEndPosition()).magnitudeSquared() < AgentComponent::MIN_FINAL_DIST * AgentComponent::MIN_FINAL_DIST);
	}
}
//~~~~~~~~~~~~~~~~~~~~~~~~
//	Action functions
//~~~~~~~~~~~~~~~~~~~~~~~~

static BehaviourResult targetPlayer(AgentComponent* agent)
{
	agent->setTargettingPlayer(true);
	return BehaviourResult::SUCCESS;
}

static BehaviourResult alertPlayers(AgentComponent* agent)
{
	agent->getDataComponent()->setFoundTarget(true);
	return BehaviourResult::SUCCESS;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

AgentComponent::~AgentComponent()
{
	delete behaviourTree;
}


void AgentComponent::init(AgentDataComponent* blackboard, CollisionManager* manager)
{
	//initialize information
	((AnimatedSprite*)gameObject->getSprite())->setSettings(0, 5, 0);
	gameObject->getSprite()->setDrawOffset(20);
	//create face:
	this->child = new GameObject(gameObject, "beeFace", true, { -9,-41 });
	((AnimatedSprite*)this->child->getSprite())->pause();
	this->child->getSprite()->setDrawOffset(20);
	//add rigidbody
	b2BodyDef actorbDef = RigidBodyComponent::genBodyDef(b2_dynamicBody, true);
	b2FixtureDef actorfDef = RigidBodyComponent::genFixtureDef(RigidBodyComponent::ENEMY);
	b2CircleShape actorShape = b2CircleShape();
	actorShape.m_radius = AgentComponent::COLLIDER_RAD;
	actorfDef.shape = &actorShape;
	auto actorRb = gameObject->addComponent<RigidBodyComponent>();
	actorRb->init(manager, actorbDef, actorfDef);
	//add trigger (for detecting player when masked)
	b2FixtureDef actorTriggerfDef = RigidBodyComponent::genFixtureDef(RigidBodyComponent::ENEMY, RigidBodyComponent::PLAYER, nullptr, true);
	b2CircleShape actorTriggerShape = b2CircleShape();
	actorTriggerShape.m_radius = AgentComponent::TRIGGER_RAD;
	actorTriggerfDef.shape = &actorTriggerShape;
	actorRb->addFixture(actorTriggerfDef);

	//set member variables
	this->blackboard = blackboard;
	blackboard->addAgent();
	pathfinder = blackboard->getPathfinder();
	destination = transform->getPosition();
	sprite = (AnimatedSprite*)gameObject->getSprite();
	spriteHover = sprite->getDrawOffset();
	this->child = child;

	/*
		Generate behaviour tree
		this is very messy
		it was easier to do this per agent, but it could be modified to be inside the data component, saving a lot of memory
		but come on, gotta use these gigabytes for something

		? == Question
		! == Action
		(AND) == Sequence
		(OR) == Selector
	*/

	behaviourTree = new SequenceBehaviour();
	behaviourTree->addMultiple(
		6,
		//Check for player (OR)
		(new SelectorBehaviour())->addMultiple(
			3,
			//React to finding player (AND)
			(new SequenceBehaviour())->addMultiple(
				3,
				//Player found but not targeted?
				new QuestionBehaviour(checkIfAlerted),
				//Play alert animation!
				new AnimationBehaviour((AnimatedSprite*)child->getSprite(), FACE_ALERT_START, FACE_ALERT_END),
				//Target player!
				new QuestionBehaviour(targetPlayer)
			),
			//Search for player (AND)
			(new SequenceBehaviour())->addMultiple(
				3,
				//Player not found?
				new NotDecoratorBehaviour(new BooleanBehaviour(targettingPlayer)),
				//Seen player?
				new BooleanBehaviour(collidedWithPlayer),
				//Alert blackboard!
				new QuestionBehaviour(alertPlayers)
			),
			//cheat here because I structured the behaviour tree poorly
			new SucceedBehaviour()
		),
		//Generate path
		(new SelectorBehaviour())->addMultiple(
			2,
			//Need to generate a path?
			new QuestionBehaviour(checkGenPath),
			//Generate path! (if targettingPlayer is false, generate a path to a random non-wall position. if cannot generate path, return false)
			new GeneratePathBehaviour()
		),
		
		//turn (OR)
		(new SelectorBehaviour())->addMultiple(
			2,
			//Direction of path is opposite current sprite direction?
			new QuestionBehaviour(shouldTurn),
			//Play turn animation!
			new FlipAgentBehaviour((AnimatedSprite*)gameObject->getSprite(), child)
		),
		//Move to next node!
		new AgentMoveNodeBehaviour(),
		//Reached end?
		new QuestionBehaviour(checkWithinDist),
			//Attack player (AND)
		(new SequenceBehaviour())->addMultiple(
			4,
			//Targetting player?
			new BooleanBehaviour(targettingPlayer),
			//start attack animation!
			(new ParallelBehaviour())->addMultiple(
				2,
				new AnimationBehaviour((AnimatedSprite*)gameObject->getSprite(), ATTACK_START, ATTACK_HIT),
				new AnimationBehaviour((AnimatedSprite*)child->getSprite(), FACE_ATTACK_START, FACE_ATTACK_HIT)
			),
			//Hit player (OR)
			(new SelectorBehaviour())->addMultiple(
				2,
				//Not still near player?
				new NotDecoratorBehaviour(new QuestionBehaviour(checkDistToPlayer)),
				//Damage player!
				new AttackPlayerBehaviour(blackboard->getTarget()->getComponentOfType<PlayerComponent>())
			),
			//End attack animation!
			(new ParallelBehaviour())->addMultiple(
				2,
				new AnimationBehaviour((AnimatedSprite*)gameObject->getSprite(), ATTACK_HIT+1, ATTACK_END),
				new AnimationBehaviour((AnimatedSprite*)child->getSprite(), FACE_ATTACK_HIT+1, FACE_ATTACK_END)
			)
		)
	);
	
}

void AgentComponent::start()
{
	rigidBody = gameObject->getComponentOfType<RigidBodyComponent>();
	additiveShader = Game::getInstance().getTextureManager()->getShader("additiveTint");
}

void AgentComponent::fixedUpdate()
{
	//path following code
	//if Behaviour Tree says to move to next node:
	if (movingToNode)
	{
		//check if at destination
		if ((transform->getPosition() - destination).magnitudeSquared() < MIN_NODE_DIST * MIN_NODE_DIST)
		{
			movingToNode = false;
		}

		//move toward destination
		movementDirection = (destination - transform->getPosition()).normalised();
		movementDirection.y *= -1;
		Vector2 velocity = (movementDirection *  (MAX_VELOCITY + CHASE_VEL_MULTIPLIER * targettingPlayer) - rigidBody->getVelocity());
		if (velocity.magnitudeSquared() > ((MAX_ACCELERATION  + CHASE_ACCEL_MULTIPLIER *targettingPlayer )* (MAX_ACCELERATION + CHASE_ACCEL_MULTIPLIER * targettingPlayer)) * PHYSICS_TIME_STEP * PHYSICS_TIME_STEP)
			velocity = velocity.normalised() * (MAX_ACCELERATION + CHASE_ACCEL_MULTIPLIER * targettingPlayer) * PHYSICS_TIME_STEP;
		rigidBody->addVelocity(velocity);
	}
}

void AgentComponent::update()
{
	//if dying play death animation and clear velocity
	if (health <= 0)
	{
		if (!dead)
		{
			Vector2 v = { 0 };
			rigidBody->setVelocity(v);
			((AnimatedSprite*)gameObject->getSprite())->setCallback(deadCallback, this);
			((AnimatedSprite*)gameObject->getSprite())->setSettings(DEATH_START, DEATH_END, DEATH_START);
			((AnimatedSprite*)gameObject->getSprite())->play();
			child->setIsDrawn(false);
			dead = true;
		}
	}
	else
	{
		timer += Game::getDeltaTime();
		//sprite hovers up and down
		gameObject->getSprite()->setDrawOffset(spriteHover + sin(timer * HOVER_SPEED) * HOVER_MAG);
		child->getSprite()->setDrawOffset(spriteHover + sin(timer * HOVER_SPEED) * HOVER_MAG);
		
		//execute behaviour tree here
		behaviourTree->execute(this);

		//for hit flash effect
		if (hitFlashing)
		{
			hitTimer += Game::getDeltaTime();

			if (hitTimer <= PI / (2 * HIT_TINT_SPEED))
			{
				float invincibilityTintAmount = cosf(hitTimer * HIT_TINT_SPEED);
				unsigned char invincibilityTint = cosf(hitTimer * HIT_TINT_SPEED) * 0xFF;
				//SetShaderValue(*shader, shaderTintLocation, &invincibilityTintAmount, SHADER_UNIFORM_FLOAT);
				sprite->setTint({ invincibilityTint,invincibilityTint,invincibilityTint,0xFF });
				if (child)
				{
					child->getSprite()->setTint({ invincibilityTint,invincibilityTint,invincibilityTint,0xFF });

				}

			}
			else
			{
				//at end of hit flash effect reset variables
				sprite->setTint({ 0xFF, 0xFF, 0xFF, 0xFF });
				child->getSprite()->setTint({ 0xFF, 0xFF, 0xFF, 0xFF });
				sprite->clearShader();
				if (child)
				{
					child->getSprite()->clearShader();
				}
				hitFlashing = false;
			}

		}
	}
}

void AgentComponent::hit(int damage, float knockback, const Vector2& position)
{
	//when hit by player start hit flash effect, take damage, and take knockback
	health -= damage;
	if (health <= 0)
	{
		sprite->clearShader();
		sprite->setTint(WHITE);
		return;
	}
	hitFlashing = true;
	hitTimer = 0;
	sprite->setShader(additiveShader);
	if (child)
	{
		child->getSprite()->setShader(additiveShader);
	}
	Vector2 knockbackVector = (position - transform->getPosition()).normalised() * knockback;
	knockbackVector.x *= -1;
	rigidBody->addVelocity(knockbackVector);
}

void AgentComponent::onTriggerEnter(RigidBodyComponent* collisionBody, b2Fixture* collisionFixture)
{
	if (!collidedWithPlayer && !collisionFixture->IsSensor())
	{
		if (collisionBody != nullptr && collisionFixture->GetFilterData().categoryBits == RigidBodyComponent::PLAYER)
		{
			//bee will be alerted if player collides with it
			collidedWithPlayer = true;
		}
	}
}

#ifdef DRAW_DEBUG
void AgentComponent::debugDraw()
{
	//draw path and player position
	static Color b = { 0,0,0,200 };
	if (!path.empty())
	{
		for (size_t i = 0; i < path.size() - 1; i++)
		{
			DrawLineEx(path[i], path[i + 1], 8, b);
			DrawCircle(path[i].x, path[i].y, 10, b);
		}
	}
	DrawCircle(blackboard->getTargetPosition()->x, blackboard->getTargetPosition()->y, 10, RED);
}
#endif

bool AgentComponent::checkShouldGeneratePath()
{
	//part of an action/question node
	//moves up path or says to generate path, or does nothing and returns false

	if (targettingPlayer)
	{
		//if target has moved ALWAYS return true
		if (checkTargetMoved())
		{
			return true;
		}
		else if (pathIndex > 0)
		{
			//if not reached end, move to next node
			pathIndex--;
			destination = path[pathIndex];
			return false;
		}
		else
		{
			//if reached end of path, stay still
			destination = transform->getPosition();
			return true;
		}
	}
	else
	{
		//if path exists already and havent reached the end, follow it
		if (pathIndex > 0)
		{
			pathIndex--;
			destination = path[pathIndex];
			return false;
		}
		else
		{
			//else generate a new path
			return true;
		}
	}
	return true;
}