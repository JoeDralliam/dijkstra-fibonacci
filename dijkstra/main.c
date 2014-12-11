#include "dijkstra.h"
#include "graph.h"

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void print_edge(void* v)
{
    edge_t* e = (edge_t*)v;
    printf(" %lu", e->vertex);    
}

typedef distance_list (*dijkstra_t)(graph_t*, size_t, gen_array*);

int main(int argc, char** argv)
{
    FILE* graph_file = NULL;
    FILE* stats_file = NULL;
    dijkstra_t dijkstra_fun = &dijkstra;
    _Bool connexity_test = false;
    _Bool silent = false;
    size_t source = 0;
    for(int i = 1; i  < argc; ++i)
    {
	if(strcmp(argv[i], "--in-file") == 0)
	{
	    assert(i + 1 < argc); /* --in-file requires an argument */
	    assert(graph_file == NULL); /* --in-file should only be specified once */
	    ++i;
	    graph_file = fopen(argv[i], "r");
	}
	else if(strcmp(argv[i], "--stat-file") == 0)
	{
	    assert(i + 1 < argc);
	    assert(stats_file == NULL);
	    ++i;
	    stats_file = fopen(argv[i], "a");
	}
	else if(strcmp(argv[i], "--use-fibonacci-heap") == 0)
	{
	    dijkstra_fun = &dijkstra;
	}
	else if(strcmp(argv[i], "--use-array") == 0)
	{
	    dijkstra_fun = &dijkstra_quadratic;
	}
	else if(strcmp(argv[i], "--source") == 0)
	{
	    assert(i + 1 < argc);
	    ++i;
	    source = atoi(argv[i]);
	}
	else if(strcmp(argv[i], "--connexity-test") == 0)
	{
	    connexity_test = true;
	}
	else if(strcmp(argv[i], "--silent") == 0)
	{
	    silent = true;
	}
    }

    size_t nEdges;
    graph_t graph = graph_read_from_file(graph_file ? graph_file : stdin, &nEdges);

    if(graph_file)
    {
	fclose(graph_file);
    }

    if(connexity_test && !graph_connexe(&graph))
    {
	printf("ERROR: Non connexe graph and connexe graph required...\n");
	printf("\t\t (enforced by --connexity-test)");
	return 0;
    }
    

    clock_t start_time = clock();
    
    gen_array paths;
    distance_list dist = dijkstra_fun(&graph, source, &paths);

    clock_t end_time = clock();


    size_t vCount = graph_vertices_count(&graph);
    
    if(!silent)
    {
	for(size_t j = 0; j < vCount; ++j)
	{
	    if(source == j)
	    {
		continue;
	    }
	    
	    gen_array* path = (gen_array*)array_get(&paths, j);
	    if(path)
	    {
		size_t minDist = (size_t)array_get(&dist, j);
		printf("%lu", source);
		array_iter((gen_array*)array_get(&paths, j), print_edge);
		printf(" %lu\n", minDist);
	    }
	    else
	    {
		printf("infini\n");
	    }
	}
    }
    
    dijkstra_free_paths(&paths);
    dijkstra_free_distances(&dist);
    
    
    if(stats_file)
    {
	fprintf(stats_file,
		"%lu %lu %f\n",
		vCount, nEdges,
		(double)(end_time - start_time) / CLOCKS_PER_SEC);
    }
    
/*    graph_free(&graph, NULL); */
}
