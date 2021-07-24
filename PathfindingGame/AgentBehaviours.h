#pragma once

#include "NecessaryHeaders.h"
#include "SelectorBehaviour.h"
#include "SequenceBehaviour.h"
#include "AgentComponent.h"

#include "Game.h"

class AnimationBehaviour : public Behaviour
{
public:
	AnimationBehaviour(AnimatedSprite* sprite, int startFrame, int endFrame) : status(BehaviourResult::FAILURE), startFrame(startFrame), endFrame(endFrame), sprite(sprite)
	{
		
	}

	virtual BehaviourResult execute(AgentComponent* agent) 
	{
		switch (status)
		{
		case BehaviourResult::FAILURE: //this only happens once
			{
				sprite->setSettings(startFrame, endFrame, startFrame);
				sprite->play();
				sprite->setCallback(endOfAnimationCallback, this);
				status = BehaviourResult::RUNNING;
			}
			break;
		case BehaviourResult::SUCCESS:
			{
				status = BehaviourResult::FAILURE;
				return BehaviourResult::SUCCESS;
			}
			break;
		}
		return status;
	};

	static void endOfAnimationCallback(void* instancePointer)
	{
		auto self = (AnimationBehaviour*)instancePointer;
		self->sprite->pauseAt(self->endFrame);
		self->status = BehaviourResult::SUCCESS;
	}
protected:
	BehaviourResult status;
	AnimatedSprite* sprite; int startFrame; int endFrame;
};

class RandomBehaviour : public Behaviour
{
public:
	RandomBehaviour(float chanceToSucceed) : chance(chanceToSucceed)
	{	}

	virtual BehaviourResult execute(AgentComponent* agent)
	{
		return rand() < chance * RAND_MAX ? BehaviourResult::SUCCESS : BehaviourResult::FAILURE;
	};

protected:
	float chance;
};

class CountdownBehaviour : public Behaviour
{
public:
	CountdownBehaviour(float time) : time(time), timer(time)
	{
		
	}

	virtual BehaviourResult execute(AgentComponent* agent)
	{
		timer -= Game::getDeltaTime();
		if (timer <= 0)
		{
			timer = time;
			return BehaviourResult::SUCCESS;
		}
		return BehaviourResult::RUNNING;
	};

protected:
	float timer;
	const float time;
};

class FlipAgentBehaviour : public Behaviour
{
public:
	FlipAgentBehaviour(AnimatedSprite* sprite, GameObject* child) : sprite(sprite), child(child), status(BehaviourResult::FAILURE), staticFaceFrame(faceStaticCalmFrame)
	{
		
	}

	virtual BehaviourResult execute(AgentComponent* agent)
	{
		switch (status)
		{
		case BehaviourResult::FAILURE: //this only happens once
			{
				staticFaceFrame = agent->checkTargettingPlayer() ? faceStaticAngryFrame : faceStaticCalmFrame;
				sprite->setSettings(startFrame, endFrame, startFrame);
				sprite->playAt(startFrame);
				sprite->setCallback(endOfAnimationCallback, this);
				((AnimatedSprite*)child->getSprite())->setSettings(faceStartFrame, faceEndFrame, faceStartFrame);
				((AnimatedSprite*)child->getSprite())->playAt(faceStartFrame);
				status = BehaviourResult::RUNNING;
			}
			break;
		case BehaviourResult::SUCCESS:
			{
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
		self->sprite->setSettings(flyingStartFrame, flyingEndFrame, flyingStartFrame);
		//flip child sprite
		AnimatedSprite* childSprite = (AnimatedSprite *)self->child->getSprite();
		childSprite->pauseAt(self->staticFaceFrame);
		childSprite->flip();
		self->child->getTransform()->flipPositionX();
		//end action
		self->status = BehaviourResult::SUCCESS;
	}
protected:
	static constexpr int faceStaticAngryFrame = 14;
	static constexpr int faceStaticCalmFrame = 14;
	int staticFaceFrame;
	static constexpr int faceStartFrame = 15;
	static constexpr int faceEndFrame = 24;
	static constexpr int startFrame = 15;
	static constexpr int endFrame = 24;
	static constexpr int flyingStartFrame = 0;
	static constexpr int flyingEndFrame = 5;

	BehaviourResult status;
	AnimatedSprite* sprite; GameObject* child;
};

class GeneratePathBehaviour : public Behaviour
{
public:
	GeneratePathBehaviour() : status(BehaviourResult::FAILURE)
	{
	}

	virtual BehaviourResult execute(AgentComponent* agent)
	{
		switch (status)
		{
		case BehaviourResult::FAILURE: //this only happens once
			{

			}
			break;
		case BehaviourResult::RUNNING:
			{

			}
			break;
		case BehaviourResult::SUCCESS:
			{
				status = BehaviourResult::FAILURE;
				return BehaviourResult::SUCCESS;
			}
			break;
		}
		return status;
	};

protected:
	BehaviourResult status;
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

			}
			break;
		case BehaviourResult::RUNNING:
			{

			}
			break;
		case BehaviourResult::SUCCESS:
			{
				status = BehaviourResult::FAILURE;
				return BehaviourResult::SUCCESS;
			}
			break;
		}
		return status;
	};

protected:
	BehaviourResult status;
};

class AttackPlayerBehaviour : public Behaviour
{
public:
	AttackPlayerBehaviour() : status(BehaviourResult::FAILURE)
	{
	}

	virtual BehaviourResult execute(AgentComponent* agent)
	{
		switch (status)
		{
		case BehaviourResult::FAILURE: //this only happens once
			{

			}
			break;
		case BehaviourResult::RUNNING:
			{

			}
			break;
		case BehaviourResult::SUCCESS:
			{
				status = BehaviourResult::FAILURE;
				return BehaviourResult::SUCCESS;
			}
			break;
		}
		return status;
	};

protected:
	BehaviourResult status;
};