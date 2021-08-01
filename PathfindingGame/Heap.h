#pragma once
#include <vector>
#include <algorithm>
#include "PathfindingNode.h"

class PathHeap
{
public:
	PathHeap();
	PathHeap(int capacity);

	//push value to heap
	void push(PathfindingNode* value);
	//pop value off heap
	PathfindingNode* pop();
	//check first value without removing it from heap
	PathfindingNode* peek();
	//update node position in heap (used if it's fScore changes)
	void updateNode(PathfindingNode* value);
	//check if value is in heap
	bool contains(PathfindingNode* value);
	//confirm the heap is ordered correctly (expensive!)
	bool confirmOrdered();
	//return size of heap
	int size();
	//clear heap
	void clear();
private:
	//actual values from inside heap
	std::vector<PathfindingNode*> data;

	//internal index getters
	int getParentIndex(int index);
	int getChild1Index(int index);
	int getChild2Index(int index);
};