#include "Allocator.h"


Allocator::Allocator(size_t size, void * start)
{
	mStart = start;
	mSize = size;
	mUsed_Memory = 0;
	mNum_Allocation = 0;
}

Allocator::~Allocator()
{
	//assert(mNum_Allocation == 0 && mUsed_Memory == 0);
	mStart = nullptr;
	mSize = 0;
}


void * Allocator::getStart() const
{
	return mStart;
}

size_t Allocator::getSize() const
{
	return mSize;
}

size_t Allocator::getUsedMemory() const
{
	return mUsed_Memory;
}

size_t Allocator::getNumAllocation() const
{
	return mNum_Allocation;
}