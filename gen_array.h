#ifndef GEN_ARRAY_H
#define GEN_ARRAY_H

#include <stddef.h>

/* Dynamic array of ints */

typedef void* element_t;

typedef struct gen_array {
    size_t allocated;
    size_t used;
    element_t* array;
} gen_array;

/* Create an array of length zero */
gen_array array_create();

/* Create an array of length zero, with pre-allocated memory */
gen_array array_allocate(size_t l);

/* Create an array of a given length, and init it with NULL */
gen_array array_init(size_t length);


/* Free an array */
void array_free(gen_array* a);

/* Add an element to the end */
void array_push_back(gen_array* a, element_t e);
/* Remove the element at the end */
void array_pop_back(gen_array* a);

/* Swap two elements */
void array_swap(gen_array* a, size_t i, size_t j);

/* Increase allocated memory to specified value.
   Does nothing if allocated memory is larger */
void array_grow(gen_array* a, size_t l);

void array_shrink(gen_array* a, size_t l);

element_t array_get(gen_array const* a, size_t i);
void array_set(gen_array* a, size_t i, element_t val);
size_t array_length(gen_array const* a);

/* return last element */
void* array_back(gen_array const* a);

/* a[0] && a[1] && .. && a[n] */
_Bool array_forall(gen_array* a, _Bool (*pred)(element_t, void*), void* param);

/* a[0] || a[1] || .. || a[n] */
_Bool array_exists(gen_array* a, _Bool (*pred)(element_t, void*), void* param);

void array_iter(gen_array* a, void (*fun)(element_t));

#endif


