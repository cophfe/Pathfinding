#pragma once
#include <vector>
#include <algorithm>
#include "PathfindingNode.h"

class PathHeap
{
public:
	PathHeap();
	PathHeap(int capacity);
	//no need for copy & move constructors and assigners because there are no 'new' keywords (aka vector class handles it)

	void push(PathfindingNode* value);
	PathfindingNode* pop();
	PathfindingNode* peek();
	void updateNode(PathfindingNode* value);
	bool contains(PathfindingNode* value);

	bool confirmOrdered();

	int size();
	void clear();
private:
	std::vector<PathfindingNode*> data;

	int getParentIndex(int index);
	int getChild1Index(int index);
	int getChild2Index(int index);
};