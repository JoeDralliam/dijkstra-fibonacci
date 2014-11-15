#ifndef INT_ARRAY_H
#define INT_ARRAY_H

#include <stddef.h>

/* Dynamic array of ints */

typedef struct {
    size_t allocated;
    size_t used;
    int* array;
} int_array;

/* Create an array of length zero */
int_array array_create();

/* Create an array of length zero, with pre-allocated memory */
int_array array_allocate(size_t l);

/* Create an array of a given length, and init it with val */
int_array array_init(size_t length, int initVal);


/* Free an array */
void array_free(int_array a);

/* Add an element to the end */
void array_push_back(int_array a, int e);
/* Remove the element at the end */
void array_pop_back(int_array a);

/* Swap two elements */
void array_swap(int_array a, size_t i, size_t j);

/* Increase allocated memory to specified value.
   Does nothing if allocated memory is larger */
void array_grow(int_array a, size_t l);

void array_shrink(int_array a, size_t l);

int array_get(int_array a, size_t i);
void array_set(int_array a, size_t i, int val);
size_t array_length(int_array a);


#endif
