#pragma once
#include "Behaviour.h"


//this behaviour works as a simplified question node, accepting a reference to a bool and returning success or failure based on it's value
class BooleanBehaviour : 
	public Behaviour
{
public:
	BooleanBehaviour(const bool& boolean);
	virtual BehaviourResult execute(AgentComponent* agent);
	virtual ~BooleanBehaviour() {};

protected:
	const bool& boolean;
};

typedef BehaviourResult(*questionFunction)(AgentComponent*);

//This behaviour works as a generic question node, accepting a function.
//it also works as a generic action node
class QuestionBehaviour :
	public Behaviour
{
public:
	QuestionBehaviour(questionFunction);
	virtual BehaviourResult execute(AgentComponent* agent);
	virtual ~QuestionBehaviour() {};

protected:
	questionFunction function;
};

