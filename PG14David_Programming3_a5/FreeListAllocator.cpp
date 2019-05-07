#include "FreeListAllocator.h"

FreeListAllocator::FreeListAllocator(size_t size, void * start) : Allocator(size, start), mFreeBlocks((FreeBlock*)mStart)
{
	assert(size > sizeof(FreeBlock));
	mFreeBlocks->size = size;
	mFreeBlocks->next = nullptr;
}

FreeListAllocator::~FreeListAllocator()
{
	mFreeBlocks = nullptr;
}

void * FreeListAllocator::allocate(size_t size, byte alignment)
{
	assert(size != 0 && alignment != 0);

	FreeBlock* previouseFreeBlock = nullptr;
	FreeBlock* freeBlock = mFreeBlocks;

	while (freeBlock != nullptr)
	{
		byte adjustment = pointer_math::alignForwardAdjustmentWithHeader(freeBlock, alignment, sizeof(AllocationHeader));
		size_t totalSize = size + adjustment;
		if (freeBlock->size < totalSize)
		{
			previouseFreeBlock = freeBlock;
			freeBlock = freeBlock->next;
			continue;
		}

		static_assert(sizeof(AllocationHeader) >= sizeof(FreeBlock), "AllocationHeader >= FreeBlock");

		if (freeBlock->size - totalSize <= sizeof(AllocationHeader))
		{
			totalSize = freeBlock->size;

			if (previouseFreeBlock != nullptr)
			{
				previouseFreeBlock->next = freeBlock->next;
			}
			else
			{
				freeBlock = freeBlock->next;
			}
		}
		else
		{

			FreeBlock* nextBlock = (FreeBlock*)(pointer_math::add(freeBlock, totalSize));

			nextBlock->size = freeBlock->size - totalSize;
			nextBlock->next = freeBlock->next;

			if (previouseFreeBlock != nullptr)
			{
				previouseFreeBlock->next = nextBlock;
			}
			else
			{
				mFreeBlocks = nextBlock;
			}

			unptr alignedAddress = (unptr)freeBlock + adjustment;

			AllocationHeader* header = (AllocationHeader*)(alignedAddress - sizeof(AllocationHeader));
			header->size = totalSize;
			header->adjustment = adjustment;
			mUsed_Memory += totalSize;
			mNum_Allocation++;

			assert(pointer_math::alignForwardAdjustment((void*)alignedAddress, alignment) == 0);

			return (void*)alignedAddress;
		}



	}

	return nullptr;
}

void FreeListAllocator::deallocate(void * p)
{
	assert(p != nullptr);
	AllocationHeader* header = (AllocationHeader*)pointer_math::subtract(p, sizeof(AllocationHeader));

	unptr blockStart = reinterpret_cast<unptr>(p) - header->adjustment;
	size_t blockSize = header->size;
	unptr blockEnd = blockStart + blockSize;

	FreeBlock* previousFreeBlock = nullptr;
	FreeBlock* freeBlock = mFreeBlocks;

	while (freeBlock != nullptr)
	{
		if ((unptr)freeBlock >= blockEnd)
		{
			break;
		}
		previousFreeBlock = freeBlock;
		freeBlock = freeBlock->next;
	}

	if (previousFreeBlock == nullptr)
	{
		previousFreeBlock = (FreeBlock*)blockStart;
		previousFreeBlock->size = blockSize;
		previousFreeBlock->next = mFreeBlocks;
		mFreeBlocks = previousFreeBlock;
	}
	else if ((unptr)previousFreeBlock + previousFreeBlock->size == blockStart)
	{
		previousFreeBlock->size += blockSize;
	}
	else
	{
		FreeBlock* temp = (FreeBlock*)blockStart;
		temp->size = blockSize;
		temp->next = previousFreeBlock->next;
		previousFreeBlock->next = temp;
		previousFreeBlock = temp;
	}

	if (freeBlock != nullptr && (unptr)freeBlock == blockEnd)
	{
		previousFreeBlock->size += freeBlock->size;
		previousFreeBlock->next = freeBlock->next;
	}

	mNum_Allocation--;
	mUsed_Memory -= blockSize;
}
