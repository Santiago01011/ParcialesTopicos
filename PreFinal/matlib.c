#include "main.h"

#define SIZE_OF_ELEM 6

int createMat(tMat* mat, size_t tamElem, int cols, int rows){
    int row, j;
    mat->data = malloc(sizeof(void*) * rows); // Allocate for rows of pointers
    if ( !mat->data ) return 0;

    for ( row = 0; row < rows; row++ ){
        (mat->data)[row] = malloc(tamElem * cols); // Allocate for each row
        if ( (mat->data)[row] == NULL ) { //Free method in case of allocation failure
            for ( j = 0; j < row; j++ )
                free( mat->data[j] );
            free(mat->data);
            return 0;
        }
    }
    mat->cols = cols;
    mat->rows = rows;
    mat->tamElem = tamElem;
    return 1;
}


void printMat(tMat* mat, printStruct printM){
    int row, col;
    if (!mat || !mat->data) return;
    for( row = 0; row < mat->rows; row++ ){
        putchar('|');
        for( col = 0; col < mat->cols; col++ ){
            printM( (mat->data)[row] + (col * mat->tamElem) );
        }
        putchar('\n');
    }
}

void printMatInfTri(tMat* mat, printStruct printM){
    int row, col;
    float aux = 0.00;
    if (!mat || !mat->data) return;
    for( row = 0; row < mat->rows; row++ ){
        putchar('|');
        for( col = 0; col < mat->cols; col++ ){
            if( row > col && (row + col) >= mat->cols )
                printM( (mat->data)[row] + (col * mat->tamElem) );
            else
                printf("       ");
        }
        putchar('\n');
    }
}

void freeMat(tMat* mat){
    int row;
    if( !mat || !mat->data ) return;
    for( row = 0; row < mat->rows; row++ )
        free( mat->data[row] );
    free( mat->data );
}


void printMatInt(const void* elem){
    printf("%2d|", *(int*)elem);
}
void printMatFloat(const void* elem){
    printf("%6.2f|", *(float*)elem);
}

int loadMatFromArch(const char* filePath, tMat* mat){
    char* buffer, *ptrBffAux;
    int dim, row = 0, col = 0;
    float aux, **data;
    FILE* arch = fopen(filePath, "r");
    if(!arch) return 0;
    buffer = (char*)malloc(12);
    if(!buffer){
        fclose(arch);
        return 0;
    }
    fgets(buffer, 12, arch);
    sscanf(buffer, "%d", &dim);
    printf("%d\n", dim);
    buffer = realloc(buffer, (dim * SIZE_OF_ELEM));
    if(!buffer){
        fclose(arch);
        return 0;
    }
    if( !createMat(mat, sizeof(float), dim, dim) )
        return 0;
    data = (float**)mat->data;
    while( fgets(buffer, dim * SIZE_OF_ELEM, arch) && row < dim){
        col = 0;
        ptrBffAux = buffer;
        while( ptrBffAux != NULL && sscanf(ptrBffAux, "%f|", &aux) == 1){
            memcpy(&data[row][col], &aux, mat->tamElem);
            col ++;
            ptrBffAux = strchr(ptrBffAux, '|');
            if(ptrBffAux != NULL)
                ptrBffAux++;
        }

        row++;
    }

    free(buffer);
    fclose(arch);
    return 1;
}


