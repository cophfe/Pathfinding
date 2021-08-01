#pragma once

#include "NecessaryHeaders.h"
#include "SelectorBehaviour.h"
#include "SequenceBehaviour.h"
#include "AgentComponent.h"
#include "Game.h"
#include "PlayerComponent.h"

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	THIS HEADER HOLDS A BUNCH OF BEHAVIOURS SPECIFIC TO THE AGENT'S BEHAVIOUR TREE
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

//flips the bee sprite
class FlipAgentBehaviour : public Behaviour
{
public:
	//Constructor
	FlipAgentBehaviour(AnimatedSprite* sprite, GameObject* child) : sprite(sprite), child(child), status(BehaviourResult::FAILURE), staticFaceFrame(AgentComponent::FACE_STATIC_CALM)
	{	}

	virtual BehaviourResult execute(AgentComponent* agent)
	{
		switch (status)
		{

		//this only happens once, and it sets up all the values necessary for the sprite to change
		case BehaviourResult::FAILURE: 
			{
				//set body animation settings
				sprite->setSettings(AgentComponent::TURN_START , AgentComponent::TURN_END, AgentComponent::TURN_START);
				sprite->playAt(AgentComponent::TURN_START);
				//set callback (runs at end of sprite loop)
				sprite->setCallback(endOfAnimationCallback, this);
				//set face animation settings
				staticFaceFrame = agent->checkTargettingPlayer() ? AgentComponent::FACE_STATIC_ANGRY : AgentComponent::FACE_STATIC_CALM;
				((AnimatedSprite*)child->getSprite())->setSettings(AgentComponent::FACE_TURN_START, AgentComponent::FACE_TURN_END, AgentComponent::FACE_TURN_START);
				((AnimatedSprite*)child->getSprite())->playAt(AgentComponent::FACE_TURN_START);
				//set status to running so that the behaviour tree waits until this behaviour is done to continue
				status = BehaviourResult::RUNNING;
			}
			break;
		case BehaviourResult::SUCCESS:
			{
				//at end of animation return success and reset status
				status = BehaviourResult::FAILURE;
				return BehaviourResult::SUCCESS;
			}
			break;
		}
		return status;
	};

	//called at the end of animation loop
	static void endOfAnimationCallback(void* instancePointer)
	{
		auto self = (FlipAgentBehaviour*)instancePointer;
		
		//flip sprite
		self->sprite->flip();
		self->sprite->setSettings(AgentComponent::FLYING_START, AgentComponent::FLYING_END, AgentComponent::FLYING_START);
		//flip child sprite and pause it at the static face
		AnimatedSprite* childSprite = (AnimatedSprite *)self->child->getSprite();
		childSprite->pauseAt(self->staticFaceFrame);
		childSprite->flip();
		self->child->getTransform()->flipPositionX();
		//end behaviour
		self->status = BehaviourResult::SUCCESS;
		//remove this callback from the sprite
		self->sprite->setCallback(nullptr, nullptr);
	}

protected:
	AnimatedSprite* sprite; GameObject* child;
	BehaviourResult status;
	//the static frame for the face (could be angry or calm)
	int staticFaceFrame;
};

//Generates path toward target
class GeneratePathBehaviour : public Behaviour
{
public:
	GeneratePathBehaviour()
	{
	}

	virtual BehaviourResult execute(AgentComponent* agent)
	{
		//if agent is targetting player generate a path toward the player
		if (agent->targettingPlayer)
		{
			if (agent->pathfinder->AStarPath(agent->pathfinder->getNodeFromPoint(&agent->transform->getPositionReference()), agent->playerNode, &agent->path) > 0)
			{
				//if astar returns a result, set the destination to the first node
				agent->pathIndex = (int)agent->path.size() - 1;
				agent->destination = agent->path[agent->pathIndex];
				agent->movementDirection = (agent->destination - agent->transform->getPosition()).normalised();
				agent->movementDirection.y *= -1;
			}
			else
			{
				return BehaviourResult::FAILURE;
			}
		}
		//else return a path toward a random position
		else
		{
			auto pathfinder = agent->pathfinder;

			PathfindingNode* node = pathfinder->getRandomUnblockedNode();

			if (pathfinder->AStarPath(agent->pathfinder->getNodeFromPoint(&agent->transform->getPositionReference()), node, &agent->path) > 0)
			{
				//if astar returns a result, set the destination to the first node
				agent->pathIndex = (int)agent->path.size() - 1;
				agent->destination = agent->path[agent->pathIndex];
				agent->movementDirection = (agent->destination - agent->transform->getPosition()).normalised();
				agent->movementDirection.y *= -1;
			}
			else
			{
				return BehaviourResult::FAILURE;
			}
		}

		return BehaviourResult::SUCCESS;
	};
};

//moves the agent toward the next path node
class AgentMoveNodeBehaviour : public Behaviour
{
public:
	AgentMoveNodeBehaviour() : status(BehaviourResult::FAILURE)
	{
	}

	virtual BehaviourResult execute(AgentComponent* agent)
	{
		switch (status)
		{
		case BehaviourResult::FAILURE: //this only happens once
			{
				//movement happens through fixedUpdate(), this just notifies the agent it to start and waits until it reaches the end
				agent->movingToNode = true;
				//make behaviour tree halt at this node until the task is done
				status = BehaviourResult::RUNNING;
				hasCollidedWithPlayer = agent->collidedWithPlayer;
				timer = 0;
			}
			break;
		case BehaviourResult::RUNNING:
			{
				timer += Game::getDeltaTime();
				//if player has moved into range of bee, cancel moving to node (lets attack start earlier)
				if (hasCollidedWithPlayer != agent->collidedWithPlayer)
				{
					status = BehaviourResult::FAILURE;
					return BehaviourResult::FAILURE;
				}
				//if it has taken too long to get to the next node, cancel moving to node
				if (timer >= TIME_OUT)
				{
					status = BehaviourResult::FAILURE;
					return BehaviourResult::FAILURE;
				}
				//if movingToNode value was changed inside of the agent or the agent got close enough to the node, return success
				if (agent->movingToNode == false || ((agent->getTransform()->getPosition() - *agent->getDataComponent()->getTargetPosition()).magnitudeSquared() < AgentComponent::MIN_FINAL_DIST * AgentComponent::MIN_FINAL_DIST))
				{
					status = BehaviourResult::FAILURE;
					return BehaviourResult::SUCCESS;
				}
			}
			break;
		}
		return status;
	};

protected:
	static constexpr float TIME_OUT = 5;
	float timer;
	BehaviourResult status;
	bool hasCollidedWithPlayer;
};

//used to hit the player
class AttackPlayerBehaviour : public Behaviour
{
public:
	AttackPlayerBehaviour(PlayerComponent* player) : player(player)
	{
	}

	virtual BehaviourResult execute(AgentComponent* agent)
	{
		player->hit(AgentComponent::DAMAGE, AgentComponent::KNOCKBACK, agent->getTransform()->getPosition());
		return BehaviourResult::SUCCESS;
	};

protected:
	PlayerComponent* player;
};
