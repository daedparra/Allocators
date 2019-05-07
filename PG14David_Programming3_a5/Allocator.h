#pragma once

#include <assert.h>
#include <new>
#include <functional>

typedef uint8_t byte;

static const byte DEFAULT_ALIGNMENT = 8;

class Allocator
{
public:
	Allocator(size_t size, void* start);
	virtual ~Allocator();

	virtual void* allocate(size_t size, byte alignment = 4) = 0;
	virtual void deallocate(void* p) = 0;
	void* getStart() const;
	size_t getSize() const;
	size_t getUsedMemory() const;
	size_t getNumAllocation() const;

protected:

	void* mStart;
	size_t mSize;
	size_t mUsed_Memory;
	size_t mNum_Allocation;
};

namespace allocator
{

	template<class T> T* allocateNew(Allocator& allocator)
	{
		return new (allocator.allocate(sizeof(T), __alignof(T))) T;
	}

	template<class T> T* allocateNew(Allocator& allocator, const T& t)
	{
		return new (allocator.allocate(sizeof(T), __alignof(T))) T(t);
	}


	template<class T> void deallocateDelete(Allocator& allocator, T& object)
	{
		object->~T();
		allocator->deallocate(&object);
	}

	template<class T> T* allocateArray(Allocator& allocator, size_t length)
	{
		assert(length != 0);
		byte headerSize = sizeof(size_t) / sizeof(T);

		if (sizeof(size_t) % sizeof(T) > 0)
		{
			headerSize += 1;
		}


		T* p = ((T*)allocator.allocate(sizeof(T)*(length + headerSize), __alignof(T))) + headerSize;

		for (size_t i = 0; i < length; i++)
		{
			new(&p) T;
		}

		return p;
	}

	template<class T> void deallocateArray(Allocator& allocator, T* array)
	{
		assert(array != nullptr);
		size_t length = *(((size_t*)array) - 1);

		byte headerSize = sizeof(size_t) / sizeof(T);
		if (sizeof(size_t) % sizeof(T) > 0)
		{
			headerSize += 1;
		}
		allocator.deallocate(array - headerSize);
	}
}

