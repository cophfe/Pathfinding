#include "BooleanBehaviour.h"

BooleanBehaviour::BooleanBehaviour(const bool& boolean) : boolean(boolean)
{
}

BehaviourResult BooleanBehaviour::execute(AgentComponent* agent)
{
	return boolean ? BehaviourResult::SUCCESS : BehaviourResult::FAILURE;
}

QuestionBehaviour::QuestionBehaviour(questionFunction function)
{
	this->function = function;
}

BehaviourResult QuestionBehaviour::execute(AgentComponent* agent)
{
	return function(agent);
}
