#include "../main.h"
#ifndef LIBVEC_H_INCLUDED
#define LIBVEC_H_INCLUDED



#define MINVECTAM 20

typedef struct {
    void* data;
    size_t tamElem;
    size_t cantElem;
    size_t totalLength;
} tVector;


typedef void (*printStruct)(const void*);

int createVector(tVector* vec, size_t tamElem);
int addToBeginning(tVector* vec, void* elem);
int addToEnd(tVector* vec, void* elem);
void printVec(tVector* vec, printStruct printElemVec);
int freeVec(tVector* vec);

#endif // LIBVEC_H_INCLUDED
