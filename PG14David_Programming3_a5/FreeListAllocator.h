#pragma once
#include "Allocator.h"
#include "Pointer.h"

class FreeListAllocator : public Allocator
{
public:
	FreeListAllocator(size_t size, void* start);
	~FreeListAllocator();

	void* allocate(size_t size, byte alignment) override;
	void deallocate(void* p) override;

private:


	// Struckts to help with List allocation;
	struct AllocationHeader
	{
		size_t size;
		byte adjustment;
	};

	struct FreeBlock
	{
		size_t size;
		FreeBlock* next;
	};

	// Preventing Copies
	FreeListAllocator& operator=(const FreeListAllocator&) = delete;
	FreeBlock* mFreeBlocks;
};

