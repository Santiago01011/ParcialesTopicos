#include "../main.h"
#ifndef DATALOG_H_INCLUDED
#define DATALOG_H_INCLUDED
#define SEVERITYLENGTH 8
#define MAXLOGLENGTH 256

typedef struct {
    int h,m,s;
} tTime;

typedef struct {
    int d,m,y;
    tTime time;
} tDate;

typedef struct{
    tDate date;
    char severity[SEVERITYLENGTH];
    char *msg;
}DataLine;

int genArchs();
void generateRandomLog(FILE *file, int entryCount);
void printDataLine(const void* data);
void title(void);
int chargeVectorFromFile(const char* filePath, tVector* vec);
#endif // DATALOG_H_INCLUDED
