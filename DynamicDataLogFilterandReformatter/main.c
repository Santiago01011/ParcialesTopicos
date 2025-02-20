#include "main.h"


int main()
{
    title();
    //genArchs();
    tVector logVector;
    logVector.data = NULL;
    if( !createVector(&logVector, sizeof(DataLine)) ){
        puts("Error allocating memory for the logVector");
        return MEM_ERROR;
    }
    chargeVectorFromFile("Archs/large_test.txt", &logVector);
    printVec(&logVector, printDataLine);
    return 0;
}
