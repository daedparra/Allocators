// PG14David_Programming3_a5.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "LinearAllocator.h"
#include "FreeListAllocator.h"

using namespace std;

class MyObject
{
public:
	int x;
	MyObject()
	{
		x = 0;
	}
	~MyObject() {}
};



int main()
{
	byte x[124];
	void* p = &x[0];
	LinearAllocator LA(sizeof(x), p);

	void* y = LA.allocate(sizeof(MyObject), __alignof(MyObject));
	MyObject* p2 = new (y) MyObject();


	FreeListAllocator FLA(sizeof(x), p);

	void* w = FLA.allocate(sizeof(MyObject), __alignof(MyObject));
	MyObject* p3 = new (w) MyObject();


	p2->x = 6;
	p3->x = 60;




	LA.clear();

	FLA.deallocate(p3);


	// https://github.com/mtrebi/memory-allocators
	// https://www.gamedev.net/articles/programming/general-and-gameplay-programming/c-custom-memory-allocation-r3010/
}
