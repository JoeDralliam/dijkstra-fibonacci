#include "ratp.h"

#include "osm_xml.h"

#include <stdlib.h>
#include <string.h>

char* read_line(FILE* file, char sep)
{
    long start = ftell(file);

    char c = fgetc(file);
    if(c == EOF)
    {
	return NULL;
    }
	
    while (c != sep && c != EOF)
    {
	c = fgetc(file);
    }
    
    long end = ftell(file);
    fseek(file, start, SEEK_SET); /* rewind */

    size_t len = end - start;

    char* str = malloc(sizeof(char) * (len + 1));    
    fgets(str, len, file);
    str[len] = '\0';
    fgetc(file); /* read separator */
    return str;
}

gen_array ratp_station_indexes(gen_array nodes, FILE* ratpfile, int masque)
{
    gen_array indexes = array_create();

    char* line;
    while((line = read_line(ratpfile, '\n')) != NULL)
    {
	char* cA = line;
	char* cB = strchr(cA, '#') + 1; *(cB - 1) = '\0';
	char* cC = strchr(cB, '#') + 1; *(cC - 1) = '\0';
	char* cD = strchr(cC, '#') + 1; *(cD - 1) = '\0';
	char* cE = strchr(cD, '#') + 1; *(cE - 1) = '\0';
	char* cF = strchr(cE, '#') + 1; *(cF - 1) = '\0';

	int code = 0;
	if(strcmp(cF, "metro") == 0)
	{
	    code = METRO;
	}
	else if(strcmp(cF, "bus") == 0)
	{
	    code = BUS;
	}
	else if(strcmp(cF, "tram") == 0)
	{
	    code = TRAM;
	}

	if(code & masque)
	{
	    double lon = atof(cB);
	    double lat = atof(cC);

	    
	    station_t* station = (station_t*)malloc(sizeof(station_t));
	    station->idx = find_nearest(nodes, lat, lon);
	    station->name = (char*)malloc(sizeof(char) * (strlen(cD) + 1));
	    strcpy(station->name, cD);

	    array_push_back(&indexes, (void*)station);
	}
	free(line);
    }
    return indexes;
       

}

void ratp_free_station_indexes(gen_array* idx)
{
    size_t len = array_length(idx);
    for(size_t i = 0; i < len; ++i)
    {
	station_t* station = (station_t*)array_get(idx, i);
	free(station->name);
	free(station);
    }
    array_free(idx);
}
