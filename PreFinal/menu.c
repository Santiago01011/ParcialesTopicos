#include "main.h"


int mainMenu(const char* msg){
    char opt;
    do{
        puts(msg);
        fflush(stdin);
        opt = getchar();
        opt = toupper(opt);
        switch(opt){
            case 'A':
                CreateIntMat();
                break;
            case 'B':
                CreateFloatMat();
                break;
            case 'F':
                break;
            default:
                puts("Unsupported action, select one of the list");
                break;
        }

    }while( opt != 'F' );
    return 1;
}


int CreateIntMat(){
    tMat mat;
    int rows = 5, cols = 5;
    int* intData;
    if( !createMat(&mat, sizeof(int), rows, cols) )
        return 5;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            intData = ((int**)mat.data)[i];
            intData[j] = i * cols + j; // Example data
        }
    }
    puts("Integer Matrix:");
    printMat(&mat, printMatInt);
    freeMat(&mat);
    return 1;
}

int CreateFloatMat(){
    tMat mat;
    int rows = 5, cols = 5;
    float* floatData;
    if( !createMat(&mat, sizeof(float), rows, cols) )
        return 5;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            floatData = ((float**)mat.data)[i];
            floatData[j] = i * cols + j; // Example data
        }
    }
    puts("Integer Matrix:");
    printMat(&mat, printMatFloat);
    freeMat(&mat);
    return 1;
}
