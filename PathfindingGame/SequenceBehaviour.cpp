#include "SequenceBehaviour.h"

BehaviourResult SequenceBehaviour::execute(AgentComponent* agent)
{
	//starts from pending child
	for (int i = pendingChildIndex; i < children.size(); i++)
	{
		switch (children[i]->execute(agent))
		{
		case BehaviourResult::SUCCESS:
			{
				//all need to return success for behaviour to return success
				pendingChildIndex = 0;
				continue;
			}
			break;
		case BehaviourResult::RUNNING:
			{
				//same as selector
				pendingChildIndex = i;
				return BehaviourResult::RUNNING;
			}
			break;
		case BehaviourResult::FAILURE:
			{
				//if one returns failure, return failure
				pendingChildIndex = 0;
				return BehaviourResult::FAILURE;
			}
			break;
		}
	}
	return BehaviourResult::SUCCESS;
}
