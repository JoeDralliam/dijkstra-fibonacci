#ifndef FIBONACCI_HEAP_H
#define FIBONACCI_HEAP_H

#include "heap.h"

struct fibonacci_root;

/* Liste doublement chainés des racines du tas */
typedef struct fibonacci_root {
    heap_t* heap;
    struct fibonacci_root* prev;
    struct fibonacci_root* next;
} fibonacci_root;


/* Représentation du tas. min pointe vers la racine de valeur minimal */
typedef struct {
    size_t nelems;
    fibonacci_root* min;
} fibonacci_heap;


/*
Construit un nouveau tas
*/
fibonacci_heap* fibonacci_create();


void fibonacci_free(fibonacci_heap* heap);

/*
Teste si le tas est vide
*/
_Bool fibonacci_empty(fibonacci_heap* heap);

/*
Ajoute un élément de valeur val au tas, auquel on associe les données data
*/
void fibonacci_add(fibonacci_heap* heap, int val, void* data);

/* 
Supprime l'élément de valeur minimal et renvoie sa valeur.
Les données associées sont écrites dans (*dataOut).
 */
int fibonacci_extract_min(fibonacci_heap* heap, void** dataOut);

/* Affiche le tas sur la sortie standard */
void fibonacci_dump(fibonacci_heap* heap);

#endif
