#pragma once

#include "NecessaryHeaders.h"
#include "SelectorBehaviour.h"
#include "SequenceBehaviour.h"
#include "AgentComponent.h"
#include "Game.h"
#include "PlayerComponent.h"

//flips the bee sprite
class FlipAgentBehaviour : public Behaviour
{
public:
	FlipAgentBehaviour(AnimatedSprite* sprite, GameObject* child) : sprite(sprite), child(child), status(BehaviourResult::FAILURE), staticFaceFrame(AgentComponent::faceStaticCalm)
	{
		
	}

	virtual BehaviourResult execute(AgentComponent* agent)
	{
		switch (status)
		{
		case BehaviourResult::FAILURE: //this only happens once
			{
				//set body settings
				sprite->setSettings(AgentComponent::beeTurnStart , AgentComponent::beeTurnEnd, AgentComponent::beeTurnStart);
				sprite->playAt(AgentComponent::beeTurnStart);
				//set callback (runs at end of sprite loop)
				sprite->setCallback(endOfAnimationCallback, this);
				//set face settings
				staticFaceFrame = agent->checkTargettingPlayer() ? AgentComponent::faceStaticAngry : AgentComponent::faceStaticCalm;
				((AnimatedSprite*)child->getSprite())->setSettings(AgentComponent::faceTurnStart, AgentComponent::faceTurnEnd, AgentComponent::faceTurnStart);
				((AnimatedSprite*)child->getSprite())->playAt(AgentComponent::faceTurnStart);

				status = BehaviourResult::RUNNING;
			}
			break;
		case BehaviourResult::SUCCESS:
			{
				//at end of animation return success
				status = BehaviourResult::FAILURE;
				return BehaviourResult::SUCCESS;
			}
			break;
		}
		return status;
	};

	static void endOfAnimationCallback(void* instancePointer)
	{
		auto self = (FlipAgentBehaviour*)instancePointer;
		
		//flip sprite
		self->sprite->flip();
		self->sprite->setSettings(AgentComponent::beeFlyingStart, AgentComponent::beeFlyingEnd, AgentComponent::beeFlyingStart);
		//flip child sprite
		AnimatedSprite* childSprite = (AnimatedSprite *)self->child->getSprite();
		childSprite->pauseAt(self->staticFaceFrame);
		childSprite->flip();
		self->child->getTransform()->flipPositionX();
		//end action
		self->status = BehaviourResult::SUCCESS;
		//delete callback
		self->sprite->setCallback(nullptr, nullptr);
	}
protected:
	int staticFaceFrame;

	BehaviourResult status;
	AnimatedSprite* sprite; GameObject* child;
};

class GeneratePathBehaviour : public Behaviour
{
public:
	GeneratePathBehaviour()
	{
	}

	virtual BehaviourResult execute(AgentComponent* agent)
	{
		if (agent->targettingPlayer)
		{
			if (agent->pathfinder->AStarPath(agent->pathfinder->getNodeFromPoint(&agent->transform->getPositionReference()), agent->playerNode, &agent->path) > 0)
			{
				//if astar returns a result, set the destination to the first node
				agent->pathIndex = agent->path.size() - 1;
				agent->destination = agent->path[agent->pathIndex];
				agent->movementDirection = (agent->destination - agent->transform->getPosition()).normalised();
				agent->movementDirection.y *= -1;
			}
			else
			{
				return BehaviourResult::FAILURE;
			}
		}
		else
		{
			auto pathfinder = agent->pathfinder;

			PathfindingNode* node = pathfinder->getRandomUnblockedNode();

			if (pathfinder->AStarPath(agent->pathfinder->getNodeFromPoint(&agent->transform->getPositionReference()), node, &agent->path) > 0)
			{
				//if astar returns a result, set the destination to the first node
				agent->pathIndex = agent->path.size() - 1;
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
				status = BehaviourResult::RUNNING;
			}
			break;
		case BehaviourResult::RUNNING:
			{
				if (agent->targettingPlayer != agent->collidedWithPlayer)
				{
					return BehaviourResult::FAILURE;
				}
				if (agent->movingToNode == false || ((agent->getTransform()->getPosition() - *agent->getDataComponent()->getTargetPosition()).magnitudeSquared() < AgentComponent::minDistanceToFinalNode * AgentComponent::minDistanceToFinalNode))
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
	BehaviourResult status;
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
		player->hit(agent->getTransform()->getPosition());
		return BehaviourResult::SUCCESS;
	};

protected:
	PlayerComponent* player;
};
