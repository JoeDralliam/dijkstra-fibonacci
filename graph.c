#include "graph.h"

#include <stdlib.h>
#include <stdio.h>

edge_t* edge_create(size_t vertex, size_t weight, void* data)
{
    edge_t* e = malloc(sizeof(edge_t));
    e->vertex = vertex;
    e->weight = weight;
    e->data   = data;
    return e;
}

graph_t graph_create(size_t verticesCount)
{
    graph_t graph = array_allocate(verticesCount);
    for(size_t i = 0; i < verticesCount; ++i)
    {
	edge_list* eList = malloc(sizeof(gen_array));
	*eList = array_create();
	array_push_back(&graph, eList);
    }
    return graph;
}

void graph_free(graph_t* graph, void (*free_data)(void*))
{
    size_t l = array_length(graph);
    for(size_t i = 0; i < l; ++i)
    {
	edge_list* eList = array_get(graph, i);

	if(free_data)
	{
	    size_t listLength = array_length(eList);
	    for(size_t j = 0; j < listLength; ++j)
	    {
		edge_t* e = array_get(eList, j);
		if(i < e->vertex)
		{
		    free_data(e->data);
		}
	    }
	}
	
	array_iter(eList, free);
	array_free(eList);
	free(eList);
    }
    array_free(graph);
}

void graph_add_edge(graph_t* graph, size_t v1, size_t v2, size_t w, void* data)
{
    edge_list* eList1 = array_get(graph, v1);
    edge_list* eList2 = array_get(graph, v2);
    
    array_push_back(eList1, (void*)edge_create(v2, w, data));
    array_push_back(eList2, (void*)edge_create(v1, w, data));
}

size_t graph_vertices_count(graph_t const* graph)
{
    return array_length(graph);
}

edge_list const* graph_adjacent_vertices(graph_t const* graph, size_t v)
{
    return array_get(graph, v);
}

graph_t graph_read_from_file(FILE* fin)
{
    size_t nVert, nEdge;
    fscanf(fin, "%lu%lu", &nVert, &nEdge);
    graph_t graph = graph_create(nVert);
    for(size_t i = 0; i < nEdge; ++i)
    {
	size_t from, to, weight;
	fscanf(fin, "%lu%lu%lu", &from, &to, &weight);
	graph_add_edge(&graph, from, to, weight, NULL);
    }
    return graph;
}