#pragma once
#include "NecessaryHeaders.h"
#include "GameObject.h"
#include "Pathfinder.h"

//note while this is technically a blackboard, it isn't very complex, only holding a few variables for the agents to read
class AgentDataComponent : public Component
{
public:
	void init(GameObject* target, Pathfinder* pathfinder);

	inline Vector2* getTargetPosition() { return &target->getTransform()->getGlobalPosition(); }
	inline GameObject* getTarget() { return target; }
	inline PathfindingNode* getTargetPathfinderNode() { return pathfinder->getNodeFromPoint(&target->getTransform()->getGlobalPosition()); }
	inline const bool& targetIsFound() { return targetHasBeenFound; }
	inline void setFoundTarget(bool isFound = true) { targetHasBeenFound = isFound; }
	inline Pathfinder* getPathfinder() { return pathfinder; }
private:
	GameObject* target;

	Pathfinder* pathfinder;
	bool targetHasBeenFound;
	bool targetMovedThisFrame;
};

