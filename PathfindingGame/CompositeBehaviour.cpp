#include "CompositeBehaviour.h"

CompositeBehaviour* CompositeBehaviour::add(Behaviour* childBehaviour)
{
	children.push_back(childBehaviour);
	return this;
}

CompositeBehaviour* CompositeBehaviour::addMultiple(int count, ...)
{
	//goes through va_list for the extra arguments
	va_list arguments;
	va_start(arguments, count);
	children.reserve(count);
	for (int i = 0; i < count; i++)
	{
		children.push_back(va_arg(arguments, Behaviour*));
	}
	va_end(arguments);

	return this;
}

void CompositeBehaviour::remove(Behaviour* childBehaviour)
{
	children.erase(std::remove(children.begin(), children.end(), childBehaviour), children.end());
}

CompositeBehaviour::~CompositeBehaviour()
{
	for (auto child : children)
	{
		delete child;
	}
}
