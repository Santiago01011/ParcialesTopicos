#include "../main.h"
#ifndef LIBMAT_H_INCLUDED
#define LIBMAT_H_INCLUDED

#include <ctype.h>

#define MAX_BUFFER 100



int readMatFromFile(const char* filePath, float** mat, int* dim);
int printMat(float** mat, int dim);

#endif // LIBMAT_H_INCLUDED
