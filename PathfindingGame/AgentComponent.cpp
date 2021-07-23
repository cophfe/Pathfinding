#include "AgentComponent.h"
#include "Game.h"

//behaviour tree
#include "BooleanBehaviour.h"
#include "SelectorBehaviour.h"
#include "SequenceBehaviour.h"

void AgentComponent::init(AgentDataComponent* blackboard)
{
	this->blackboard = blackboard;
	pathfinder = blackboard->getPathfinder();
	destination = transform->getPosition();
	sprite = (AnimatedSprite*)gameObject->getSprite();
	spriteHover = sprite->getDrawOffset();

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
				new BooleanBehaviour(),
				//Play alert animation!
				new Behaviour(),
				//Target player!
				new Behaviour()
			),
			//Search for player (AND)
			(new SequenceBehaviour())->addMultiple(
				3,
				//Player not found?
				new BooleanBehaviour(),
				//Seen player?
				new BooleanBehaviour(),
				//Alert blackboard!
				new Behaviour()
			)
		),
		//Generate path
		(new SelectorBehaviour())->addMultiple(
			2,
			//Need to generate a path?
			new BooleanBehaviour(),
			//Generate path!
			new Behaviour()
		),
		//Direction of path is opposite current sprite direction?
		(new SelectorBehaviour())->addMultiple(
			2,
			//Don't need to turn?
			new BooleanBehaviour(),
			//Play turn animation!
			new Behaviour
		),
		//Move to next node!
		new Behaviour(),
		//Reached end?
		new BooleanBehaviour(),
		//Do end action (OR)
		(new SelectorBehaviour())->addMultiple(
			2,
			//Attack player (AND)
			(new SequenceBehaviour())->addMultiple(
				4,
				//Targetting player?
				new BooleanBehaviour(),
				//Start attack animation!
				new Behaviour(),
				//Hit player (OR)
				(new SelectorBehaviour())->addMultiple(
					2,
					//Not still near player?
					new BooleanBehaviour(),
					//Damage player!
					new Behaviour()
				),
				//End attack animation!
				new Behaviour()
			),
			(new SelectorBehaviour())->addMultiple(
				2,
				//Idle (AND)
				(new SequenceBehaviour)->addMultiple(
					2,
					// Should idle? 
					BooleanBehaviour(),
					//Idle!
					Behaviour()
				),
				//Target random valid position!
				new Behaviour()
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
#endif // DRAW_DEBUG