#include "dijkstra.h"

#include <stdio.h>

void print_vertex(void* v)
{
    printf("-%lu-", (size_t)v);
}

int main(int argc, char** argv)
{
    graph_t graph;
    if(argc == 2)
    {
	FILE* file = fopen(argv[1], "r");
	graph = graph_read_from_file(file);
	fclose(file);
    }
    else
    {
	graph = graph_read_from_file(stdin);
    }

    size_t vCount = graph_vertices_count(&graph);
    
    for(size_t i = 0; i < vCount; ++i)
    {
	    gen_array paths = array_create();
	    distance_list dist = dijkstra(&graph, i, &paths);
	    for(size_t j = 0; j < vCount; ++j)
	    {
		if(i == j)
		{
		    continue;
		}
		size_t minDist = (size_t)array_get(&dist, j);
		printf("Minimal path of length %3lu from %lu to %lu found: ",
		       minDist, i, j);
		array_iter((gen_array*)array_get(&paths, j), print_vertex);
		printf("\n");
	    }
	    dijkstra_free_paths(&paths);
	    dijkstra_free_distances(&dist);
	    printf("\n");
    }
    graph_free(&graph);
}
