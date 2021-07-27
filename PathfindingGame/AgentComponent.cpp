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

//	Question functions

static BehaviourResult checkIfAlerted(AgentComponent* agent)
{
	return (BehaviourResult)(agent->getDataComponent()->targetIsFound() && !agent->checkTargettingPlayer());
}

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
	return (BehaviourResult)((agent->getTransform()->getPosition() - *agent->getDataComponent()->getTargetPosition()).magnitudeSquared() < AgentComponent::minDistanceToFinalNode * AgentComponent::minDistanceToFinalNode);
}

static BehaviourResult checkWithinDist(AgentComponent* agent)
{
	if (agent->checkTargettingPlayer())
	{
		return checkDistToPlayer(agent);
	}
	else
	{
		return (BehaviourResult)((agent->getTransform()->getPosition() - agent->getEndPosition()).magnitudeSquared() < AgentComponent::minDistanceToFinalNode * AgentComponent::minDistanceToFinalNode);
	}
}

AgentComponent::~AgentComponent()
{
	delete behaviourTree;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void AgentComponent::init(AgentDataComponent* blackboard, CollisionManager* manager)
{
	((AnimatedSprite*)gameObject->getSprite())->setSettings(0, 5, 0);
	gameObject->getSprite()->setDrawOffset(20);
	//actor child:
	this->child = new GameObject(gameObject, "beeFace", true, { -9,-41 });
	((AnimatedSprite*)this->child->getSprite())->pause();
	this->child->getSprite()->setDrawOffset(20);
	//add rigidbody
	b2BodyDef actorbDef = RigidBodyComponent::genBodyDef(b2_dynamicBody, true);
	b2FixtureDef actorfDef = RigidBodyComponent::genFixtureDef(RigidBodyComponent::ENEMY);
	b2CircleShape actorShape = b2CircleShape();
	actorShape.m_radius = AgentComponent::colliderRadius;
	actorfDef.shape = &actorShape;
	auto actorRb = gameObject->addComponent<RigidBodyComponent>();
	actorRb->init(manager, actorbDef, actorfDef);
	//add trigger
	b2FixtureDef actorTriggerfDef = RigidBodyComponent::genFixtureDef(RigidBodyComponent::ENEMY, RigidBodyComponent::PLAYER, nullptr, true);
	b2CircleShape actorTriggerShape = b2CircleShape();
	actorTriggerShape.m_radius = AgentComponent::triggerRadius;
	actorTriggerfDef.shape = &actorTriggerShape;
	actorRb->addFixture(actorTriggerfDef);

	this->blackboard = blackboard;
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
				new AnimationBehaviour((AnimatedSprite*)child->getSprite(), faceAlertStartFrame, faceAlertEndFrame),
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
				new AnimationBehaviour((AnimatedSprite*)gameObject->getSprite(), beeAttackStart, beeAttackHitPoint),
				new AnimationBehaviour((AnimatedSprite*)child->getSprite(), faceAttackStart, faceAttackHitPoint)
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
				new AnimationBehaviour((AnimatedSprite*)gameObject->getSprite(), beeAttackHitPoint+1, beeAttackEnd),
				new AnimationBehaviour((AnimatedSprite*)child->getSprite(), faceAttackHitPoint+1, faceAttackEnd)
			)
		)
	);
	
}

void AgentComponent::start()
{
	rigidBody = gameObject->getComponentOfType<RigidBodyComponent>();
}

void AgentComponent::fixedUpdate()
{

	//path following code
	//if Behaviour Tree says to move to next node:
	if (movingToNode)
	{
		//check if at destination
		if ((transform->getPosition() - destination).magnitudeSquared() < minDistanceToNode * minDistanceToNode)
		{
			movingToNode = false;
		}

		//move toward destination
		movementDirection = (destination - transform->getPosition()).normalised();
		movementDirection.y *= -1;
		Vector2 velocity = (movementDirection *  (maxVelocity + chaseVelocityMultiplier * targettingPlayer) - rigidBody->getVelocity());
		if (velocity.magnitudeSquared() > ((maxAcceleration  + chaseAccelerationMultiplier *targettingPlayer )* (maxAcceleration + chaseAccelerationMultiplier * targettingPlayer)) * PHYSICS_TIME_STEP * PHYSICS_TIME_STEP)
			velocity = velocity.normalised() * (maxAcceleration + chaseAccelerationMultiplier * targettingPlayer) * PHYSICS_TIME_STEP;
		rigidBody->addVelocity(velocity);
	}
}

void AgentComponent::update()
{
	static float timer = 0;
	timer += Game::getDeltaTime();
	gameObject->getSprite()->setDrawOffset(spriteHover + sin(timer * floatSpeed) * floatMagnitude);
	child->getSprite()->setDrawOffset(spriteHover + sin(timer * floatSpeed) * floatMagnitude);

	behaviourTree->execute(this);
}

void AgentComponent::hit(int damage, float knockback, const Vector2& position)
{
	health -= damage;
	rigidBody->addVelocity((position - transform->getPosition()).normalised() * Vector2 { -knockback, knockback });
}

void AgentComponent::onTriggerEnter(RigidBodyComponent* collisionBody, b2Fixture* collisionFixture)
{
	if (!collidedWithPlayer && !collisionFixture->IsSensor())
	{
		if (collisionBody != nullptr && collisionFixture->GetFilterData().categoryBits == RigidBodyComponent::PLAYER)
		{
			collidedWithPlayer = true;
		}
	}
}

#ifdef DRAW_DEBUG
void AgentComponent::debugDraw()
{
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

bool AgentComponent::checkShouldGeneratePath()
{
	if (targettingPlayer)
	{
		//technically this is an action but whatever it's easier this way
		if (checkTargetMoved())
		{
			return true;
		}
		else if (pathIndex > 0)
		{
			pathIndex--;
			destination = path[pathIndex];
			return false;
		}
		else
		{
			destination = transform->getPosition();
			return true;
		}
	}
	else
	{
		if (pathIndex > 0)
		{
			pathIndex--;
			destination = path[pathIndex];
			return false;
		}
		else
		{
			return true;
		}
	}
	return true;
}
#endif // DRAW_DEBUG