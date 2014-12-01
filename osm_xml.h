#ifndef OSM_XML_H
#define OSM_XML_H

#include "graph.h"

size_t* find_index(gen_array nodes, long int refI);
graph_t parse_osm(char const* filename, gen_array* nodes);

#endif

