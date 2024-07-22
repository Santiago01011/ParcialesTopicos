#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>


#define TODO_OK 0

typedef struct{
    char timeStamp[13];
    char Dom[8];
    float kmRec;
    float litrosUso;
    char estado[11];
}Telemetria;

typedef struct{
    void* vec;
    int ce;
    size_t cap;
    size_t tamElem;
}Vector;

typedef struct{
    char Dom[8];
    float consumo;
}Informe;

typedef int (*Cmp)(const void*, const void*);
typedef void (*Actualizar)(void*, const void*);
typedef void (*print_callback)(void*);
void generarArchivos();
int generarInforme(const char* nombreArchTel, const char* nombreArchInf);

int crearVector(Vector* vec, size_t tamElem);
int destruirVector(Vector* vec);
int InsertarEnTdaOrdenado(Vector* vec, void* elem, Cmp comparar, Actualizar Actualizar);
int vectorInsertarAlFinal_ALU(Vector* vector, void* elem);
bool buscarEnVectorOrd_ALU(const Vector* v, void* elem, Cmp cmp, int* pos);
int vectorInsertarEnPos_ALU(Vector* v, void* elem, int pos);
int formatearLinea(FILE* arch, Telemetria* elem);


void actuInforme(void* actualizado, const void* actualizador){
    Informe* inf1 = (Informe*)actualizado;
    Informe* inf2 = (Informe*)actualizador;
    inf1->consumo += inf2->consumo;
}
int cmpDom(const void* e1, const void* e2){
    Informe* inf1 = (Informe*)e1;
    Informe* inf2 = (Informe*)e2;
    return strcmp(inf1->Dom, inf2->Dom);
}
void printTelemetria(void* elem){
    Telemetria* a = (Telemetria*)elem;
    printf("Telemetria: %s\t Dom: %s\t Km: %.2f\t Litros: %.2f\t Estado: %s\n", a->timeStamp, a->Dom, a->kmRec, a->litrosUso, a->estado);
}
void printInforme(void* elem){
    Informe* a = (Informe*)elem;
    printf("Dom: %s\t consumo: %.2f\n",a->Dom, a->consumo);
}

int main(){
    int res;
    generarArchivos();
    res=generarInforme("telemetria.txt", "informe.txt"); if(res!=TODO_OK) return -1;
    return TODO_OK;
}

int generarInforme(const char* nombreArchTel, const char* nombreArchInf){
    int res, pos;
    Telemetria auxTelem;
    Informe auxInf;
    FILE* archT = fopen("telemetria.txt", "r");
    if(!archT){
        puts("Error al crear el archivo telemetria.txt");
        return -11;
    }
    Vector vecInf;
    res=crearVector(&vecInf, sizeof(Informe)); if(res!= TODO_OK) return -1;
    while(!formatearLinea(archT, &auxTelem)){
        strcpy(auxInf.Dom, auxTelem.Dom);
        buscarEnVectorOrd_ALU(&vecInf, auxInf.Dom, cmpDom, &pos);
        printTelemetria(&auxTelem);
        if(pos==-1){
            if(!strcmp(auxTelem.estado, "VALID"))
                auxInf.consumo=auxTelem.kmRec/auxTelem.litrosUso;
            if(!strcmp(auxTelem.estado, "UNRELIABLE")||!strcmp(auxTelem.estado, "NO VALID"))
                auxInf.consumo=0;
            InsertarEnTdaOrdenado(&vecInf, &auxInf, cmpDom, actuInforme);
        }
        if(pos!=-1&&!strcmp(auxTelem.estado, "VALID")){
            auxInf.consumo=auxTelem.kmRec/auxTelem.litrosUso;
            actuInforme((vecInf.vec)+(pos*vecInf.tamElem), &auxInf);
            //printInforme(vecInf.vec + pos*vecInf.tamElem);
        }
    }fclose(archT);
    FILE* archI = fopen(nombreArchInf, "wt");
    if(!archI){
        destruirVector(&vecInf);
        printf("Error al abrir el archivo %s", nombreArchInf);
        return -12;
    }
    for(int i=0; i<vecInf.ce; i++){
        Informe* aux = (vecInf.vec)+(i*vecInf.tamElem);
        if(aux->consumo == 0)
            fprintf(archI, "%s\t-Reportes no validos, requiere inspeccion manual-\n", aux->Dom);
        else
            fprintf(archI, "%s\t%.2f\n", aux->Dom, aux->consumo);
    }


    fclose(archI);
    destruirVector(&vecInf);
    return res;
}



