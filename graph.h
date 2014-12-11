#ifndef GRAPH_H
#define GRAPH_H

#include "gen_array.h"
#include <stdio.h>


/* 
Un graphe est représenté par liste d'adjacence.
La i ème case contient les aretes issue du sommet i
Le nombre de sommet est fixé à la construction
*/
typedef gen_array graph_t;


typedef gen_array vertex_list;

/* Type des aretes (pondérées) */
typedef struct edge_t {
    size_t vertex;
    size_t weight;
    void* data;
} edge_t;

typedef gen_array edge_list;

graph_t graph_create(size_t verticesCount);

/* 
Libère le graphe.
Le second paramètre sert à libérer les données associées aux arêtes
(NULL pour ne rien faire)
*/
void graph_free(graph_t* graph, void (*free_data)(void*));

/*
Ajoute une arêtes entre v1 et v2, de poids weight auquel est attaché data (éventuellement NULL)
*/
void graph_add_edge(graph_t* graph, size_t v1, size_t v2, size_t weight, void* data);

/*
Nombre de sommet du graphe
*/
size_t graph_vertices_count(graph_t const* graph);


/*
Liste des arêtes issues du sommet v
*/
edge_list const* graph_adjacent_vertices(graph_t const* graph, size_t v);


/*
Teste la connexité du graph
*/
_Bool graph_connexe(graph_t const* graph);


_Bool graph_adjacent(graph_t* graph, size_t a, size_t b);

/*
Lis le graphe à partir d'un fichier
*/
graph_t graph_read_from_file(FILE* fin, size_t* nEdges);

#endif
