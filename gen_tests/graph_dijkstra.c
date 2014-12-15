#include "graph.h"

#include <assert.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#include <sys/time.h>
#include <unistd.h>

#define MAX(a,b) (((a) < (b)) ? (b) : (a))

/* Semi-open range [min; max[ */
size_t rand_int(size_t min, size_t max)
{
    return min + rand() % (max - min);
}


size_t somme_distance(gen_array* a)
{
    size_t l = array_length(a);
    size_t res = 0;
    for(size_t i = 0; i < l; ++i)
    {
	edge_t* e = (edge_t*)array_get(a, i);
	res += e->weight;
    }
    return res;
}

gen_array generate_graph(size_t nVertex, size_t nEdge, size_t maxWeight, graph_t* g)
{
    *g = graph_create(nVertex);
    gen_array p = array_init(nVertex);

    array_set(&p, 0, malloc(sizeof(gen_array)));
    for(size_t i = 1; i < nVertex; ++i)
    {
	if(i < nEdge)
	{
	    size_t origin = rand_int(0, i);
	    size_t weight = rand_int(1, maxWeight);
	    assert(weight > 0);
	    graph_add_edge(g, origin, i, weight, NULL);
	    
	    gen_array* path = malloc(sizeof(gen_array));
	    *path = array_copy((gen_array*)array_get(&p, origin));
	    edge_t* e = array_back(graph_adjacent_vertices(g, origin));
	    array_push_back(path, e);
	    

	    array_set(&p, i, path);
	}
    }

    for(size_t j = nVertex; j < nEdge; ++j)
    {
	size_t a = rand_int(0, nVertex);
	size_t b = (a + rand_int(1, nVertex)) % nVertex;

	if(graph_adjacent(g, a, b))
	{
	    j--;
	    continue;	       
	}
	
	size_t dA = somme_distance((gen_array*)array_get(&p, a));
	size_t dB = somme_distance((gen_array*)array_get(&p, b));

	size_t min = 2 + (dA < dB ? dB - dA : dA - dB);
	size_t w = rand_int(min, MAX(2 * min, maxWeight));

	graph_add_edge(g, a, b, w, NULL);
    }

    return p;    
}


void print_graph(FILE* f, graph_t* g, size_t nV, size_t nE)
{
    fprintf(f, "%lu %lu\n", nV, nE);
    for(size_t i = 0; i < nV; ++i)
    {
	gen_array const* adj = graph_adjacent_vertices(g, i);
	size_t nAdj = array_length(adj);
	for(size_t j = 0; j  < nAdj; ++j)
	{
	    edge_t* e = (edge_t*)array_get(adj, j);
	    if(i < e->vertex)
	    {
		fprintf(f, "%lu %lu %lu\n", i, e->vertex, e->weight);
	    }
	}
    }
}

void print_result(FILE* f, gen_array* p)
{
    size_t nV = array_length(p);
    for(size_t i = 1; i < nV; ++i)
    {
	gen_array* path = (gen_array*)array_get(p, i);
	if(path == NULL)
	{
	    fprintf(f, "infini\n");
	}
	else
	{
	    fprintf(f, "0");
	    size_t l = array_length(path);
	    for(size_t j = 0; j < l; ++j)
	    {
		edge_t* e = array_get(path, j);
		fprintf(f, " %lu", e->vertex);
	    }
	    fprintf(f, " %lu\n", somme_distance(path));
	}
    }
}

static void free_array_g(void* a)
{
    if(a != NULL)
    {
	array_free((gen_array*)a);
    }
}

int main(int argc, char** argv)
{
    assert(argc == 3);

    
    struct timeval time; 
    gettimeofday(&time,NULL);


    srand(time.tv_sec * 1000L * 1000L + time.tv_usec);
    
    size_t nVertex = rand_int(100, 1000 * 1000);
    size_t nEdge   = rand_int(10,
			      10 * 1000);
    graph_t g;
    gen_array p = generate_graph(nVertex, nEdge, 1000, &g);

    FILE* fin = fopen(argv[1], "w");
    print_graph(fin, &g, nVertex, nEdge);
    fclose(fin);

    FILE* fout = fopen(argv[2], "w");
    print_result(fout, &p);
    fclose(fout);
    
    array_iter(&p, free_array_g);
    array_iter(&p, free);
    array_free(&p);
    
    graph_free(&g, NULL);
/*    struct timespec t, t2;
    t.tv_sec = 0;
    t.tv_nsec = 10L * 1000L * 1000L;
    nanosleep(&t, &t2);
*/
}
