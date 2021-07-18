#include "AgentDataComponent.h"

void AgentDataComponent::init(GameObject* target, Pathfinder* pathfinder)
{
	this->target = target;
	this->pathfinder = pathfinder;
	targetHasBeenFound = false;
	targetMovedThisFrame = true;
	previousTargetNode = pathfinder->getNodeFromPoint(getTargetPosition());
}

void AgentDataComponent::fixedUpdate()
{
	PathfindingNode* node = pathfinder->getNodeFromPoint(getTargetPosition());
	targetMovedThisFrame = !(previousTargetNode == node);
	previousTargetNode = node;
}


 