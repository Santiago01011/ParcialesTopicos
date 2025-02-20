#include "libvec.h"


int createVector(tVector* vec, size_t tamElem){
    if( (vec->data) != NULL ){
        puts("tried to overwrite an allocated vector");
        return 0;
    }
    vec->data = malloc( tamElem * MINVECTAM );
    vec->cantElem = 0;
    vec->tamElem = tamElem;
    vec->totalLength = MINVECTAM;
    return 1;
}

int addToBeginning(tVector* vec, void* elem){


    return 1;
}

int addToEnd(tVector* vec, void* elem){
    if( !(vec->data) ){
        puts("uninitialized vector");
        return 0;
    }
    if( vec->cantElem >= vec->totalLength ){
        void* aux = realloc( vec->data, vec->totalLength * 2 * vec->tamElem );
        if(!aux)
            exit(MEM_ERROR);
        vec->data = aux;
        vec->totalLength *= 2;
    }
    if( memcpy( (vec->data) + (vec->cantElem * vec->tamElem), elem, vec->tamElem ) == NULL)
        return 0;
    vec->cantElem ++;
    return 1;
}

void printVec(tVector* vec, printStruct printElemVec){
    int i;
    for(i = 0; i < vec->cantElem; i++)
        printElemVec( (vec->data) + (i * vec->tamElem) );
}

int freeVec(tVector* vec){
    int i;
    if( vec->data == NULL )
        return 0;
    for( i = 0; i < vec->cantElem; i++ ){
        DataLine* aux = (DataLine*)(char*)(vec->data) + (i * vec->tamElem);
        free( aux->msg );
    }
    free( vec->data );
    vec->cantElem = 0;
    vec->totalLength = 0;
    return 1;
}

