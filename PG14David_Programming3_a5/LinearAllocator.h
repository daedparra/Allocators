#include "Allocator.h"
#include "Pointer.h"
#include <stdint.h>

class LinearAllocator : public Allocator
{
public:
	LinearAllocator(size_t size, void* start);
	~LinearAllocator();

	void* allocate(size_t size, byte alignment) override;
	void deallocate(void* p) override;
	void clear();

private:
	LinearAllocator(const LinearAllocator&);


	LinearAllocator& operator=(const LinearAllocator&) = delete;
	void* mCurrentPosition;
};

