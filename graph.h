#ifndef GRAPH_H
#define GRAPH_H

#include "gen_array.h"
#include <stdio.h>


typedef gen_array graph_t;
typedef gen_array vertex_list;

typedef struct edge_t {
    size_t vertex;
    size_t weight;
} edge_t;

typedef gen_array edge_list;

graph_t graph_create(size_t verticesCount);

void graph_free(graph_t* graph);

void graph_add_edge(graph_t* graph, size_t v1, size_t v2, size_t weight);

size_t graph_vertices_count(graph_t const* graph);

edge_list const* graph_adjacent_vertices(graph_t const* graph, size_t v);

graph_t graph_read_from_file(FILE* fin);

#endif
