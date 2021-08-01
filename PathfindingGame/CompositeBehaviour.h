#pragma once
#include "Behaviour.h"
#include <vector>

//has multiple child behaviours. has to be inherited from for custom execute functions
class CompositeBehaviour :
    public Behaviour
{
public:
	//add 1 behaviour
	virtual CompositeBehaviour* add(Behaviour* childBehaviour);
	//add multiple behaviours to vector
	virtual CompositeBehaviour* addMultiple(int count, ... );
	//remove child behaviour by value
	virtual void remove(Behaviour* childBehaviour);
	virtual ~CompositeBehaviour();
protected:
	std::vector<Behaviour*> children;
	// all this is used for is for starting from the pending child, so it should by default be 0, as by default it would start from 0 anyway
	unsigned int pendingChildIndex = 0; 
};

