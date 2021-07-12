#pragma once
#include "NecessaryHeaders.h"
#include "Component.h"
//#include "AgentCommunicationComponent.h"
#include "Pathfinder.h"

class AgentComponent : public Component
{
public:
	void init(Pathfinder* pathfinder);// , AgentCommunicationComponent blackboard );
	void update();
private:

};

