#include "dataLog.h"


#include <time.h>

#define MAX_ENTRIES 120000
#define FILE_NAME "Archs/large_test.txt"


int genArchs(){
    FILE *file = fopen(FILE_NAME, "w");
    if( !file ){
        perror("Error opening file");
        return 1;
    }

    srand(time(NULL)); // Seed random number generator
    generateRandomLog(file, MAX_ENTRIES);

    fclose(file);
    printf("File '%s' created with %d entries.\n", FILE_NAME, MAX_ENTRIES);
    return 0;
}

void generateRandomLog(FILE *file, int entryCount) {
    const char *severities[] = {"INFO", "WARN", "ERROR", "DEBUG", "FATAL"};
    const int severityCount = sizeof(severities) / sizeof(severities[0]);

    for (int i = 0; i < entryCount; i++) {
        int year = rand() % 24 + 2000; // 2000 - 2023
        int month = rand() % 12 + 1;
        int day = rand() % 28 + 1;
        int hour = rand() % 24;
        int minute = rand() % 60;
        int second = rand() % 60;
        const char *severity = severities[rand() % severityCount];

        fprintf(file, "%04d-%02d-%02d %02d:%02d:%02d|%s|This is log entry number %d\n",
                year, month, day, hour, minute, second, severity, i + 1);
    }
}


int chargeVectorFromFile(const char* filePath, tVector* vec){
    DataLine DataAux;
    char buffer[MAXLOGLENGTH];
    FILE* arch = fopen(filePath, "r");
    if(!arch){
        puts("Error opening the file");
        return 0;
    }
    while(fgets(buffer, sizeof(buffer), arch)){
        DataAux.msg = malloc(MAXLOGLENGTH);
        if( !(DataAux.msg) ){
            puts("Error allocating memory for the message");
            return 0;
        }
        if( sscanf(buffer, "%d-%d-%d %d:%d:%d|%7[^|]|%[^\n]", &DataAux.date.y, &DataAux.date.m, &DataAux.date.d,
            &DataAux.date.time.h, &DataAux.date.time.m, &DataAux.date.time.s, DataAux.severity, DataAux.msg) == 8)
            addToEnd(vec, &DataAux);
    }


    //free(DataAux.msg); !!!!!!!!!!!!!!
    fclose(arch);
    return 1;
}

void printDataLine(const void* data){
    DataLine DataAux = *(DataLine*)data;
    printf("%d-%d-%d %d:%d:%d|%s|%s\n", DataAux.date.y, DataAux.date.m, DataAux.date.d,
            DataAux.date.time.h, DataAux.date.time.m, DataAux.date.time.s, DataAux.severity, DataAux.msg);
}


void title(void){
    puts("_|_|_|                _|                _| \n_|    _|    _|_|_|  _|_|_|_|    _|_|_|  _|          _|_|      _|_|_| \n_|    _|  _|    _|    _|      _|    _|  _|        _|    _|  _|    _| \n_|    _|  _|    _|    _|      _|    _|  _|        _|    _|  _|    _| \n_|_|_|      _|_|_|      _|_|    _|_|_|  _|_|_|_|    _|_|      _|_|_| \n                                                                  _|\n_|_|_|_|                                                _|    _|_|_| \n_|        _|_|    _|  _|_|  _|_|_|  _|_|      _|_|_|  _|_|_|_|    _|_|    _|  _|_| \n_|_|_|  _|    _|  _|_|      _|    _|    _|  _|    _|    _|      _|_|_|_|  _|_| \n_|      _|    _|  _|        _|    _|    _|  _|    _|    _|      _|        _| \n_|        _|_|    _|        _|    _|    _|    _|_|_|      _|_|    _|_|_|  _|\n");
}
