#include "int_array.h"
#include "exit_code.h"
#include <assert.h>
#include <stdlib.h>

int_array array_create()
{
    int_array a = { .allocated =  0, .used = 0, .array = 0 };
    return a;
}

int_array array_allocate(size_t l)
{
    int* array = malloc(l * sizeof(int));
    if(array == 0)
    {
	exit(out_of_memory_code);
    }
    
    int_array a = { .allocated = l, .used = 0, .array = 0 };
    return a;
}

int_array array_init(size_t length, int initVal)
{
    int_array a = array_allocate(length);
    for(size_t i = 0; i < length; ++i)
    {
	a.array[i] = initVal;
    }
    return a;
}


void array_free(int_array a)
{
    if(a.array != 0)
    {
	free(a.array);
	a.used = 0;
	a.allocated = 0;
	a.array = 0;
    }
}

void array_resize(int_array a, size_t l)
{
    size_t used = a.used;
    int* array = malloc(l * sizeof(int));
    if(array == 0)
    {
	exit(out_of_memory_code);
    }

    for(size_t i = 0; i < a.used; ++i)
    {
	array[i] = a.array[i];
    }
    
    a.used = used;
    a.allocated = l;
    a.array = array;
}

void array_push_back(int_array a, int e)
{
    if(a.used > a.allocated)
    {
	array_resize(a, (a.allocated < 5) ? 10 : ((3 * a.allocated) / 2));
    }
    a.array[a.used]= e;
    ++a.used;
}

void array_pop_back(int_array a)
{
    assert(a.used > 0);
    --a.used;
    if(a.used < a.allocated / 2)
    {
	array_resize(a, a.allocated / 2);
    }
}

void array_swap(int_array a, size_t i, size_t j)
{
    assert(i < a.used && j < a.used);
    int tmp = a.array[i];
    a.array[i] = a.array[j];
    a.array[j] = tmp;
}


void array_grow(int_array a, size_t l)
{
    if(l > a.allocated)
    {
	array_resize(a, l);
    }
}

void array_shrink(int_array a, size_t l)
{
    assert(l >= a.used);
    if(l < a.allocated)
    {
	array_resize(a, l);
    }
}


int array_get(int_array a, size_t i)
{
    assert(i < a.used);
    return a.array[i];
}

void array_set(int_array a, size_t i, int val)
{
    assert(i < a.used);
    a.array[i] = val;
}

size_t array_length(int_array a)
{
    return a.used;
}
