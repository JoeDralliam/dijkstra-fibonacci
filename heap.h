#ifndef HEAP_H
#define HEAP_H

#include "gen_array.h"

/* Tas classique */
typedef struct heap_t {
    int val;
    void* data;
    gen_array children;
} heap_t;

/* Construit un tas en lui attachant des données */
heap_t* heap_create(int val, void* data);

/* Libère un tas (ne libère pas ses fils */
void heap_free(heap_t*);

/* Libère un tas et ses fils */
void heap_recursive_free(heap_t*);

/* Ajoute un fils au niveau de la racine */
void heap_add_to_root(heap_t* heap, heap_t* child);

/* Affiche le tas */
void heap_dump(heap_t* heap, int tabs);

#endif
