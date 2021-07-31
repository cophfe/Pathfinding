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
	FlipAgentBehaviour(AnimatedSprite* sprite, GameObject* child) : sprite(sprite), child(child), status(BehaviourResult::FAILURE), staticFaceFrame(AgentComponent::FACE_STATIC_CALM)
	{
		
	}

	virtual BehaviourResult execute(AgentComponent* agent)
	{
		switch (status)
		{
		case BehaviourResult::FAILURE: //this only happens once
			{
				//set body settings
				sprite->setSettings(AgentComponent::TURN_START , AgentComponent::TURN_END, AgentComponent::TURN_START);
				sprite->playAt(AgentComponent::TURN_START);
				//set callback (runs at end of sprite loop)
				sprite->setCallback(endOfAnimationCallback, this);
				//set face settings
				staticFaceFrame = agent->checkTargettingPlayer() ? AgentComponent::FACE_STATIC_ANGRY : AgentComponent::FACE_STATIC_CALM;
				((AnimatedSprite*)child->getSprite())->setSettings(AgentComponent::FACE_TURN_START, AgentComponent::FACE_TURN_END, AgentComponent::FACE_TURN_START);
				((AnimatedSprite*)child->getSprite())->playAt(AgentComponent::FACE_TURN_START);

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
		self->sprite->setSettings(AgentComponent::FLYING_START, AgentComponent::FLYING_END, AgentComponent::FLYING_START);
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
				hasCollidedWithPlayer = agent->collidedWithPlayer;
				timer = 0;
			}
			break;
		case BehaviourResult::RUNNING:
			{
				timer += Game::getDeltaTime();
				if (hasCollidedWithPlayer != agent->collidedWithPlayer)
				{
					status = BehaviourResult::FAILURE;
					return BehaviourResult::FAILURE;
				}
				if (timer >= TIME_OUT)
				{
					status = BehaviourResult::FAILURE;
					return BehaviourResult::FAILURE;
				}
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
	BehaviourResult status;
	float timer;
	bool hasCollidedWithPlayer;
	static constexpr float TIME_OUT = 5;
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
