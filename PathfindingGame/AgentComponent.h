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

#ifdef DRAW_DEBUG
	void debugDraw();
#endif // DRAW_DEBUG

private:

	//this is here for the future, just in case target should be able to be changed
	inline PathfindingNode* getTargetNode() { return blackboard->getTargetPathfinderNode(); }
	inline bool checkTargetMoved() { return blackboard->checkTargetMovedThisFrame(); }
	
	Pathfinder* pathfinder;

	AgentDataComponent* blackboard;
	RigidBodyComponent* rigidBody;

	std::vector<Vector2> path;
	Vector2 destination;
	
	float maxAcceleration = 140.0f;
	float maxVelocity = 8.0f / 10;

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// 	   internal only
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	bool shouldReconstructPath = true;
	Vector2 movementDirection;
	int pathIndex = 0;

	PathfindingNode* currentNode;
};

