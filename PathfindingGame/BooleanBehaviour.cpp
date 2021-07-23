#include "BooleanBehaviour.h"

BooleanBehaviour::BooleanBehaviour(questionFunction function)
{
	this->function = function;
}

BehaviourResult BooleanBehaviour::execute(AgentComponent* agent)
{
	return function(agent);
}
