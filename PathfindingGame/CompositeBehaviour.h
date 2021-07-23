#pragma once
#include "Behaviour.h"
#include <vector>

class CompositeBehaviour :
    public Behaviour
{
public:
	CompositeBehaviour* add(Behaviour* childBehaviour);

	CompositeBehaviour* addMultiple(int count, ... );
	void remove(Behaviour* childBehaviour);
	virtual ~CompositeBehaviour();
protected:
	std::vector<Behaviour*> children;
	unsigned int pendingChildIndex = 0; // all this is used for is for starting from the pending child, so it should by default be 0
};

