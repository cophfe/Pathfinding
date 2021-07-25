#pragma once

#include "NecessaryHeaders.h"
#include "SelectorBehaviour.h"
#include "SequenceBehaviour.h"
#include "Game.h"

//plays an animation and pauses at the end
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
		//stop looping
		auto self = (AnimationBehaviour*)instancePointer;
		self->sprite->pauseAt(self->endFrame);
		//end behaviour successfuly
		self->status = BehaviourResult::SUCCESS;
		//delete callback
		self->sprite->setCallback(nullptr, nullptr);
	}
protected:
	BehaviourResult status;
	AnimatedSprite* sprite; int startFrame; int endFrame;
};

//returns success or fail based on rand()
class RandomBehaviour : public Behaviour
{
public:
	RandomBehaviour(float chanceToSucceed) : chance(chanceToSucceed)
	{
	}

	virtual BehaviourResult execute(AgentComponent* agent)
	{
		return rand() < chance * RAND_MAX ? BehaviourResult::SUCCESS : BehaviourResult::FAILURE;
	};

protected:
	float chance;
};

//halts behaviour tree until end of countdown
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

//always fails
class FailBehaviour : public Behaviour
{
public:
	virtual BehaviourResult execute(AgentComponent* agent)
	{
		return BehaviourResult::FAILURE;
	};
};

//always succeeds
class SucceedBehaviour : public Behaviour
{
public:
	virtual BehaviourResult execute(AgentComponent* agent)
	{
		return BehaviourResult::SUCCESS;
	};
};

//returns success if a position is within a distance of the agent
class WithinDistanceBehaviour : public Behaviour
{
public:
	WithinDistanceBehaviour(float distance, const Vector2& position) : pos(position), dist(distance)
	{
	}

	virtual BehaviourResult execute(AgentComponent* agent)
	{
		int x = pos.x - agent->getTransform()->getPosition().x;
		int y = pos.y - agent->getTransform()->getPosition().y;
		return (BehaviourResult)((pos - agent->getTransform()->getPosition()).magnitudeSquared() < (dist * dist));
	};

protected:
	float dist;
	const Vector2& pos;
};

//inverts SUCCESS and FAILURE of it's child behaviour, keeps RUNNING as RUNNING
class NotDecoratorBehaviour : public Behaviour
{
public:
	NotDecoratorBehaviour(Behaviour* behaviour) : behaviour(behaviour)
	{

	}

	virtual BehaviourResult execute(AgentComponent* agent)
	{
		switch (behaviour->execute(agent))
		{
		case BehaviourResult::FAILURE:
			return BehaviourResult::SUCCESS;
			break;
		case BehaviourResult::SUCCESS:
			return BehaviourResult::FAILURE;
			break;
		case BehaviourResult::RUNNING:
			return BehaviourResult::RUNNING;
			break;
		}
		return BehaviourResult::FAILURE;
	}

	virtual ~NotDecoratorBehaviour()
	{
		delete behaviour;
	}
protected:
	Behaviour* behaviour;
};