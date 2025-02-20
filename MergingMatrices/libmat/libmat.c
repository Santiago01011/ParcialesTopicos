#include "libmat.h"


int readMatFromFile(const char* filePath, float** mat, int* dim){
    char buffer[MAX_BUFFER];
    char* pbff;
    int col = 0, row = 0;
    FILE* arch = fopen(filePath, "r");
    if(!arch){
        puts("Error opening file");
        return 0;
    }
    if( fgets(buffer, MAX_BUFFER, arch) ){
        *dim = atoi(buffer);
        printf("%s dim: %d\n", filePath, *dim);
    }
    else{
        puts("Error reading the dimentios");
        return 0;
    }
    *mat = (float*)malloc( (*dim) * (*dim) * sizeof(float) );
    if ( !(*mat) ){
        puts("Memory allocation failed");
        fclose(arch);
        return 0;
    }
    while( fgets(buffer, MAX_BUFFER, arch) ){
        pbff = buffer;
        col = *dim - 1;
        while( (pbff = strrchr(buffer,'|')) ) {
            pbff++;
            (*mat)[row * (*dim) + col] = atof(pbff);
            pbff--;
            *pbff = '\0';
            col--;
        }
        (*mat)[row * (*dim) + col] = atof(buffer);
        row++;
    }
    fclose(arch);
    return 1;
}

int printMat(float** mat, int dim){
    int row, col;
    if( !(*mat) )
        return 0;
    for( row = 0; row < dim; row++ ){
        for( col = 0; col < dim; col++ ){
            printf("%.2f", (*mat)[row * dim + col]);
            if( col + 1 < dim)
                putchar('|');
        }
        putchar('\n');
    }
    return 1;
}

int mergeMatrices(float** mat1, float** mat2, int dim){
    int row, col, auxCol;
    float diagSum;
    FILE* arch = fopen(FILE_PATH_MERGED, "w");
    if(!arch){
        puts("Error creating merge file");
        return 0;
    }
    for( row = 0; row < dim; row++ ){
        for( col = 0; col < dim; col++ ){
            if( col == row ){
                diagSum = 0;
                for( auxCol = 0; auxCol < dim; auxCol++ ){
                    diagSum += (*mat2)[row * dim + auxCol] + (*mat1)[row * dim + auxCol];
                }
                fprintf(arch, "%.2f", diagSum   );
            }
            if( col > row ){
                fprintf(arch, "%.2f", (*mat2)[row * dim + col] );
            }
            if( col < row ){
                fprintf(arch, "%.2f", (*mat1)[row * dim + col] );
            }
            if(col + 1 < dim)
                fprintf(arch, "|");
        }
            fprintf(arch, "\n");
    }
    fclose(arch);
    return 1;
}
