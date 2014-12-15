#ifndef RANDOM_MATRIX_H_INCLUDED
#define RANDOM_MATRIX_H_INCLUDED

static const int out_of_memory_code = 2;
static const int matrix_not_allocated = 3;

typedef struct Matrix
{
    int h;
    int w;
    int *cases;
} Matrix ;

Matrix* createMatrix(int h, int w);

Matrix* multiply(Matrix* a, Matrix* b);
void fillMatrix(Matrix *matrix, int minint, int maxint);
void printMatrix(Matrix *matrix, FILE*);
void freeMatrix(Matrix *matrix);

#endif // RANDOM_MATRIX_H_INCLUDED
