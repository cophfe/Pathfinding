#include "SelectorBehaviour.h"

BehaviourResult SelectorBehaviour::execute(AgentComponent* agent)
{

	for (int i = pendingChildIndex; i < children.size(); i++)
	{
		switch (children[i]->execute(agent))
		{
		case BehaviourResult::SUCCESS:
			{
				pendingChildIndex = 0;
				return BehaviourResult::SUCCESS;
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
				continue;
			}
			break;
		}
	}

	pendingChildIndex = 0;
	return BehaviourResult::FAILURE;
}
