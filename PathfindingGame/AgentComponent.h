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
	static const float nodeMinDistance;
	AgentDataComponent* blackboard;
	RigidBodyComponent* rigidbody;

	std::vector<Vector2> path;
	Vector2 destination;
	
	float acceleration;
	Vector2 movementDirection;
	bool targetMovedSinceLastChecked;
	int pathIndex = 0;
};

