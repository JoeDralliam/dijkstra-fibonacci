#ifndef FIBONACCI_HEAP_H
#define FIBONACCI_HEAP_H

#include "heap.h"

struct fibonacci_root;

typedef struct fibonacci_root {
    heap_t* heap;
    struct fibonacci_root* prev;
    struct fibonacci_root* next;
} fibonacci_root;


/* Pointer to the minimal root */
typedef struct {
    size_t nelems;
    fibonacci_root* min;
} fibonacci_heap;




fibonacci_heap* fibonacci_create();

void fibonacci_free(fibonacci_heap* heap);

_Bool fibonacci_empty(fibonacci_heap* heap);

void fibonacci_add(fibonacci_heap* heap, int val);

int fibonacci_extract_min(fibonacci_heap* heap);

void fibonacci_dump(fibonacci_heap* heap);

#endif
