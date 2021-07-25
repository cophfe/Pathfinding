#pragma once
#include "CompositeBehaviour.h"

//returns SUCCESS if all children return SUCCESS
class SequenceBehaviour :
    public CompositeBehaviour
{
public:
	virtual BehaviourResult execute(AgentComponent* agent);
	virtual ~SequenceBehaviour() {};
};

