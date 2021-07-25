#pragma once

#include "CompositeBehaviour.h"

//returns SUCCESS if one child returns SUCCESS and doesn't evaluate the rest
class SelectorBehaviour :
	public CompositeBehaviour
{
public:
	virtual BehaviourResult execute(AgentComponent* agent);
	virtual ~SelectorBehaviour() {};
};

