#pragma once
#include "NecessaryHeaders.h"
#include "Component.h"
#include "AgentDataComponent.h"
#include "RigidBodyComponent.h"
#include "Pathfinder.h"

class SequenceBehaviour;
class Behaviour;
class AgentMoveNodeBehaviour;
class GeneratePathBehaviour;

class AgentComponent : public Component
{
public:
	//movement settings
	static constexpr float chaseVelocityMultiplier = 3.0f;
	static constexpr float chaseAccelerationMultiplier = 15.0f;
	static constexpr float chaseAccelerationMultiplierSq = chaseAccelerationMultiplier * chaseAccelerationMultiplier;
	static constexpr float maxAcceleration = 4.0f;
	static constexpr float maxVelocity = 2.0f;
	static constexpr float minDistanceToNode = 55.0f;
	static constexpr float minDistanceToFinalNode = 200.0f;
	//hover settings
	static constexpr float floatMagnitude = 15.0f;
	static constexpr float floatSpeed = 10.0f;
	//idle settings
	static constexpr float idleChance = 0.9f;
	static constexpr float idleTime = 2.0f;
	static constexpr float idleVarience = 0.5f;
	static constexpr float attackDistance = 200.0f;
	//collider size settings
	static constexpr float colliderRadius = 0.5f;
	static constexpr float triggerRadius = 3.0f;
	//attack settings
	static constexpr int healthMax = 2;
	static constexpr float knockback = 10;
	static constexpr int damage = 1;
	
	//amimation start and end frames:
	//face:
	static constexpr int faceAlertStartFrame = 1;
	static constexpr int faceAlertEndFrame = 14;
	static constexpr int faceStaticAngry = 14;
	static constexpr int faceStaticCalm = 0;
	static constexpr int faceAttackStart = 24;
	static constexpr int faceAttackHitPoint = 36;
	static constexpr int faceAttackEnd = 48;
	static constexpr int faceTurnStart = 15;
	static constexpr int faceTurnEnd = 23;
	//body:
	static constexpr int beeFlyingStart = 0;
	static constexpr int beeFlyingEnd = 5;
	static constexpr int beeAttackStart = 15;
	static constexpr int beeAttackHitPoint = 27;
	static constexpr int beeAttackEnd = 39;
	static constexpr int beeTurnStart = 6;
	static constexpr int beeTurnEnd = 14;

	AgentComponent() = default;
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//		Copy and Move constructors and assigners 
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//copy
	AgentComponent(const AgentComponent&) = delete;
	AgentComponent& operator=(const AgentComponent&) = delete;
	//move
	AgentComponent(AgentComponent&&) = delete;
	AgentComponent& operator=(AgentComponent&&) = delete;
	virtual ~AgentComponent();
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//		Overloaded functions
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	void init(AgentDataComponent* blackboard, CollisionManager* manager);
	void start();
	void fixedUpdate();
	void update();
	void hit(int damage, float knockback, const Vector2& position);
	void onTriggerEnter(RigidBodyComponent* collisionBody, b2Fixture* collisionFixture);
#ifdef DRAW_DEBUG
	void debugDraw();
#endif // DRAW_DEBUG
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

	inline AgentDataComponent* getDataComponent() { return blackboard; }
	inline const bool& checkTargettingPlayer() { return targettingPlayer; }
	inline void setTargettingPlayer(bool value)
	{
		targettingPlayer = value;
	}
	bool checkShouldGeneratePath();
	const bool& searchForPlayer()
	{
		//collided with player is set on collision with player (obviously)
		return collidedWithPlayer;
	}

	inline bool checkShouldTurn()
	{
		return (destination - transform->getPosition()).x < 0 != gameObject->getSprite()->getFlipped();
	}

	Vector2 getEndPosition() { return path.empty() ? Vector2{99999999.0f, 99999999.0f } : path[0]; }
private:
	//behaviours have access to agent if they want, as some of them work as though they are functions of the agentcomponent
	friend class AgentMoveNodeBehaviour;
	friend class GeneratePathBehaviour;

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
	
	//	Health
	int health = healthMax;

	//	Pathfinding
	Pathfinder* pathfinder;
	AgentDataComponent* blackboard;
	RigidBodyComponent* rigidBody;
	std::vector<Vector2> path;
	PathfindingNode* playerNode = nullptr;
	Vector2 destination;
	bool shouldReconstructPath = true;
	Vector2 movementDirection;
	int pathIndex = 0;
	bool movingToNode = false;

	//	Behaviour tree
	SequenceBehaviour* behaviourTree;
	bool targettingPlayer = false;
	bool collidedWithPlayer = false;


	//	Rendering
	float spriteHover;
	AnimatedSprite* sprite;
	GameObject* child;

	
};

