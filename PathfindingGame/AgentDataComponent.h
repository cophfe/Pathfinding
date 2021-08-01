#pragma once
#include "NecessaryHeaders.h"
#include "GameObject.h"
#include "Pathfinder.h"
#include "PlayerComponent.h"

//note: I'm calling this a blackboard but it isn't very complex, only holding a few variables for the agents to read
class AgentDataComponent : public Component
{
public:
	void init(GameObject* target, Pathfinder* pathfinder);

	//getters and setters
	inline Vector2* getTargetPosition() { return &target->getTransform()->getGlobalPosition(); }
	inline GameObject* getTarget() { return target; }
	inline PathfindingNode* getTargetPathfinderNode() { return pathfinder->getNodeFromPoint(&target->getTransform()->getGlobalPosition()); }
	inline bool targetIsFound() { return targetHasBeenFound || !player->hasMaskOn(); }
	inline void setFoundTarget(bool isFound = true) { targetHasBeenFound = isFound; }
	inline Pathfinder* getPathfinder() { return pathfinder; }
	inline void addAgent() { agentNumber++; }
	inline void removeAgent() { agentNumber--; }
	inline bool noAgentsLeft() { return agentNumber <= 0; }
private:

	PlayerComponent* player;
	GameObject* target;
	int agentNumber = 0;
	Pathfinder* pathfinder;
	bool targetHasBeenFound;
	bool targetMovedThisFrame;
};

