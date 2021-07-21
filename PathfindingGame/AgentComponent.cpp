#include "AgentComponent.h"
#include "Game.h"

void AgentComponent::init(AgentDataComponent* blackboard)
{
	this->blackboard = blackboard;
	pathfinder = blackboard->getPathfinder();
	destination = transform->getPosition();
	sprite = (AnimatedSprite*)gameObject->getSprite();
	spriteHover = sprite->getDrawOffset();
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