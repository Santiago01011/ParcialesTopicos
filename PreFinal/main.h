#ifndef MAIN_H_INCLUDED
#define MAIN_H_INCLUDED
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
typedef struct{
    void** data;
    size_t tamElem, cols, rows;
} tMat;

typedef void(*printStruct)(const void*);

/**< MENU */
int mainMenu(const char* msg);
int CreateFloatMat();
int CreateIntMat();

/**< MATLIB */
int createMat(tMat* mat, size_t tamElem, int cols, int rows);
void printMat(tMat* mat, printStruct printM);
void printMatInt(const void* elem);
void printMatFloat(const void* elem);
void freeMat(tMat* mat);
int loadMatFromArch(const char* filePath, tMat* mat);
void printMatInfTri(tMat* mat, printStruct printM);

#endif // MAIN_H_INCLUDED
