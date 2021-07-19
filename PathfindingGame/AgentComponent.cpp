#include "AgentComponent.h"

void AgentComponent::init(AgentDataComponent* blackboard)
{
	this->blackboard = blackboard;
	pathfinder = blackboard->getPathfinder();
	destination = transform->getPosition();
	currentNode = pathfinder->getNodeFromPoint(&transform->getPosition());
}

void AgentComponent::start()
{
	rigidBody = gameObject->getComponentOfType<RigidBodyComponent>();
}

void AgentComponent::fixedUpdate()
{
	//path following code
	
	if (!shouldReconstructPath)
	{
		//if player moves to a different node the path should be reconstructed
		shouldReconstructPath = checkTargetMoved();
		shouldReconstructPath = true;
	}

	if (true)//(transform->getPosition() - destination).magnitudeSquared() < minDistanceToNode * minDistanceToNode)
	{
		if (pathIndex == 0)
		{
			std::cout << "reached end!\n";
		}
		if (shouldReconstructPath)
		{
			if (pathfinder->AStarPath(pathfinder->getNodeFromPoint(&transform->getPosition()), getTargetNode(), &path) > 0)
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
	Vector2 velocity = (movementDirection * maxVelocity - rigidBody->getVelocity());
	if (velocity.magnitudeSquared() > maxAcceleration * maxAcceleration * PHYSICS_TIME_STEP * PHYSICS_TIME_STEP)
		velocity = velocity.normalised() * maxAcceleration * PHYSICS_TIME_STEP;
	rigidBody->addVelocity(velocity);
	
	
	
}

#ifdef DRAW_DEBUG
void AgentComponent::debugDraw()
{
	if (!path.empty())
	{
		for (size_t i = 0; i < path.size() - 1; i++)
		{
			DrawLineEx(path[i], path[i + 1], 8, BLACK);
			DrawCircle(path[i].x, path[i].y, 10, BLACK);
		}
	}
}
#endif // DRAW_DEBUG