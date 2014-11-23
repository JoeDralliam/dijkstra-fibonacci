#include "fibonacci_heap.h"

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>

#define ASSERT_EQ(x, y) \
    do { if(x != y) { printf("%d != %d",x,y); assert(x == y); } } while(false)

int main()
{
    fibonacci_heap* heap = fibonacci_create();

    for(int i = 0; i < 100; ++i)
    {
	fibonacci_add(heap, i, NULL);
    }

    for(int i = 0; i < 50; ++i)
    {
//	fibonacci_dump(heap);
	void* data;
	int min = fibonacci_extract_min(heap, &data);
	ASSERT_EQ(min, i);
    }

    for(int i = 0; i < 20; ++i)
    {
	fibonacci_add(heap, i * 4, NULL);
    }

    while(!fibonacci_empty(heap))
    {
//	fibonacci_dump(heap);
	void* data;
	fibonacci_extract_min(heap, &data);
    }

    fibonacci_free(heap);
}


