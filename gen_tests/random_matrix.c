#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "random_matrix.h"

Matrix* createMatrix(int h, int w)
{
    if (h < 1) {h = 1;}
    if (w < 1) {w = 1;}
    Matrix *matrix = malloc(sizeof(Matrix));
    if (matrix == NULL)
    {
        exit(out_of_memory_code);
    }
    matrix->h = h;
    matrix->w = w;
    matrix->cases = NULL;
    matrix->cases = malloc(h*w*sizeof(int));
    if (matrix->cases == NULL)
    {
        free(matrix);
        exit(out_of_memory_code);
    }
    return (matrix);
}

Matrix* multiply(Matrix* a, Matrix* b)
{
    assert(a->w == b->h);
    Matrix* res = createMatrix(a->h, b->w);
    for(int i = 0; i < res->h; i++)
    {
	for(int j = 0; j < res->w; j++)
	{
	    int sum = 0;
	    for(int k = 0; k < a->w; ++k)
	    {
		sum += *(a->cases + i * a->w + k) * *(b->cases + k * b->w + j);
	    }	    
	    *(res->cases + i*res->w + j) = sum;
	}
    }
    return res;
}



void fillMatrix(Matrix *matrix, int minint, int maxint)
{
    int ecart = maxint - minint +1;
    if (maxint<minint) {ecart = 1;}
    if (matrix->cases == NULL)
    {
        exit(matrix_not_allocated);
    }
    int i,j;
    for (i = 0; i<matrix->h; i++)
    {
        for (j = 0; j<matrix->w; j++)
        {
            *(matrix->cases + i*matrix->w +j) = (rand() % (ecart)) + minint;
        }
    }
}

void printMatrix(Matrix *matrix, FILE* f)
{
    int i,j;
    fprintf(f, "%d %d\n", matrix->h, matrix->w); 
    for (i = 0; i<matrix->h; i++)
    {
        for (j = 0; j < matrix->w-1; j++)
        {
            fprintf(f, "%d ", *(matrix->cases + i*matrix->w +j));
        }
        fprintf(f, "%d\n", *(matrix->cases + i*matrix->w + matrix->w-1));
    }
}

void freeMatrix(Matrix *matrix)
{
    free(matrix->cases);
    free(matrix);
}

