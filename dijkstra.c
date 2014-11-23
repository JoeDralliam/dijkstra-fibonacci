#include "dijkstra.h"

#include <stdbool.h>
#include <stdlib.h>

#include "fibonacci_heap.h"


typedef struct data_t
{
    struct data_t* parent;
    size_t vertex;
} data_t;

data_t* data_create(size_t vertex, data_t* parent)
{
    data_t* d = malloc(sizeof(data_t));
    d->parent = parent;
    d->vertex = vertex;
    return d;
}

void data_extract_path(data_t* d, vertex_list* res)
{
    if(d != NULL)
    {
	data_extract_path(d->parent, res);
	array_push_back(res, (void*)d->vertex);
    }
}


distance_list dijkstra(graph_t* graph, size_t from, gen_array* paths)
{
    size_t vCount = graph_vertices_count(graph);
    
    *paths = array_allocate(vCount);
    for(size_t i = 0; i < vCount; ++i)
    {
	gen_array* path = malloc(sizeof(gen_array));
	*path = array_create();
	array_push_back(paths, (void*)path);
    }

    distance_list distances = array_init(vCount);
    
    gen_array visite = array_allocate(vCount);
    
    gen_array datas = array_allocate(vCount);
    for(size_t i = 0; i < vCount; ++i)
    {
	array_push_back(&visite, (void*)false);
    }

    fibonacci_heap* toVisit = fibonacci_create();


    data_t* initData = data_create(from, NULL);
    array_push_back(&datas, initData);
    fibonacci_add(toVisit, 0, initData);
    
    while(!fibonacci_empty(toVisit))
    {
	void* dataGen;
	size_t minDist = fibonacci_extract_min(toVisit, &dataGen);
	data_t* data = dataGen;
	
	if(!(_Bool)array_get(&visite, data->vertex))
	{
	    data_extract_path(data, (gen_array*)array_get(paths, data->vertex));
	    array_set(&distances, data->vertex, (void*)minDist);
	    array_set(&visite, data->vertex, (void*)true);
	    
	    vertex_list const* adj = graph_adjacent_vertices(graph, data->vertex);
	    size_t nAdj = array_length(adj);
	    for(size_t i = 0; i < nAdj; ++i)
	    {
		edge_t* edge = (edge_t*)array_get(adj, i);

		/* This check may be omitted */
		if(!(_Bool)array_get(&visite, edge->vertex))
		{
		    data_t* chData = data_create(edge->vertex, data);
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

static void array_free_gen(void* a)
{
    array_free((gen_array*)a);
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
