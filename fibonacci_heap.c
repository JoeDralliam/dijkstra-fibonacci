#include "fibonacci_heap.h"

#include <assert.h>
#include <math.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#include "exit_code.h"


static void create_root(fibonacci_heap* heap, int val)
{
    fibonacci_root* root = malloc(sizeof(fibonacci_root));
    root->heap = heap_create(val);
    
    fibonacci_root* prev = heap->min;
    fibonacci_root* next = prev->next;
    
    root->prev = prev;
    prev->next = root;

    root->next = next;
    next->prev = root;

    /* Update min value */
    if(heap->min->heap->val > val)
    {
	heap->min = root;
    }
}

static void create_initial_root(fibonacci_heap* heap, int val)
{
    fibonacci_root* root = malloc(sizeof(fibonacci_root));
    root->heap = heap_create(val);
    
    root->prev = root;
    root->next = root;
    heap->min = root;
}

static void free_root(fibonacci_root* rt)
{
    heap_free(rt->heap);
    free(rt);
}

static void free_recursive_root(fibonacci_root* rt)
{
    heap_recursive_free(rt->heap);
    free(rt);
}

static void insert_root(heap_t* hp, fibonacci_root* prev, fibonacci_root* next)
{
    assert(prev->next == next);
    assert(next->prev == prev);
    
    fibonacci_root* root = malloc(sizeof(fibonacci_root));
    root->heap = hp;
    
    root->prev = prev;
    prev->next = root;

    root->next = next;
    next->prev = root;   
}

static size_t degree_root(fibonacci_root* rt)
{
    return array_length(&rt->heap->children);
}

static fibonacci_root* fusion_root(fibonacci_root* a, fibonacci_root* b)
{
    if(a->heap->val > b->heap->val)
    {
	return fusion_root(b, a);
    }
    heap_add_to_root(a->heap, b->heap);
    
    fibonacci_root* prev = b->prev;
    fibonacci_root* next = b->next;
    
    prev->next = next;
    next->prev = prev;
    
    free(b);
    
    return a;
}


static void consolidate(fibonacci_heap* heap, fibonacci_root* base)
{
    size_t maxd = (size_t)log2((double)heap->nelems);
    
    gen_array arr = array_init(maxd + 1);
    
    fibonacci_root* lastRoot = base->prev;
    fibonacci_root* pred = NULL;
    fibonacci_root* rt = base;
    do
    {
	fibonacci_root* current = rt;
	pred = rt;
	rt = rt->next;

	size_t degree = degree_root(current);
	while(array_get(&arr, degree) != NULL)
	{
	    fibonacci_root* other = array_get(&arr, degree);
	    current = fusion_root(current, other);
	    array_set(&arr, degree, NULL);
	    degree++;
	}
	array_set(&arr, degree, current);
    } while(pred != lastRoot);

    fibonacci_root* first = NULL;
    fibonacci_root* last = NULL;

    for(size_t i = 0; i < maxd + 1; ++i)
    {
	fibonacci_root* curr = array_get(&arr, i);
	if(curr != NULL)
	{
	    if(first == NULL)
	    {
		first = curr;
		heap->min = curr;
	    }
	    else
	    {
		last->next = curr;
		curr->prev = last;
		if(heap->min->heap->val > curr->heap->val)
		{
		    heap->min = curr;
		}
	    }
	    last = curr;
	}
	}
	    array_free(&arr);
	    first->prev = last;
	    last->next = first;
    
}


fibonacci_heap* fibonacci_create()
{
    fibonacci_heap* heap = malloc(sizeof(fibonacci_heap));
    heap->min = NULL;
    heap->nelems = 0;
    return heap;
}

void fibonacci_free(fibonacci_heap* heap)
{
    if(!fibonacci_empty(heap))
    {
	fibonacci_root* rt = heap->min;
	do
	{
	    fibonacci_root* next = rt->next;
	    free_recursive_root(rt);
	    rt = next;
	} while(rt != heap->min);
    }
    free(heap);
}

_Bool fibonacci_empty(fibonacci_heap* heap)
{
    return heap->nelems == 0;
}

void fibonacci_add(fibonacci_heap* heap, int val)
{
    if(!fibonacci_empty(heap))
    {
	create_root(heap, val);
    }
    else
    {
	create_initial_root(heap, val);
    }
    heap->nelems++;
}

int fibonacci_extract_min(fibonacci_heap* heap)
{
    if(fibonacci_empty(heap))
    {
	fprintf(stderr, "Try to extract minimum from an empty fibonacci heap\n");
	assert(false);
	exit(out_of_bounds);
    }
    fibonacci_root* min_root = heap->min;
    heap_t* min_heap = min_root->heap;
    
    int min_val = min_heap->val;
    


     
    if(heap->nelems == 1)
    {
	free_root(min_root);
	heap->min = NULL;
	heap->nelems--;
	return min_val;
    }


    fibonacci_root* prev = min_root->prev;    
    size_t nchildren = array_length(&min_heap->children);
    for(size_t i = 0; i < nchildren; ++i)
    {
	insert_root(array_get(&min_heap->children, i), prev, min_root);
	prev = prev->next;
    }

    fibonacci_root* next = min_root->next;

    prev->next = next;
    next->prev = prev;
    
    free_root(min_root);
    consolidate(heap, prev);
    heap->nelems--;
    
    return min_val;
}

void fibonacci_dump(fibonacci_heap* heap)
{
    fibonacci_root* rt = heap->min;
    printf("Fibonacci heap (%lu elements):\n", heap->nelems);
    do
    {
       heap_dump(rt->heap, 0);
       rt = rt->next;
    } while(rt != heap->min);
    printf("\n");
}
