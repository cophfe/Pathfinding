#pragma once

#include "CompositeBehaviour.h"

class SelectorBehaviour :
	public CompositeBehaviour
{
public:
	virtual BehaviourResult execute(AgentComponent* agent);
};

