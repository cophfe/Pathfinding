#include "AgentComponent.h"

void AgentComponent::init(AgentDataComponent* blackboard)
{
	this->blackboard = blackboard;
}

void AgentComponent::start()
{
	rigidbody = gameObject->getComponentOfType<RigidBodyComponent>();
}

void AgentComponent::fixedUpdate()
{
	Pathfinder* pathfinder = blackboard->getPathfinder();
	if (!targetMovedSinceLastChecked)
		targetMovedSinceLastChecked = blackboard->checkTargetMovedThisFrame();
	if ((transform->getPosition() - path[pathIndex]).magnitudeSquared() < minDistanceToNode * minDistanceToNode)
	{
		if (targetMovedSinceLastChecked)
		{
			pathIndex = 0;
			pathfinder->AStarPath(pathfinder->getNodeFromPoint(&transform->getPosition()), blackboard->getTargetPathfinderNode() , &path);
		}
		else
		{
			pathIndex++;
			if (pathIndex >= path.size() - 1)
				path.clear();
		}
		targetMovedSinceLastChecked = false;
	}
	if (!path.empty())
	{
		movementDirection = (path[pathIndex] - transform->getPosition()).normalised();
		rigidbody->setVelocity(movementDirection * acceleration * PHYSICS_TIME_STEP);
	}
	
}
