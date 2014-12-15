#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h> 
#include "random_matrix.h"

int main(int argc, char** argv)
{
    srand(time(NULL));

    assert(argc > 2);
    FILE* fin = fopen(argv[1], "w");
    FILE* fout = fopen(argv[2], "w");
    
    int n = 2 + rand() % 10; 
    int* taille = malloc((n+1) * sizeof(int));
    fprintf(fin, "%d\n", n);
    for(int i = 0; i < n + 1; ++i)
    {
	taille[i] = 1 + rand() % 20;
    }

    Matrix* res = createMatrix(taille[0], taille[1]);
    fillMatrix(res, 1, 2);
    printMatrix(res, fin);
    
    for(int i = 1; i <  n; ++i)
    {
	Matrix *matrix = createMatrix(taille[i], taille[i + 1]);
	fillMatrix(matrix, 1, 2);
	printMatrix(matrix, fin);

	Matrix* tmp = res;
	res = multiply(res, matrix);
	freeMatrix(tmp);
	freeMatrix(matrix);
    }

    printMatrix(res, fout);
    freeMatrix(res);
    return 0;
}

