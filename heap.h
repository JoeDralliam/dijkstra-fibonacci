#ifndef HEAP_H
#define HEAP_H

#include "gen_array.h"

typedef struct heap_t {
    int val;
    void* data;
    gen_array children;
} heap_t;

heap_t* heap_create(int val, void* data);

void heap_free(heap_t*);
void heap_recursive_free(heap_t*);

void heap_add_to_root(heap_t* heap, heap_t* child);

void heap_dump(heap_t* heap, int tabs);

#endif
