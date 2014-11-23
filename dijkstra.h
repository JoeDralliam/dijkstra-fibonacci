#ifndef DIJKSTRA_H
#define DIJKSTRA_H

#include "graph.h"

typedef gen_array distance_list;

distance_list dijkstra(graph_t* graph, size_t from, gen_array* paths);

void dijkstra_free_paths(gen_array* paths);

void dijkstra_free_distances(distance_list* distances);

#endif
