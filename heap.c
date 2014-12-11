#include "heap.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

static _Bool pred(void* heap, void* parent_val);

static _Bool is_min_heap(heap_t* heap)
{
    return array_forall(&heap->children, &pred, heap); 
    
}

_Bool pred(void* heap0, void* parent0)
{
    heap_t* heap = (heap_t*)heap0;
    heap_t* parent = (heap_t*)parent0;
    return parent->val <= heap->val && is_min_heap(heap);
}


heap_t* heap_create(int val, void* data)
{
    heap_t* heap = malloc(sizeof(heap_t));
    heap->val = val;
    heap->data = data;
    heap->children = array_create();
    return heap;
}

void heap_free(heap_t* heap)
{
    array_free(&heap->children);
    free(heap);
}

void heap_recursive_free(heap_t* heap)
{
    size_t l = array_length(&heap->children);
    for(size_t i = 0; i < l; ++i)
    {
	heap_recursive_free((heap_t*)array_get(&heap->children, i));
    }
    
    heap_free(heap);
}

void heap_add_to_root(heap_t* heap, heap_t* child)
{
    assert(heap != child);
/*    assert(is_min_heap(heap));
    assert(is_min_heap(child));
*/
    assert(heap->val <= child->val);

        
    array_push_back(&heap->children, child);
    
/*    assert(is_min_heap(heap)); */
    
}

void heap_dump(heap_t* heap, int tabs)
{
    for(int i = 0; i < tabs; ++i)
    {
	printf("  ");
    }
    printf("|-");
    printf("%d\n", heap->val);
    for(size_t i = 0; i < array_length(&heap->children); ++i)
    {
	heap_t* child = (heap_t*)array_get(&heap->children, i);
	heap_dump(child, tabs + 1);
    }
}
