#include "SequenceBehaviour.h"

BehaviourResult SequenceBehaviour::execute(AgentComponent* agent)
{
	for (int i = pendingChildIndex; i < children.size(); i++)
	{
		switch (children[i]->execute(agent))
		{
		case BehaviourResult::SUCCESS:
			{
				continue;
			}
			break;
		case BehaviourResult::RUNNING:
			{
				pendingChildIndex = i;
				return BehaviourResult::RUNNING;
			}
			break;
		case BehaviourResult::FAILURE:
			{
				pendingChildIndex = 0;
				return BehaviourResult::FAILURE;
			}
			break;
		}
	}
	return BehaviourResult::SUCCESS;
}
