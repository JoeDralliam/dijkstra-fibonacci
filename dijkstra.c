#include "dijkstra.h"

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>

#include "fibonacci_heap.h"

/* 
Donnée conservée à l'intérieur du tas de fibonacci
 */ 
typedef struct data_t
{
    edge_t* edge;
    size_t origin;
} data_t;

/*
Crée un nouvelle donnée. 
Si edge une aretes sortant du sommet initial ("from"), parent vaut NULL.
*/
data_t* data_create(edge_t* edge, size_t origin)    
{
    data_t* d = malloc(sizeof(data_t));
    d->edge   = edge;
    d->origin = origin;    
    return d;
}

/*
Reconstruit le chemin de from à d dans le tableau res.
Le chemin est representé par un suite d'aretes.
*/
void data_extract_path(data_t* d, gen_array* paths)
{
    gen_array* path = malloc(sizeof(gen_array));
    *path = array_copy((gen_array*)array_get(paths, d->origin));
    array_push_back(path, (void*)d->edge);
    array_set(paths, d->edge->vertex, path);    
}

distance_list dijkstra(graph_t* graph, size_t from, gen_array* paths)
{
    size_t vCount = graph_vertices_count(graph);
    
    *paths = array_init(vCount);
/*    for(size_t i = 0; i < vCount; ++i)
    {
	gen_array* path = malloc(sizeof(gen_array));
	*path = array_create();
	array_push_back(paths, (void*)path);
    }
*/
    distance_list distances = array_init(vCount);
    
    gen_array visite = array_allocate(vCount);
    
    gen_array datas = array_allocate(vCount);
    for(size_t i = 0; i < vCount; ++i)
    {
	array_push_back(&visite, (void*)false);
    }


    /* Initialisation du tas */
    fibonacci_heap* toVisit = fibonacci_create();

    {
	vertex_list const* adj = graph_adjacent_vertices(graph, from);
	size_t nAdj = array_length(adj);
	for(size_t i = 0; i < nAdj; ++i)
	{
	    edge_t* edge = (edge_t*)array_get(adj, i);
	    data_t* chData = data_create(edge, from);
	    array_push_back(&datas, chData);
	    fibonacci_add(toVisit, edge->weight, chData);
	}

	gen_array* path = malloc(sizeof(gen_array));
	*path = array_create();
	array_set(paths, from, path);
	array_set(&distances, from, (void*)0);
	array_set(&visite, from, (void*)true);
    }
    
    while(!fibonacci_empty(toVisit))
    {
	void* dataGen;
	size_t minDist = fibonacci_extract_min(toVisit, &dataGen);
	data_t* data = dataGen;

	size_t vertex = data->edge->vertex;
	if(!(_Bool)array_get(&visite, vertex))
	{
	    data_extract_path(data, paths);
	    array_set(&distances, vertex, (void*)minDist);
	    array_set(&visite, vertex, (void*)true);
	    
	    vertex_list const* adj = graph_adjacent_vertices(graph, vertex);
	    size_t nAdj = array_length(adj);
	    for(size_t i = 0; i < nAdj; ++i)
	    {
		edge_t* edge = (edge_t*)array_get(adj, i);

		/* This check may be omitted */
		if(!(_Bool)array_get(&visite, edge->vertex))
		{
		    data_t* chData = data_create(edge, vertex);
		    array_push_back(&datas, chData);
		    fibonacci_add(toVisit, minDist + edge->weight, chData);
		}
	    }
	}
    }

    fibonacci_free(toVisit);

    array_iter(&datas, free);
    array_free(&datas);

    array_free(&visite);

    return distances;
}

typedef struct {
    data_t* data;
    size_t totWeight;
} array_elt;

void weighted_array_add(gen_array* array, size_t totWeight, data_t* data)
{
    array_elt* elt = (array_elt*)malloc(sizeof(array_elt));
    elt->totWeight = totWeight;
    elt->data = data;
    array_push_back(array, (void*)elt);
}

size_t weighted_array_extract_min(gen_array* array, data_t** data)
{
    size_t len = array_length(array);
    assert(len > 0);

    size_t minIdx = 0;
    size_t minWgt = ((array_elt*)array_get(array, minIdx))->totWeight;
    
    for(size_t i = 1; i < len; ++i)
    {
	size_t wgt = ((array_elt*)array_get(array, i))->totWeight;
	if(minWgt > wgt)
	{
	    minIdx = i;
	    minWgt = wgt;
	}
    }

    array_elt* elt = (array_elt*)array_get(array, minIdx);

    array_swap(array, minIdx, len - 1); 
    array_pop_back(array);

    *data = elt->data;
    size_t res = elt->totWeight;
    free(elt);
    return res;
}

void weighted_array_free(gen_array* array)
{
    array_iter(array, free);
    array_free(array);
}

distance_list dijkstra_quadratic(graph_t* graph, size_t from, gen_array* paths)
{
    size_t vCount = graph_vertices_count(graph);
    
    *paths = array_init(vCount);

    distance_list distances = array_init(vCount);
    
    gen_array visite = array_allocate(vCount);
    
    gen_array datas = array_allocate(vCount);
    for(size_t i = 0; i < vCount; ++i)
    {
	array_push_back(&visite, (void*)false);
    }


    /* Initialisation du tas */
    gen_array toVisit = array_create();

    {
	vertex_list const* adj = graph_adjacent_vertices(graph, from);
	size_t nAdj = array_length(adj);
	for(size_t i = 0; i < nAdj; ++i)
	{
	    edge_t* edge = (edge_t*)array_get(adj, i);
	    data_t* chData = data_create(edge, from);
	    array_push_back(&datas, chData);
	    weighted_array_add(&toVisit, edge->weight, chData);
	}

   	gen_array* path = malloc(sizeof(gen_array));
	*path = array_create();
	array_set(paths, from, path); 
     }
    
    while(array_length(&toVisit) != 0)
    {
	data_t* data;
	size_t minDist = weighted_array_extract_min(&toVisit, &data);

	size_t vertex = data->edge->vertex;
	if(!(_Bool)array_get(&visite, vertex))
	{
	    data_extract_path(data, paths);
	    array_set(&distances, vertex, (void*)minDist);
	    array_set(&visite, vertex, (void*)true);
	    
	    vertex_list const* adj = graph_adjacent_vertices(graph, vertex);
	    size_t nAdj = array_length(adj);
	    for(size_t i = 0; i < nAdj; ++i)
	    {
		edge_t* edge = (edge_t*)array_get(adj, i);

		/* This check may be omitted */
		if(!(_Bool)array_get(&visite, edge->vertex))
		{
		    data_t* chData = data_create(edge, vertex);
		    array_push_back(&datas, chData);
		    weighted_array_add(&toVisit, minDist + edge->weight, chData);
		}
	    }
	}
    }

    weighted_array_free(&toVisit);

    array_iter(&datas, free);
    array_free(&datas);

    array_free(&visite);

    return distances;
}


static void array_free_gen(void* a)
{
    if(a)
    {
	array_free((gen_array*)a);
    }
}

void dijkstra_free_paths(gen_array* paths)
{
    array_iter(paths, array_free_gen);
    array_iter(paths, free);
    array_free(paths);
}


void dijkstra_free_distances(distance_list* distances)
{
    array_free(distances);
}
