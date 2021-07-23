#pragma once
#include "NecessaryHeaders.h"
#include "Component.h"
#include "AgentDataComponent.h"
#include "RigidBodyComponent.h"
#include "Pathfinder.h"
constexpr float minDistanceToNode = 10.0f;
constexpr float floatMagnitude = 15.0f;
constexpr float floatSpeed = 10.0f;

class SequenceBehaviour;

class AgentComponent : public Component
{
public:
	void init(AgentDataComponent* blackboard );
	void start();
	void fixedUpdate();
	void update();
#ifdef DRAW_DEBUG
	void debugDraw();
#endif // DRAW_DEBUG

private:

	//this is here for the future, just in case target should be able to be changed
	inline PathfindingNode* getTargetNode() { return blackboard->getTargetPathfinderNode(); }
	inline bool checkTargetMoved()
	{
		PathfindingNode* node = blackboard->getTargetPathfinderNode();
		if (node == playerNode)
		{
			playerNode = node;
			return false;
		}
		else
		{
			playerNode = node;
			return true;
		}
	}
	
	Pathfinder* pathfinder;
	AnimatedSprite* sprite;

	AgentDataComponent* blackboard;
	RigidBodyComponent* rigidBody;

	std::vector<Vector2> path;
	Vector2 destination;
	
	float maxAcceleration = 4.0f;
	float maxVelocity = 2.0f;

	//behaviour node
	SequenceBehaviour* behaviourTree;

	bool shouldReconstructPath = true;
	Vector2 movementDirection;
	int pathIndex = 0;

	float spriteHover;

	PathfindingNode* playerNode;
};

