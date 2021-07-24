#pragma once
#include "NecessaryHeaders.h"
#include "Component.h"
#include "AgentDataComponent.h"
#include "RigidBodyComponent.h"
#include "Pathfinder.h"
constexpr float minDistanceToNode = 10.0f;
constexpr float floatMagnitude = 15.0f;
constexpr float floatSpeed = 10.0f;
constexpr float idleChance = 0.3f;
constexpr float idleTime = 2.0f;
constexpr float idleVarience = 0.5f;

constexpr int beeAlertStartFrame = 1;
constexpr int beeAlertEndFrame = 14;

constexpr float attackDistance = 50.0f;

class SequenceBehaviour;
class Behaviour;

class AgentComponent : public Component
{
public:
	//overloaded functions
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	void init(AgentDataComponent* blackboard, GameObject* child);
	void start();
	void fixedUpdate();
	void update();
	void onCollisionEnter(RigidBodyComponent* collisionBody, b2Manifold* manifold);
#ifdef DRAW_DEBUG
	void debugDraw();
#endif // DRAW_DEBUG
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

	inline AgentDataComponent* getDataComponent() { return blackboard; }
	inline const bool& checkTargettingPlayer() { return targettingPlayer; }
	inline void setTargettingPlayer(bool value) { targettingPlayer = value; }
	bool checkShouldGeneratePath();
	const bool& searchForPlayer()
	{
		//collided with player is set on collision with player (obviously)
		return collidedWithPlayer;
	}

	inline bool checkShouldTurn()
	{
		return (movementDirection.x > 0 != gameObject->getSprite()->getFlipped());
	}
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
	GameObject* child;

	PathfindingNode* playerNode;

	bool targettingPlayer = false;
	bool collidedWithPlayer = false;
	//behaviours have access to agent
	friend Behaviour;
};

