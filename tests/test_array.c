#include "gen_array.h"

#include <assert.h>

int main()
{
    gen_array a = array_create();

    array_push_back(&a, NULL);
    assert(array_length(&a) == 1);

    array_free(&a);
}
