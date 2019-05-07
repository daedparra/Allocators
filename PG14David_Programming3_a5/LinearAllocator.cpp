#include "LinearAllocator.h"


LinearAllocator::LinearAllocator(size_t size, void * start) : Allocator(size, start), mCurrentPosition(start)
{
	assert(size > 0);
}

LinearAllocator::~LinearAllocator()
{
	mCurrentPosition = nullptr;
}

void * LinearAllocator::allocate(size_t size, byte alignment)
{
	assert(size != 0);

	byte adjustment = pointer_math::alignForwardAdjustment(mCurrentPosition, alignment);

	if (mUsed_Memory + adjustment + size > mSize)
	{
		return nullptr;
	}

	unptr aligned_address = (unptr)mCurrentPosition + adjustment;
	mCurrentPosition = (void*)(aligned_address + size);
	mUsed_Memory += size + adjustment;
	mNum_Allocation++;

	return (void*)aligned_address;
}

void LinearAllocator::deallocate(void * p)
{
	assert(false && "clear()");
}

void LinearAllocator::clear()
{
	mNum_Allocation = 0;
	mUsed_Memory = 0;
	mCurrentPosition = mStart;
}
