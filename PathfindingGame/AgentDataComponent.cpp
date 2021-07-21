#include "AgentDataComponent.h"

void AgentDataComponent::init(GameObject* target, Pathfinder* pathfinder)
{
	this->target = target;
	this->pathfinder = pathfinder;
	targetHasBeenFound = false;
	targetMovedThisFrame = true;
}


 