#pragma once
#include "NecessaryHeaders.h"
#include "GameObject.h"
#include "Pathfinder.h"

class AgentDataComponent : public Component
{
public:
	void init(GameObject* target, Pathfinder* pathfinder);

	inline const Vector2* getTargetPosition() { return &target->getTransform()->getGlobalPosition(); }
	inline PathfindingNode* getTargetPathfinderNode() { return pathfinder->getNodeFromPoint(&target->getTransform()->getGlobalPosition()); }
	inline bool targetIsFound() { return targetHasBeenFound; }
	inline void setFoundTarget(bool isFound = true) { targetHasBeenFound = isFound; }
	inline Pathfinder* getPathfinder() { return pathfinder; }
private:
	GameObject* target;

	Pathfinder* pathfinder;
	bool targetHasBeenFound;
	bool targetMovedThisFrame;
};

