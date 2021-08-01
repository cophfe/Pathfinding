#pragma once
#include "CompositeBehaviour.h"

//waits until all children have returned either SUCCESS or FAILURE to stop running, useful for running multiple behaviours that take multiple frames at the same time
class ParallelBehaviour :
    public CompositeBehaviour
{
public:
	virtual BehaviourResult execute(AgentComponent* agent);

	//as the isDone array is now added and has to be the same size as the child array, custom add, addMultiple and remove functions were added
	virtual CompositeBehaviour* add(Behaviour* childBehaviour);
	virtual CompositeBehaviour* addMultiple(int count, ...);
	virtual void remove(Behaviour* childBehaviour);
	virtual ~ParallelBehaviour() {};

protected:
	std::vector<bool> isDone;
	bool returnFail = false;
};

