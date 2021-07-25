#pragma once
#include "NecessaryHeaders.h"
#include "AgentComponent.h"

enum class BehaviourResult : char
{
	FAILURE,
	SUCCESS,
	RUNNING,
};

//Base behaviour
//always returns FAILURE
class Behaviour
{
public:
	virtual BehaviourResult execute(AgentComponent* agent) { return BehaviourResult::FAILURE; };
	virtual ~Behaviour() {};
};

