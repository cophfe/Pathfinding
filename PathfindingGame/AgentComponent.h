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
	static constexpr float CHASE_VEL_MULTIPLIER = 3.0f;
	static constexpr float CHASE_ACCEL_MULTIPLIER = 15.0f;
	static constexpr float MAX_ACCELERATION = 4.0f;
	static constexpr float MAX_VELOCITY = 2.0f;
	static constexpr float MIN_NODE_DIST = 55.0f;
	static constexpr float MIN_FINAL_DIST = 200.0f;
	//hover settings
	static constexpr float HOVER_MAG = 15.0f;
	static constexpr float HOVER_SPEED = 10.0f;
	////idle settings
	//static constexpr float idleChance = 0.9f;
	//static constexpr float idleTime = 2.0f;
	//static constexpr float idleVarience = 0.5f;
	//collider size settings
	static constexpr float COLLIDER_RAD = 0.5f;
	static constexpr float TRIGGER_RAD = 3.0f;
	//attack settings
	static constexpr float ATTACK_DIST = 200.0f;
	static constexpr int MAX_HEALTH = 3;
	static constexpr float KNOCKBACK = 10;
	static constexpr int DAMAGE = 1;
	static constexpr float HIT_TINT_SPEED = 2.5f * PI;

	//amimation start and end frames:
	//face:
	static constexpr int FACE_ALERT_START = 1;
	static constexpr int FACE_ALERT_END = 14;
	static constexpr int FACE_STATIC_ANGRY = 14;
	static constexpr int FACE_STATIC_CALM = 0;
	static constexpr int FACE_ATTACK_START = 24;
	static constexpr int FACE_ATTACK_HIT = 36;
	static constexpr int FACE_ATTACK_END = 48;
	static constexpr int FACE_TURN_START = 15;
	static constexpr int FACE_TURN_END = 23;
	//body:
	static constexpr int FLYING_START = 0;
	static constexpr int FLYING_END = 5;
	static constexpr int ATTACK_START = 15;
	static constexpr int ATTACK_HIT = 27;
	static constexpr int ATTACK_END = 39;
	static constexpr int TURN_START = 6;
	static constexpr int TURN_END = 14;
	static constexpr int DEATH_START = 40;
	static constexpr int DEATH_END = 45;

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

	inline PathfindingNode* getTargetNode() { return blackboard->getTargetPathfinderNode(); }

	//for behaviour node
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
	//plays at end of death animation
	static void deadCallback(void* ptr)
	{
		AgentComponent* c = (AgentComponent*)ptr;
		c->gameObject->setIsDrawn(false);
		c->gameObject->getComponentOfType<RigidBodyComponent>()->disableComponent();
		c->blackboard->removeAgent();
	}
	
	//	Health
	int health = MAX_HEALTH;
	bool dead = false;

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
	float timer = 0;

	//	Behaviour tree
	SequenceBehaviour* behaviourTree;
	bool targettingPlayer = false;
	bool collidedWithPlayer = false;

	//	Rendering
	float spriteHover;
	AnimatedSprite* sprite;
	GameObject* child;
	Shader* additiveShader = nullptr;
	float hitTimer = 0;
	bool hitFlashing = false;
};

