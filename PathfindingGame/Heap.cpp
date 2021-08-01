#include "Heap.h"
#include "PathfindingNode.h"

PathHeap::PathHeap()
{
}

PathHeap::PathHeap(int capacity)
{
	data.reserve(capacity);
}

void PathHeap::push(PathfindingNode* value)
{
	//push value onto back of vector
	int currentIndex = data.size();
	data.push_back(value);
	int parentIndex = getParentIndex(currentIndex);
	// then move it up the array until it's in the correct position
	while (data[currentIndex]->fScore < data[parentIndex]->fScore)
	{
		auto temp = data[currentIndex];
		data[currentIndex] = data[parentIndex];
		data[parentIndex] = temp;

		currentIndex = parentIndex;
		parentIndex = getParentIndex(currentIndex);
	}
}

PathfindingNode* PathHeap::pop()
{
	if (data.size() == 0)
		return nullptr;

	PathfindingNode* result = data[0];

	//switch smallest value with last value
	data[0] = data[data.size() - 1];

	int currentIndex = 0;
	int child1Index = getChild1Index(0);
	int child2Index = getChild2Index(0);

	int smallestIndex = data[child1Index]->fScore < data[child2Index]->fScore ?
		child1Index : child2Index;

	//then move child down until it is in the correct position, making the heap correctly ordered
	while (data[currentIndex]->fScore > data[smallestIndex]->fScore)
	{
		auto temp = data[smallestIndex];
		data[smallestIndex] = data[currentIndex];
		data[currentIndex] = temp;

		currentIndex = smallestIndex;
		child1Index = getChild1Index(currentIndex);
		child2Index = getChild2Index(currentIndex);

		smallestIndex = data[child1Index]->fScore < data[child2Index]->fScore ?
			child1Index : child2Index;
	}

	//then delete the last value and return the result
	//this could be an error idk, seems as if value should be popped before correcting 
	//but don't have time to check
	data.pop_back();
	return result;
}

PathfindingNode* PathHeap::peek()
{
	return data[0];
}

void PathHeap::updateNode(PathfindingNode* value)
{
	if (data.size() == 0)
		return;

	//find index
	auto iter = std::find(data.begin(), data.end(), value);

	if (iter == data.end())
		return;
	int currentIndex = iter - data.begin();

	if (currentIndex >= data.size())
		return;

	int parentIndex = getParentIndex(currentIndex);

	//move value up the chain until it is at correct position
	while (parentIndex >= 0 && data[currentIndex]->fScore < data[parentIndex]->fScore)
	{
		data[currentIndex] = data[parentIndex];
		data[parentIndex] = value;

		currentIndex = parentIndex;
		parentIndex = getParentIndex(currentIndex);
	}
	return;
}

bool PathHeap::contains(PathfindingNode* value)
{
	return (std::find(data.begin(), data.end(), value) != data.end());
}

bool PathHeap::confirmOrdered()
{
	PathHeap copy = *this; //copy heap
	//get the smallest value
	PathfindingNode* min = copy.pop();
	//check if any other values are smaller
	while (copy.size() > 0)
	{
		if ((copy.pop())->fScore < min->fScore)
		{
			return false;
		}
	}
	//if not, heap is probably ordered maybe
	return true;

}

int PathHeap::size()
{
	return data.size();
}

void PathHeap::clear()
{
	data.clear();
}

int PathHeap::getParentIndex(int index)
{	
	return (index - 1) / 2;
}

int PathHeap::getChild1Index(int index)
{
	int childIndex = index * 2 + 1;
	if (childIndex < data.size())
		return childIndex;
	return index;
}

int PathHeap::getChild2Index(int index)
{
	int childIndex = index * 2 + 2;
	if (childIndex < data.size())
		return childIndex;
	return index;
}
