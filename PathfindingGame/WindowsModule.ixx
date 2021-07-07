module;

#include <windows.h>

export module WindowsModule;

export int Module_QueryPerformanceCounter(unsigned long long int* lpPerformanceCount)
{
	return QueryPerformanceCounter((LARGE_INTEGER*)lpPerformanceCount);
}

export int Module_QueryPerformanceFrequency(unsigned long long int* lpPerformanceFrequency)
{
	return QueryPerformanceFrequency((LARGE_INTEGER*)lpPerformanceFrequency);
}