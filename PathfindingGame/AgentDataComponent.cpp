#include "AgentDataComponent.h"

void AgentDataComponent::init(GameObject* target, Pathfinder* pathfinder)
{
	player = target->getComponentOfType<PlayerComponent>();
	this->target = target;
	this->pathfinder = pathfinder;
	targetHasBeenFound = false;
	targetMovedThisFrame = true;
}


 