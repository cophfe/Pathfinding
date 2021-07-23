#pragma once
#include "Behaviour.h"

typedef BehaviourResult(*questionFunction)(AgentComponent*);

//This behaviour works as a generic question node
//custom question nodes could be made seperately from this one
class BooleanBehaviour : 
	Behaviour
{
public:
	BooleanBehaviour(questionFunction);
	virtual BehaviourResult execute(AgentComponent* agent);
protected:
	questionFunction function;
};

