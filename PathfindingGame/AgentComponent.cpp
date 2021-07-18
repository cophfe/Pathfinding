#include "AgentComponent.h"

void AgentComponent::init(AgentDataComponent* blackboard)
{
	this->blackboard = blackboard;
	pathfinder = blackboard->getPathfinder();
}

void AgentComponent::start()
{
	rigidbody = gameObject->getComponentOfType<RigidBodyComponent>();
}

void AgentComponent::fixedUpdate()
{
	//path following code
	
	//check if the target has moved from one node to another
	if (!targetMovedNode)
		targetMovedNode = checkTargetMoved();

	//check if it is within reach of the destination
	if ((transform->getPosition() - destination).magnitudeSquared() < minDistanceToNode * minDistanceToNode)
	{
		//check if destination (that was just reached) is the target node
		if (!path.empty() && path[path.size() - 1] == destination)
		{
			std::cout << "CHANGE STATE!\n";
		}
		else
		{
			if (targetMovedNode)
			{
				//if the target has moved nodes, redo astar.
				pathIndex = 0;
				if (pathfinder->AStarPath(pathfinder->getNodeFromPoint(&transform->getPosition()), getTargetNode(), &path) > 0)
				{
					//if astar returns a result, set the destination to the first node
					destination = path[pathIndex];
				}
				targetMovedNode = false;
			}
			else
			{
				//if the target hasn't moved nodes, just use the next node up the path
				pathIndex++;
				destination = path[pathIndex];
			}
		}
	}

	//move toward destination
	movementDirection = (destination - transform->getPosition()).normalised();
	rigidbody->setVelocity(movementDirection * acceleration * PHYSICS_TIME_STEP);
	
	
}
