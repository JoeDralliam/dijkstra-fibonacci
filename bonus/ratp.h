#ifndef RATP_H
#define RATP_H

#include "gen_array.h"

#include <stdio.h>

enum station_kind_t {
    BUS = 1,
    METRO = 1 << 1,
    TRAM = 1 << 2
};

typedef struct {
    size_t idx;
    char* name;
} station_t;


gen_array ratp_station_indexes(gen_array nodes, FILE* ratpfile, int masque);
void ratp_free_station_indexes(gen_array* idx);

#endif
