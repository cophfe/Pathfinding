#include "AgentComponent.h"
#include "Game.h"

//	Behaviour tree
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#include "Behaviour.h"
#include "BooleanBehaviour.h"
#include "SelectorBehaviour.h"
#include "SequenceBehaviour.h"
#include "AgentBehaviours.h"

//	Question functions

static BehaviourResult checkIfAlerted(AgentComponent* agent)
{
	return (BehaviourResult)(agent->getDataComponent()->targetIsFound() && agent->checkTargettingPlayer());
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
	return (BehaviourResult)agent->checkShouldGeneratePath();
}

static BehaviourResult reachedEnd(AgentComponent* agent)
{
	//if targetting player, check what node was last reached, and if within chase distance
	//if targetting random position, check what node was last reached
}

static BehaviourResult checkWithinAttackDistance(AgentComponent* agent)
{
	//pretty simple
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void AgentComponent::init(AgentDataComponent* blackboard, GameObject* child)
{
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
		7,
		//Check for player (OR)
		(new SelectorBehaviour())->addMultiple(
			2,
			//React to finding player (AND)
			(new SequenceBehaviour())->addMultiple(
				3,
				//Player found but not targeted?
				new QuestionBehaviour(checkIfAlerted),
				//Play alert animation!
				new AnimationBehaviour((AnimatedSprite*)child->getSprite(), beeAlertStartFrame, beeAlertEndFrame),
				//Target player!
				new QuestionBehaviour(targetPlayer)
			),
			//Search for player (AND)
			(new SequenceBehaviour())->addMultiple(
				3,
				//Player not found?
				new BooleanBehaviour(targettingPlayer),
				//Seen player?
				new BooleanBehaviour(collidedWithPlayer),
				//Alert blackboard!
				new QuestionBehaviour(alertPlayers)
			)
		),
		//Generate path
		(new SelectorBehaviour())->addMultiple(
			2,
			//Need to generate a path?
			new QuestionBehaviour(checkGenPath),
			//Generate path! (if targettingPlayer is false, generate a path to a random non-wall position. if cannot generate path, return false)
			new GeneratePathBehaviour()
		),
		//Direction of path is opposite current sprite direction?
		(new SelectorBehaviour())->addMultiple(
			2,
			//Don't need to turn?
			new QuestionBehaviour(shouldTurn),
			//Play turn animation!
			new FlipAgentBehaviour((AnimatedSprite*)gameObject->getSprite(), child)
		),
		//Move to next node!
		new AgentMoveNodeBehaviour(),
		//Reached end?
		new QuestionBehaviour(reachedEnd),
		//Do end action (OR)
		(new SelectorBehaviour())->addMultiple(
			2,
			//Attack player (AND)
			(new SequenceBehaviour())->addMultiple(
				4,
				//Targetting player?
				new BooleanBehaviour(targettingPlayer),
				//Start attack animation!
				new Behaviour(), //~~~~~~~~~~this one will require it's own behaviour like flip animation, since the child node has to be updated too. should really make a parallel behaviour (allow two things to be pending at once)
				//Hit player (OR)
				(new SelectorBehaviour())->addMultiple(
					2,
					//Not still near player?
					new BooleanBehaviour(checkWithinAttackDistance),
					//Damage player!
					new AttackPlayerBehaviour()
				),
				//End attack animation!
				new Behaviour() //~~~~~~~~ also needs work
			),
			(new SelectorBehaviour())->addMultiple(
				2,
				//Idle (AND)
				(new SequenceBehaviour)->addMultiple(
					2,
					// Should idle? 
					RandomBehaviour(idleChance),
					//Idle!
					CountdownBehaviour(idleTime + ((float)rand()/RAND_MAX - 0.5f) * idleVarience)
				)
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

	if ((transform->getPosition() - destination).magnitudeSquared() < minDistanceToNode * minDistanceToNode)
	{
		if (pathIndex == 0)
		{
			std::cout << "reached end!\n";
			movementDirection = { 0 };
		}
		if (checkTargetMoved())
		{
			if (pathfinder->AStarPath(pathfinder->getNodeFromPoint(&transform->getPosition()), playerNode, &path) > 0)
			{
				//if astar returns a result, set the destination to the first node
				pathIndex = path.size() - 1;
				destination = path[pathIndex];
				
			}
			//otherwise the destination stays the same and we wait till we should reconstruct the path again
			shouldReconstructPath = false;
		}
		else
		{
			if (!path.empty() && pathIndex > 0)
			{
				pathIndex--;
				destination = path[pathIndex];
				
			}
		}
	}

	//move toward destination
	movementDirection = (destination - transform->getPosition()).normalised();
	movementDirection.y *= -1;
	sprite->setFlipped(movementDirection.x > 0);
	Vector2 velocity = (movementDirection * maxVelocity - rigidBody->getVelocity());
	if (velocity.magnitudeSquared() > maxAcceleration * maxAcceleration * PHYSICS_TIME_STEP * PHYSICS_TIME_STEP)
		velocity = velocity.normalised() * maxAcceleration * PHYSICS_TIME_STEP;
	rigidBody->addVelocity(velocity);
	
	
	
}

void AgentComponent::update()
{
	static float timer = 0;
	timer += Game::getDeltaTime();
	gameObject->getSprite()->setDrawOffset(spriteHover + sin(timer * floatSpeed) * floatMagnitude);
}

void AgentComponent::onCollisionEnter(RigidBodyComponent* collisionBody, b2Manifold* manifold)
{
	if (collisionBody->getBody()->GetFixtureList()->GetFilterData().categoryBits == RigidBodyComponent::PLAYER)
		collidedWithPlayer = true;
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
	//if targetting player
	//  if player moved since last generated path
	//		generate path
	//	else 
	//		move up the index (technically this is an action but whatever it's easier this way)
	//else
	//	if path does not exist already 
	//		if end has not been reached
	//			generatePath
	//	else
	//		move up the index
	return true;
}
#endif // DRAW_DEBUG