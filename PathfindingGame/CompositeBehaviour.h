#pragma once
#include "Behaviour.h"
#include <vector>

//has multiple child behaviours.
class CompositeBehaviour :
    public Behaviour
{
public:
	virtual CompositeBehaviour* add(Behaviour* childBehaviour);
	virtual CompositeBehaviour* addMultiple(int count, ... );
	virtual void remove(Behaviour* childBehaviour);
	virtual ~CompositeBehaviour();
protected:
	std::vector<Behaviour*> children;
	unsigned int pendingChildIndex = 0; // all this is used for is for starting from the pending child, so it should by default be 0
};

