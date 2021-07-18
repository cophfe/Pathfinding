#pragma once
#include "NecessaryHeaders.h"
#include "Component.h"
#include "AgentDataComponent.h"
#include "RigidBodyComponent.h"
#include "Pathfinder.h"
constexpr float minDistanceToNode = 10.0f;

class AgentComponent : public Component
{
public:
	void init(AgentDataComponent* blackboard );
	void start();
	void fixedUpdate();

private:

	//this is here for the future, just in case target should be able to be changed
	inline PathfindingNode* getTargetNode() { return blackboard->getTargetPathfinderNode(); }
	inline bool checkTargetMoved() { return blackboard->checkTargetMovedThisFrame(); }
	
	Pathfinder* pathfinder;

	AgentDataComponent* blackboard;
	RigidBodyComponent* rigidbody;

	std::vector<Vector2> path;
	Vector2 destination;
	
	float acceleration = 2.0f;

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// 	   internal only
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	bool targetMovedNode;
	Vector2 movementDirection;
	int pathIndex = 0;
};

