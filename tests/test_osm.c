#include "dijkstra.h"
#include "osm_xml.h"

#include <stdio.h>
#include <string.h>
#include <libxml/xmlreader.h>

char* get_name(gen_array* path, size_t idx)
{
    edge_t* e  = (edge_t*)array_get(path, idx);
    char* data = (char*)e->data;
    return data;
}


int main(int argc, char** argv)
{
    graph_t graph;
    gen_array nodes;
    if(argc == 2)
    {
	graph = parse_osm(argv[1], &nodes);
    }
    else
    {
	graph = graph_read_from_file(stdin);
    }

    size_t* iPtr = find_index(nodes, 15641131);
    size_t i = *iPtr;
    size_t* jPtr = find_index(nodes, 30560224);
    size_t j = *jPtr;
    free(iPtr);
    free(jPtr);
    
    gen_array paths = array_create();
    distance_list dist = dijkstra(&graph, i, &paths);
    size_t minDist = (size_t)array_get(&dist, j);
    if(minDist != 0)
    {
	printf("Minimal path of length %5lu from %lu to %lu found:\n",
	       minDist, i, j);
	gen_array* path = array_get(&paths, j);
	size_t path_len = array_length(path);
	for(size_t k = 0; k < path_len;)
	{
	    size_t d = 0;
	    char* name = get_name(path, k);
	    while(k < path_len && strcmp(name, get_name(path, k)) == 0)
	    {
		edge_t* e = (edge_t*)array_get(path, k);
		d += e->weight;
		++k;
	    }
	    printf("\tPrendre %s pendant %lu metres.\n", name, d);
    
	}
	printf("\n");
    }

    dijkstra_free_paths(&paths);
    dijkstra_free_distances(&dist);
    printf("\n");

    array_iter(&nodes, free);
    array_free(&nodes);
    
    graph_free(&graph, xmlFree);
}