void generarArchivos(){
    FILE* archT = fopen("telemetria.txt", "w");
    if(!archT){
        puts("Error al crear el archivo telemetria.txt");
        return;
    }
    Telemetria vecTelemetria[] = {
        {"1719077360", " AAA981", 120.00, 150.00, "NO VALID"},
        {"1719077360", " AAA981", 100.00, 100.00, "VALID"},
        {"1719077367", "AB691DG", 100.00, 100.00, "VALID"},
        {"1719077371", " ACA981", 100.00, 100.00, "NO VALID"},
        {"1719077360", " BBB755", 100.00, 100.00, "UNRELIABLE"},
        {"1719077360", " AAA981", 200.00, 150.00, "UNRELIABLE"},
        {"1719077367", "AB691DG", 200.00, 100.00, "VALID"},
        {"1719077367", "AB691DG", 200.00, 100.00, "NO VALID"},
        {"1719077371", " AAC981", 300.00, 120.00, "VALID"},
        {"1719077392", "AZ999RR", 200.00, 150.00, "VALID"},
    };
    for(int i=0; i<10; i++){
        fprintf(archT, "%12s%7s%.2f%.2f%10s\n", vecTelemetria[i].timeStamp, vecTelemetria[i].Dom, vecTelemetria[i].kmRec, vecTelemetria[i].litrosUso, vecTelemetria[i].estado);
    }
    fclose(archT);
}

int crearVector(Vector* vec, size_t tamElem){
    vec->vec = malloc(tamElem * 50);
    if(vec->vec){
        vec->ce=0;
        vec->cap=50;
        vec->tamElem=tamElem;
        return TODO_OK;
    }
    return -1;
}

int destruirVector(Vector* vec){
    if(!vec->vec)
        return -21;
    free(vec->vec);
    return TODO_OK;
}

int vectorInsertarAlFinal_ALU(Vector* vector, void* elem) {
    if (vector->ce == vector->cap) {
        vector->cap *= 2;
        vector->vec = realloc(vector->vec, vector->cap * vector->tamElem);
        if (!vector->vec) return -22;
    }
    memcpy(vector->vec + vector->ce * vector->tamElem, elem, vector->tamElem);
    vector->ce++;
    return TODO_OK;
}

int vectorInsertarEnPos_ALU(Vector* v, void* elem, int pos){
    if (v->ce == v->cap) {
        v->cap *= 2;
        v->vec = realloc(v->vec, v->cap * v->tamElem);
        if (!v->vec) return -22;
    }
    memcpy((char*)(v->vec) + (pos * v->tamElem), elem, v->tamElem);
    v->ce++;
    return TODO_OK;
}

bool buscarEnVectorOrd_ALU(const Vector* v, void* elem, Cmp cmp, int* pos){
    int inicio = 0;
    int fin = v->ce - 1;
    while (inicio <= fin){
        int medio = inicio + (fin - inicio) / 2;
        void* elemMedio = v->vec + (medio * v->tamElem);
        int resultado = cmp(elemMedio, elem);
        if(resultado == 0){
            *pos = medio;
            return true;
        }
        if(resultado < 0)
            inicio = medio + 1;
        if(resultado > 0)
            fin = medio - 1;
    }
    *pos = -1;
    return false;
}

int InsertarEnTdaOrdenado(Vector* vec, void* elem, Cmp comparar, Actualizar Actualizar){
    if(vec->ce == vec->cap){
        vec->cap *= 2;
        vec->vec = realloc(vec->vec, vec->cap * vec->tamElem);
        if(!vec->vec)
            return -22;
    }
    if(vec->ce  == 0){
        vectorInsertarAlFinal_ALU(vec, elem);
        return TODO_OK;
    }

    void * inicio;
    void* fin = vec->vec + (vec->ce * vec->tamElem);
    int resultado;

    for(inicio = vec->vec; inicio<fin; inicio+= vec->tamElem)
    {
        resultado = comparar(inicio, elem);
        if(resultado>0)
        {
            memmove(inicio + vec->tamElem, inicio, fin-inicio);
            memcpy(inicio, elem, vec->tamElem);
            vec->ce++;
            return TODO_OK;
        }
        if(resultado == 0)
        {
            Actualizar(inicio, elem);
            return TODO_OK;
        }
    }
    memcpy(fin, elem, vec->tamElem);
    vec->ce++;
    return TODO_OK;
}

int formatearLinea(FILE* arch, Telemetria* elem){
    char linea[300], *p, *pAux;
    if(fgets(linea, 300, arch)){
    p=strchr(linea, '\n');
    *p='\0';
    p-=10;
    pAux=p;
    while(*pAux == ' '){
        pAux++;
    }
    sscanf(pAux, "%[^\n]", elem->estado);
    *p='\0';
    p-=6;
    sscanf(p, "%f", &elem->litrosUso);
    *p='\0';
    p-=6;
    sscanf(p, "%f", &elem->kmRec);
    *p='\0';
    p-=7;
    sscanf(p, "%s", elem->Dom);
    *p='\0';
    sscanf(linea, "%s", elem->timeStamp);
    return TODO_OK;
    }
    return -1;
}
