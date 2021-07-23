#pragma once
#include "NecessaryHeaders.h"
#include "AgentComponent.h"

enum class BehaviourResult
{
	FAILURE,
	SUCCESS,
	RUNNING,
};
class Behaviour
{
public:
	
	virtual ~Behaviour() {};
	virtual BehaviourResult execute(AgentComponent* agent) { return BehaviourResult::FAILURE; };
};

