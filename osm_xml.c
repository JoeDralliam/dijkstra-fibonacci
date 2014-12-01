#include "osm_xml.h"

#include <stdbool.h>
#include <assert.h>
#include <string.h>
#include <math.h>

#include <libxml/xmlreader.h>

#include "gen_array.h"
#include "graph.h"

typedef struct osm_node_t
{
    long int id;
    double lat;
    double lon;
} osm_node_t;


size_t* find_index(gen_array nodes, long int refI)
{
    size_t l = array_length(&nodes);
    for(size_t i = 0; i < l; ++i)
    {
	osm_node_t* osm_nd = (osm_node_t*)array_get(&nodes, i);
	if(osm_nd->id == refI)
	{
	    size_t* res = malloc(sizeof(size_t));
	    *res = i;
	    return res;
	}
    }
    return NULL;
}

_Bool node_name_is(xmlTextReaderPtr reader, xmlChar const* expect)
{
    xmlChar* name;
    
    name = xmlTextReaderName(reader);
    if(name == NULL)
    {
	return false;
    }
    _Bool res = xmlStrEqual(name, expect);
    xmlFree(name);
    return res;
}


void process_osm_node(xmlTextReaderPtr reader, gen_array* nodes)
{    
    if(node_name_is(reader, (xmlChar const*)"node")
       /* && !xmlTextReaderIsEmptyElement(reader) */ )	
    {
	xmlChar* lat = xmlTextReaderGetAttribute(reader, (xmlChar const*)"lat");
	xmlChar* lon = xmlTextReaderGetAttribute(reader, (xmlChar const*)"lon");
	xmlChar* id = xmlTextReaderGetAttribute(reader, (xmlChar *)"id");
	assert(lat != NULL && lon != NULL && id != NULL);

	osm_node_t* nd = malloc(sizeof(osm_node_t));
	nd->id = atol((char*)id);
	nd->lat = atof((char*)lat);
	nd->lon = atof((char*)lon);
	array_push_back(nodes, nd);

	xmlFree(lat);
	xmlFree(lon);
	xmlFree(id);
    }
  
}

gen_array read_osm_nodes(char const* filename)
{
    gen_array nodes = array_create();
    xmlTextReaderPtr reader;
    
    reader = xmlNewTextReaderFilename(filename);
    if (reader != NULL)
    {
	int ret;
	while ((ret = xmlTextReaderRead(reader)) == 1)
	{    
            process_osm_node(reader, &nodes);
        }
        xmlFreeTextReader(reader);
        if (ret != 0)
	{
            printf("%s : failed to parse\n", filename);
        }
    }
    else
    {
        printf("Unable to open %s\n", filename);
    }

    return nodes;
}

void process_osm_way(xmlTextReaderPtr reader, gen_array nodes, graph_t* graph)
{
    if(node_name_is(reader, (xmlChar*)"way"))
    {
	int depth = xmlTextReaderDepth(reader);
	gen_array nds = array_create();
	_Bool isHighway = false;
	xmlChar* name = NULL;
	
	int ret;
	while((ret = xmlTextReaderRead(reader)) == 1)
	{
	    if(depth == xmlTextReaderDepth(reader))
	    {
		break;
	    }
	    if(node_name_is(reader, (xmlChar*)"tag"))
	    {
		xmlChar* k = xmlTextReaderGetAttribute(reader, (xmlChar*)"k");
		if(k != NULL)
		{
		    if(xmlStrEqual(k, (xmlChar*)"highway"))
		    {
			isHighway = true;
		    }
		    else if(xmlStrEqual(k, (xmlChar*)"name"))
		    {
			name = xmlTextReaderGetAttribute(reader, (xmlChar*)"v");
		    }

		    xmlFree(k);
		}		
	    }
	    else if(node_name_is(reader, (xmlChar*)"nd"))
	    {
		xmlChar* ref = xmlTextReaderGetAttribute(reader, (xmlChar*)"ref");
		long int refI = atol((char*)ref);
		size_t* nodeIdx = find_index(nodes, refI);
		if(nodeIdx != NULL)
		{
		    array_push_back(&nds, (void*)nodeIdx);
		}
		xmlFree(ref);
	    }
	}

	assert(ret == 1);

	if(isHighway)
	{
	    size_t ndsLen = array_length(&nds);
	    for(size_t i = 0; i + 1 < ndsLen; ++i)
	    {
		size_t idxFst = *(size_t*)array_get(&nds, i    );
		size_t idxSnd = *(size_t*)array_get(&nds, i + 1);
		
		osm_node_t* first  = (osm_node_t*)array_get(&nodes, idxFst);
		osm_node_t* second = (osm_node_t*)array_get(&nodes, idxSnd);

		double dlat = 111.32 * 1000. * (first->lat - second->lat);
		double dlon =  78.71 * 1000. * (first->lon - second->lon);
		
		size_t dist = (size_t)sqrt(dlat * dlat + dlon * dlon);
	        graph_add_edge(graph, idxFst, idxSnd, dist, xmlStrdup(name));
	    }
	}

	xmlFree(name);
	array_iter(&nds, free);
	array_free(&nds);
    }    
}
graph_t read_osm_ways(char const* filename, gen_array nodes)
{
    graph_t graph = graph_create(array_length(&nodes));
    xmlTextReaderPtr reader;
    
    reader = xmlNewTextReaderFilename(filename);
    if (reader != NULL)
    {
	int ret;
	while ((ret = xmlTextReaderRead(reader)) == 1)
	{    
            process_osm_way(reader, nodes, &graph);
        }
        xmlFreeTextReader(reader);
        if (ret != 0)
	{
            printf("%s : failed to parse\n", filename);
        }
    }
    else
    {
        printf("Unable to open %s\n", filename);
    }

    return graph;
}

graph_t parse_osm(char const* filename, gen_array* nodes)
{
    *nodes = read_osm_nodes(filename);
    return read_osm_ways(filename, *nodes);
}

/*
  struct string_t
  {
  char* str;

  Length of the string, does not count the final \0
  int len;
  };

  struct token_t
  {
  enum {
  TOK_STRING,
  TOK_IDENT,
  TOK_OPEN_MARKUP,
  TOK_CLOSE_MARKUP,
  TOK_CLOSE_INL_MARKUP,
  TOK_EQ,
  TOK_EOF
  } type;

  union {
  string_t str_val;
  int int_val;
  } value;
	
  };
    

  string_t lex_string(FILE* file)
  {
  long start = ftell(file);
  while(fgetc(file) != '"') {}
  long end = ftell(file);
  fseek(file, start, SEEK_SET);

  string_t res = {
  .len = end - start - 1,
  .str = malloc(sizeof(char) * (end - start))
  } ;
  fgets(res.str, res.len, file);
  res.str[res.len] = '\0';

  eats the "
  fgetc();
    
  return res;
  }

  string_t lex_ident(FILE* file, char c)
  {
  long start = ftell(file);
  while(int ch = fgetc(c))
  {
  if(!(isalpha(c) || isdigit(c)))
  {
  break;
  }
  }
  }

  token_t next_token(FILE* file)
  {
  int c = fgetc(file);
  while(c == '' || c == '\n' || c == '\t') {}

  token_t res;
  if(c == '"')
  {
  res.type = TOK_STRING;
  res.value.str_value = c;
  }
  else if(isalpha(c))
  {
  res.type = TOK
  }
  }

*/
