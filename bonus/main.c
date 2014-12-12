
#include "dijkstra.h"
#include "ratp.h"
#include "osm_xml.h"

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include <libxml/xmlreader.h>

char* get_name(gen_array const* path, size_t idx)
{
    edge_t* e  = (edge_t*)array_get(path, idx);
    char* data = (char*)e->data;
    return data;
}


int main(int argc, char** argv)
{
    /* 45, rue d'Ulm */
    double srcLat = 48.8422951, srcLon = 2.3442717;

    /* Croisement de l'avenue Jean Moulin et du boulevard Brune */
    double dstLat = 48.82462  , dstLon = 2.31945  ;
    _Bool stationSearch = false;
    int stationKind = 0;
    char* regionFilename = "paris.osm";
    FILE* stations = "ratp.csv";
    size_t radius = 0;
    
    for(int i = 1; i < argc; ++i)
    {
	if(strcmp(argv[i], "--source") == 0)
	{
	    assert(i + 2 < argc);
	    ++i;
	    srcLat = atof(argv[i]);
	    ++i;
	    srcLon = atof(argv[i]);
	}
	else if(strcmp(argv[i], "--destination") == 0)
	{
	    assert(i + 2 < argc);
	    ++i;
	    dstLat = atof(argv[i]);
	    ++i;
	    dstLon = atof(argv[i]);
	}
	else if(strcmp(argv[i], "--bus") == 0)
	{
	    stationKind |= BUS; 
	}
	else if(strcmp(argv[i], "--metro") == 0)
	{
	    stationKind |= METRO;
	}
	else if(strcmp(argv[i], "--tram") == 0)
	{
	    stationKind |= TRAM;
	}
	else if(strcmp(argv[i], "--nearest-station") == 0)
	{
	    stationSearch = true;
	}
	else if(strcmp(argv[i], "--shortest-path") == 0)
	{
	    stationSearch = false;
	}
	else if(strcmp(argv[i], "--region-file") == 0)
	{
	    assert(i + 1 < argc);
	    ++i;
	    regionFilename = argv[i];
	}
	else if(strcmp(argv[i], "--stations-file") == 0)
	{
	    assert(i + 1 < argc);
	    ++i;
	    stations = fopen(argv[i], "r");
	}
	else if(strcmp(argv[i], "--nearer-than") == 0)
	{
	    assert(i + 1 < argc);
	    ++i;
	    radius = atoi(argv[i]);
	}
    }

    graph_t graph;
    gen_array nodes;
		
    graph = parse_osm(regionFilename, &nodes);

    size_t i = find_nearest(nodes, srcLat, srcLon);
    
    gen_array paths = array_create();
    distance_list dist = dijkstra(&graph, i, &paths);
	
    
    if(stationSearch)
    {
	gen_array ratpIdxs = ratp_station_indexes(nodes, stations, stationKind);
	size_t idxCnt = array_length(&ratpIdxs);
	for(size_t k = 0; k < idxCnt; ++k)
	{
	    station_t* s = (station_t*)array_get(&ratpIdxs, k);
	    size_t j = s->idx;
	    size_t d = (size_t)array_get(&dist, j);
	    gen_array* path = array_get(&paths, j);
	    if((radius == 0 || d <= radius) && path)
	    {
		printf("%s : %lum\n", s->name, d);

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
	}
	ratp_free_station_indexes(&ratpIdxs);
	
    }
    else
    {
	size_t j = find_nearest(nodes, dstLat, dstLon);
	
	gen_array* path = array_get(&paths, j);
	size_t minDist = (size_t)array_get(&dist, j);
	if(path)
	{
	    printf("Chemin de %5lum trouvé:\n", minDist);
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
	else
	{
	    printf("Erreur: chemin introuvable !\n");
	}
    }

    dijkstra_free_paths(&paths);	
    dijkstra_free_distances(&dist);

    array_iter(&nodes, free);
    array_free(&nodes);
       
    graph_free(&graph, xmlFree);
}
