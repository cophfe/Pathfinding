#include "ParallelBehaviour.h"

BehaviourResult ParallelBehaviour::execute(AgentComponent* agent)
{
	//runs all children until they all return SUCCESS or FAIL
	//if a single child returns RUNNING then the entire behaviour returns running, 
	//	but only after it executes all other children that have not yet returned SUCCESS or FAIL

	char amountOfPending = 0;
	for (int i = 0; i < children.size(); i++)
	{
		if (!isDone[i])
		{
			switch (children[i]->execute(agent))
			{
			case BehaviourResult::SUCCESS:
				isDone[i] = true;
				break;
			case BehaviourResult::FAILURE:
				isDone[i] = true;
				returnFail = true;
				break;
			case BehaviourResult::RUNNING:
				amountOfPending++;
				break;
			}
		}
	}

	//if no pending behaviours then return fail if any returned fail, or success if they all returned success
	if (amountOfPending == 0)
	{
		//clear isDone
		std::fill(isDone.begin(), isDone.end(), 0);

		if (returnFail)
		{
			returnFail = false;
			return BehaviourResult::FAILURE;
		}
		else
		{
			returnFail = false;
			return BehaviourResult::SUCCESS;
		}
	}
	else
	{
		return BehaviourResult::RUNNING;
	}
}

CompositeBehaviour* ParallelBehaviour::add(Behaviour* childBehaviour)
{
	children.push_back(childBehaviour);
	isDone.push_back(false);

	return this;
}

CompositeBehaviour* ParallelBehaviour::addMultiple(int count, ...)
{
	children.reserve(count);
	isDone.reserve(count);

	va_list arguments;
	va_start(arguments, count);
	for (int i = 0; i < count; i++)
	{
		children.push_back(va_arg(arguments, Behaviour*));
		isDone.push_back(false);
	}
	va_end(arguments);

	return this;
}

void ParallelBehaviour::remove(Behaviour* childBehaviour)
{
	children.erase(std::remove(children.begin(), children.end(), childBehaviour), children.end());
}