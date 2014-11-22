#include "gen_array.h"
#include "exit_code.h"

#include <assert.h>
#include <stdlib.h>
#include <stdbool.h>

gen_array array_create()
{
    gen_array a = { .allocated =  0, .used = 0, .array = 0 };
    return a;
}

gen_array array_allocate(size_t l)
{
    element_t* array = malloc(l * sizeof(element_t));
    if(array == 0)
    {
	exit(out_of_memory_code);
    }
    
    gen_array a = { .allocated = l, .used = 0, .array = array };
    return a;
}

gen_array array_init(size_t length)
{
    gen_array a = array_allocate(length);
    a.used = length;
    for(size_t i = 0; i < length; ++i)
    {
	a.array[i] = NULL;
    }
    return a;
}



void array_free(gen_array* a)
{
    if(a->array != 0)
    {
	free(a->array);
	a->used = 0;
	a->allocated = 0;
	a->array = 0;
    }
}

void array_resize(gen_array* a, size_t l)
{
    size_t used = a->used;
    element_t* array = malloc(l * sizeof(element_t));
    if(array == 0)
    {
	exit(out_of_memory_code);
    }

    for(size_t i = 0; i < a->used; ++i)
    {
	array[i] = a->array[i];
    }

    if(a->array != 0)
    {
	free(a->array);
    } 

    a->used = used;
    a->allocated = l;
    a->array = array;
}

void array_push_back(gen_array* a, element_t e)
{
    if(a->used >= a->allocated)
    {
	array_resize(a, (a->allocated < 5) ? 10 : ((3 * a->allocated) / 2));
    }
    a->array[a->used]= e;
    ++a->used;
}

void array_pop_back(gen_array* a)
{
    assert(a->used > 0);
    --a->used;
    if(a->used < a->allocated / 2)
    {
	array_resize(a, a->allocated / 2);
    }
}

void array_swap(gen_array* a, size_t i, size_t j)
{
    assert(i < a->used && j < a->used);
    element_t tmp = a->array[i];
    a->array[i] = a->array[j];
    a->array[j] = tmp;
}


void array_grow(gen_array* a, size_t l)
{
    if(l > a->allocated)
    {
	array_resize(a, l);
    }
}

void array_shrink(gen_array* a, size_t l)
{
    assert(l >= a->used);
    if(l < a->allocated)
    {
	array_resize(a, l);
    }
}


element_t array_get(gen_array* a, size_t i)
{
    assert(i < a->used);
    return a->array[i];
}

void array_set(gen_array* a, size_t i, element_t val)
{
    assert(i < a->used);
    a->array[i] = val;
}

size_t array_length(gen_array* a)
{
    return a->used;
}

_Bool array_forall(gen_array* a, _Bool (*pred)(element_t, void*), void* param)
{
    for(size_t i = 0; i < a->used; ++i)
    {
	if(!pred(a->array[i], param))
	{
	    return false;
	}
    }
    return true;
}

    
_Bool array_exists(gen_array* a, _Bool (*pred)(element_t, void*), void* param)
{
    for(size_t i = 0; i < a->used; ++i)
    {
	if(pred(a->array[i], param))
	{
	    return true;
	}
    }
    return false;
}
