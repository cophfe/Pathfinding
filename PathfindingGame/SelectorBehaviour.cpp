#include "SelectorBehaviour.h"

BehaviourResult SelectorBehaviour::execute(AgentComponent* agent)
{
	//starts from pending child
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
				//if child is running, evaluate that child until it returns success
				pendingChildIndex = i;
				return BehaviourResult::RUNNING;
			}
			break;
		case BehaviourResult::FAILURE:
			{
				pendingChildIndex = 0;
				continue;
			}
			break;
		}
	}
	return BehaviourResult::FAILURE;
}
