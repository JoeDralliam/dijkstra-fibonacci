#ifndef DIJKSTRA_H
#define DIJKSTRA_H

#include "graph.h"



typedef gen_array distance_list;


/*
Execute l'algorithme de dijkstra.
La distance du sommet from au sommet i est stocké dans la i eme case du résultat.
Le chemin du sommet from au somme i est stocké (sous forme de tableau) dans paths[i]
L'appelant doit libérer les résultat et paths avec respectivement dijkstra_free_distances et dijkstra_free_paths
*/
distance_list dijkstra(graph_t* graph, size_t from, gen_array* paths);

void dijkstra_free_paths(gen_array* paths);

void dijkstra_free_distances(distance_list* distances);

#endif
