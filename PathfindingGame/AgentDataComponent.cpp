#include "AgentDataComponent.h"

void AgentDataComponent::init(GameObject* target, Pathfinder* pathfinder)
{
	this->target = target;
	targetHasBeenFound = false;
	targetMovedThisFrame = true;
	previousTargetNode = pathfinder->getNodeFromPoint(getTargetPosition());
}

void AgentDataComponent::fixedUpdate()
{
	PathfindingNode* node = pathfinder->getNodeFromPoint(getTargetPosition());
	targetMovedThisFrame = !(node->indexX == previousTargetNode->indexX && node->indexY == previousTargetNode->indexY);
	previousTargetNode = node;
}


 