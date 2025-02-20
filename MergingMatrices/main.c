#include "main.h"

int main()
{
    int dim;
    float *mat1 = NULL, *mat2 = NULL;

    if (!readMatFromFile(FILE_PATH_MAT1, &mat1, &dim)) {
        puts("Error loading matrix 1. Exiting...");
        return 1;
    }
    if (!readMatFromFile(FILE_PATH_MAT2, &mat2, &dim)) {
        puts("Error loading matrix 2. Cleaning up...");
        free(mat1);
        return 1;
    }

    puts("-----Matrix 1-----");
    printMat(&mat1, dim);
    puts("-----Matrix 2-----");
    printMat(&mat2, dim);

    if (mergeMatrices(&mat1, &mat2, dim)) {
        puts("Merged matrix written to file successfully.");
    } else {
        puts("Error merging matrices.");
    }

    free(mat1);
    free(mat2);
    return 0;
}
