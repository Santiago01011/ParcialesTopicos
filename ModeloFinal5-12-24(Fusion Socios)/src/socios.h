#ifndef SOCIOS_H_INCLUDED
#define SOCIOS_H_INCLUDED
#include "main.h"
#define FILE_PATH_A "Archivos/socios_a.bin"
#define FILE_PATH_B "Archivos/socios_b.txt"
#define FILE_PATH_F "Archivos/socios_merge.txt"


typedef struct
{
    int d, m, y;
}t_date;

typedef struct
{
    long dni;
    char name[30];
    t_date inscription_date;
}t_socio;


int title();
void printSocio(const void* elem);
int readSocioFromTXT(FILE* arch, t_socio* dest);
int cmpSocio(const void* d1, const void* d2);
int cmpDate(const void* d1, const void* d2);
int mergeClubesAyB(const int option);
#endif // SOCIOS_H_INCLUDED
