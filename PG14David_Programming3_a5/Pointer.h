#pragma once
#include "Allocator.h"

typedef uintptr_t unptr;

namespace pointer_math
{
	void* alignForward(void* address, byte alignment);
	const void* alignForward(const void* address, byte alignment);

	byte alignForwardAdjustment(const void* address, byte alignment);
	byte alignForwardAdjustmentWithHeader(const void* address, byte alignment, byte header_size);


	void* add(void* p, size_t x);
	const void* add(const void* p, size_t x);


	void* subtract(void* p, size_t x);
	const void* subtract(const void* p, size_t x);

}


namespace pointer_math
{

	inline void* allignForward(void* address, byte alignment)
	{
		return (void*)((reinterpret_cast<unptr>(address) + static_cast<unptr>(alignment - 1)) & static_cast<unptr>(~(alignment - 1)));
	}

	inline const void* alignForward(const void* address, byte alignment)
	{
		return (void*)((reinterpret_cast<unptr>(address) + static_cast<unptr>(alignment - 1)) & static_cast<unptr>(~(alignment - 1)));
	}

	inline byte alignForwardAdjustment(const void* address, byte alignment)
	{
		byte adjustment = alignment - (reinterpret_cast<unptr>(address)& static_cast<unptr>(alignment - 1));

		if (adjustment == alignment)
			return 0; 

		return adjustment;
	}

	inline byte alignForwardAdjustmentWithHeader(const void* address, byte alignment, byte header_size)
	{
		byte adjustment = alignForwardAdjustment(address, alignment);
		byte needed_space = header_size;
		if (adjustment < needed_space)
		{
			needed_space -= adjustment;
			adjustment += alignment * (needed_space / alignment);
			if (needed_space % alignment > 0)
				adjustment += alignment;
		}
		return adjustment;
	}


	inline void* add(void* p, size_t x)
	{
		return (void*)(reinterpret_cast<unptr>(p) + x);
	}

	inline const void* add(const void* p, size_t x)
	{
		return (const void*)(reinterpret_cast<unptr>(p) + x);
	}

	inline void* subtract(void* p, size_t x)
	{
		return (void*)(reinterpret_cast<unptr>(p) - x);
	}

	inline const void* subtract(const void* p, size_t x)
	{
		return (const void*)(reinterpret_cast<unptr>(p) - x);
	}
}
