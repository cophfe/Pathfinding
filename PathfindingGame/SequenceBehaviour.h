#pragma once
#include "CompositeBehaviour.h"
class SequenceBehaviour :
    public CompositeBehaviour
{
	virtual BehaviourResult execute(AgentComponent* agent);
};

