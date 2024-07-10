/***************************************************************************************
******************************* Complete sus datos *************************************
****************************************************************************************
* Apellido, Nombres:
*
* DNI:
*
***************************************************************************************/
/***************************************************************************************
********************* Descomente la solución que quiere probar: ************************
***************************************************************************************/
//#define SOLUCION_DOC
#define SOLUCION_ALU
#define CAP_INICIAL 50
/***************************************************************************************
************************************ Corrección ****************************************
****************************************************************************************
*
****************************************************************************************
* Nota:
***************************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../Vector/Vector.h"
#include "../SolucionParcialDeliveryTopicos/TiposDelivery.h"
#include "../SolucionParcialDeliveryTopicos/SolucionDelivery.h"
#include "../UtilitariasTopicos/Utilitarias.h"


typedef void (*print_callback)(const void*);
int txtABin_ALU(const char* nombreArchTxt);
void cambiarExtension_ALU(const char* nombreArchivoOrig, char* nombreArchivoConExtension, const char* extension);
int mostrarArchivo_ALU(const char* nombreArchivo, unsigned tam, print_callback print);
int vectorCrear_ALU(Vector* vec, size_t tamElem);
void vectorDestruir_ALU(Vector* vec);
int vectorVaciar_ALU(Vector* vec);
int vectorCargarDeArchivo_ALU(const char* nombreArch, Vector* vector, size_t tamElem);
bool buscarEnVectorOrd_ALU(const Vector* v, void* elem, Cmp cmp, int* pos);
int vectorInsertarAlFinal_ALU(Vector* vector, void* elem);
void printPedido(const void* pe);
void printReceta(const void* re);
void printIngrediente(const void* in);
void printIndiceReceta(const void* ir);
int cmpRecetas(const void* elem1, const void* elem2);
int cmpIngredientes(const void* elem1, const void* elem2);


void generarArchivoIngredientes() {
    FILE *fp = fopen("Ingredientes.dat", "wb");
    if (fp == NULL) {
        perror("Error al abrir Ingredientes.dat");
        return;
    }

    Ingrediente ingredientes[5] = {
        {"Tomate", 100},
        {"Zanahoria", 150},
        {"Salsa", 200},
        {"Queso", 300},
        {"Carne", 400}
    };

    for (int i = 0; i < sizeof(ingredientes)/sizeof(ingredientes[0]); i++) {
        fwrite(&ingredientes[i], sizeof(Ingrediente), 1, fp);
    }

    fclose(fp);
}

void generarArchivoPedidos() {
    FILE *fp = fopen("Pedidos.txt", "w");
    if (fp == NULL) {
        perror("Error al abrir Pedidos.txt");
        return;
    }
//Los pedidos se encuentran en un archivo de texto de long. variable “Pedidos.txt”, cuya estructura es: Nro. Pedido (entero), Cod. Receta (cadena(10)) y Cantidad (entero).
    Pedido pedidos[5] = {
        {1, "Pizza", 2},
        {2, "Pizza", 3},
        {3, "Empanadas", 1},
        {4, "Empanadas", 2},
        {5, "Milanesa", 1}
    };

    fprintf(fp,"Nro\tPedido\tCantidad\n");
    for (int i = 0; i < 5; i++) {
        fprintf(fp,"%d\t%s\t%d\n", pedidos[i].nroPedido, pedidos[i].codReceta, pedidos[i].cantidad);
    }

    fclose(fp);
}

void generarArchivoRecetas() {
    FILE *fp = fopen("Recetas.dat", "wb");
    if (fp == NULL) {
        perror("Error al abrir Recetas.dat");
        return;
    }

    Receta recetas[6] = {
        {"Pizza", "Tomate", 2},
        {"Pizza", "Queso", 3},
        {"Pizza", "Salsa", 1},
        {"Empanadas", "Carne", 2},
        {"Empanadas", "Queso", 1},
        {"Milanesa", "Carne", 1}
    };

    for (int i = 0; i < sizeof(recetas)/sizeof(recetas[0]); i++) {
        fwrite(&recetas[i], sizeof(Receta), 1, fp);
    }

    fclose(fp);
}

int main()
{
   //generarArchivoIngredientes();
   //generarArchivoPedidos();
   //generarArchivoRecetas();

	mostrarArchivo_ALU("Recetas.dat", sizeof(Receta), printReceta);

	puts("Ingredientes antes de la actualizacion");
	mostrarArchivo_ALU("Ingredientes.dat", sizeof(Ingrediente), printIngrediente);

	int resp;

#ifdef SOLUCION_DOC
	resp = satisfacerPedidos("Pedidos.txt", "Recetas.dat", "Ingredientes.dat");
#else
	resp = satisfacerPedidos_ALU("Pedidos.txt", "Recetas.dat", "Ingredientes.dat");
#endif

	if(resp != TODO_OK)
	{
		puts("Error actualizando los pedidos.");
		return resp;
	}

	puts("Ingredientes despues de la actualizacion");
	mostrarArchivo_ALU("Ingredientes.dat", sizeof(Ingrediente), printIngrediente);

	return TODO_OK;
}


///Procesa los pedidos del archivo Pedidos.txt, actualizando el archivo Ingredientes.dat, para que �ste refleje la cantidad que queda en stock luego de despachar los pedidos.
int satisfacerPedidos_ALU(const char* nombreArchPedidos, const char* nombreArchRecetas, const char* nombreArchIngredientes){
    int pos;
    char nombreRidx[100]="", nombreSidx[100]="", pedidoActual[11];
    cambiarExtension_ALU(nombreArchRecetas, nombreRidx,".idx");
    cambiarExtension_ALU(nombreArchIngredientes, nombreSidx, ".idx");
    txtABin_ALU(nombreArchPedidos);
    mostrarArchivo_ALU("Pedidos.dat", sizeof(Pedido), printPedido);
    mostrarArchivo_ALU(nombreRidx, sizeof(IndReceta), printIndiceReceta);
    mostrarArchivo_ALU(nombreSidx, sizeof(IndIngrediente), printIndiceReceta);
    Vector ingredientesRecetaAct;
    Vector idxReceta;
    Vector idxIngredientes;
    vectorCrear_ALU(&ingredientesRecetaAct, sizeof(Receta));
    vectorCrear_ALU(&idxReceta, sizeof(IndReceta));
    vectorCrear_ALU(&idxIngredientes, sizeof(Ingrediente));
    vectorCargarDeArchivo_ALU(nombreRidx, &idxReceta, sizeof(IndReceta));
    vectorCargarDeArchivo_ALU(nombreSidx, &idxIngredientes, sizeof(IndIngrediente));
    FILE* archRecetas = fopen(nombreArchRecetas, "rb"); if(!archRecetas){
        printf("Error al abrir el archivo Recetas.dat");
        return ERR_ARCHIVO;
    }
    FILE* archPedidos = fopen("Pedidos.dat", "rb"); if(!archPedidos){
        fclose(archRecetas);
        printf("Error al abrir el archivo Pedidos.dat");
        return ERR_ARCHIVO;
    }
    FILE* archIngredientes = fopen(nombreArchIngredientes, "r+b"); if(!archIngredientes){
        fclose(archRecetas);
        fclose(archPedidos);
        printf("Error al abrir el archivo Ingredientes.dat");
        return ERR_ARCHIVO;
    }
    Receta auxReceta;
    Pedido auxPedido;
    Ingrediente auxIngrediente;
    IndReceta* ptrInd;
    IndReceta auxInd;
    while(fread(&auxPedido, sizeof(Pedido), 1, archPedidos)==1){
        strcpy(pedidoActual, auxPedido.codReceta);
        buscarEnVectorOrd_ALU(&idxReceta, auxPedido.codReceta, cmpRecetas, &pos);
        ptrInd = (IndReceta*)(idxReceta.vec) + pos;
        auxInd = *ptrInd;
        puts("Procesando receta... ");
        printIndiceReceta(&auxInd);
        fseek(archRecetas, auxInd.nroReg * sizeof(Receta), SEEK_SET);
        puts("Ingredientes");
        fread(&auxReceta, sizeof(Receta), 1, archRecetas);
        while(strcmp(auxReceta.codReceta, pedidoActual)==0 && !feof(archRecetas)){
            auxReceta.cantidad *= auxPedido.cantidad;
            vectorInsertarAlFinal_ALU(&ingredientesRecetaAct, &auxReceta);
            fread(&auxReceta, sizeof(Receta), 1, archRecetas);
        }

        for (int i = 0; i < ingredientesRecetaAct.ce; i++) {
            Receta* ptrReceta = (Receta*)(ingredientesRecetaAct.vec) + i;
            printReceta(ptrReceta);
            buscarEnVectorOrd_ALU(&idxIngredientes, ptrReceta->codIngr, cmpIngredientes, &pos);
            ptrInd = (IndReceta*)(idxIngredientes.vec) + pos;
            auxInd = *ptrInd;
            printIndiceReceta(&auxInd);
            fseek(archIngredientes, auxInd.nroReg * sizeof(Ingrediente), SEEK_SET);
            fread(&auxIngrediente, sizeof(Ingrediente), 1, archIngredientes);
            auxIngrediente.stock -= ptrReceta->cantidad;
            if(auxIngrediente.stock <= 0){
                printf("Error, sin stock de %s", auxIngrediente.codIngr);
                fclose(archRecetas);
                fclose(archPedidos);
                fclose(archIngredientes);
                vectorDestruir_ALU(&ingredientesRecetaAct);
                vectorDestruir_ALU(&idxReceta);
                vectorDestruir_ALU(&idxIngredientes);
                remove("Pedidos.dat");
                return 5;
            }
            fseek(archIngredientes, auxInd.nroReg * sizeof(Ingrediente), SEEK_SET);
            fwrite(&auxIngrediente, sizeof(Ingrediente), 1, archIngredientes);
        }
        puts("fin de rececta\n");
        vectorVaciar_ALU(&ingredientesRecetaAct);
    }
    fclose(archRecetas);
    fclose(archPedidos);
    fclose(archIngredientes);
    vectorDestruir_ALU(&ingredientesRecetaAct);
    vectorDestruir_ALU(&idxReceta);
    vectorDestruir_ALU(&idxIngredientes);
    remove("Pedidos.dat");
	return TODO_OK;
}

///FUNCIONES ALU
void printPedido(const void* pe){
    Pedido* pedido = (Pedido*)pe;
    printf("Nro: %d\tCod receta: %s\tCantidad: %d\n", pedido->nroPedido, pedido->codReceta, pedido->cantidad);
}
void printReceta(const void* re){
    Receta* receta = (Receta*)re;
    printf("Cod: %s\tCod ingrediente: %s\tCant: %d\n", receta->codReceta, receta->codIngr, receta->cantidad);
}
void printIngrediente(const void* in){
    Ingrediente* ingrediente = (Ingrediente*)in;
    printf("Cod: %s\tstock: %d\n", ingrediente->codIngr, ingrediente->stock);
}
void printIndiceReceta(const void* ir){
    IndReceta* indice = (IndReceta*)ir;
    printf("Cod: %s\tNro reg: %d\n", indice->codReceta, indice->nroReg);
}

int mostrarArchivo_ALU(const char* nombreArchivo, unsigned tam, print_callback print){
    FILE * pf = fopen(nombreArchivo, "rb");
    if (!pf){
        printf("Error al abrir el archivo %s", nombreArchivo);
        return ERR_ARCHIVO;
    }
    void * buffer = malloc(tam);
    if(!buffer){
        fclose(pf);
        return -12;
    }
    printf("-------Archivo: %s-------\n", nombreArchivo);
    while(fread(buffer, tam, 1, pf) == 1){
        print(buffer);
    }
    printf("-------Fin de archivo-------\n");
    fclose(pf);
    free(buffer);
    return TODO_OK;
}

int vectorCrear_ALU(Vector* vec, size_t tamElem){
    vec->vec = malloc(CAP_INICIAL * tamElem);
    if (!vec->vec) {
        return SIN_MEM;
    }
    vec->ce = 0;
    vec->cap = CAP_INICIAL;
    vec->tamElem = tamElem;
    return TODO_OK;
}

void vectorDestruir_ALU(Vector* vec){
    if (vec->vec) {
        free(vec->vec);
        vec->vec = NULL;
    }
    vec->ce = 0;
    vec->cap = 0;
    vec->tamElem = 0;
}

int vectorVaciar_ALU(Vector* vec){
    if (!vec)
        return 1;
    vec->ce = 0;
    return TODO_OK;
}

int vectorCargarDeArchivo_ALU(const char* nombreArch, Vector* vector, size_t tamElem){
    FILE* archivo = fopen(nombreArch, "rb");
    if (!archivo) {
        return ERR_ARCHIVO;
    }
    void* elem = malloc(tamElem);
    if(!elem){
        fclose(archivo);
        return SIN_MEM;
    }
    while (fread(elem, tamElem, 1, archivo) == 1) {
        if (vectorInsertarAlFinal_ALU(vector, elem) != 0){
            fclose(archivo);
            return ERR_ARCHIVO;
        }
    }
    fclose(archivo);
    return TODO_OK;
}

bool buscarEnVectorOrd_ALU(const Vector* v, void* elem, Cmp cmp, int* pos){
    int inicio = 0;
    int fin = v->ce - 1;
    while (inicio <= fin) {
        int medio = inicio + (fin - inicio) / 2;
        // Calcula la dirección del elemento medio
        void* elemMedio = (char*)v->vec + (medio * v->tamElem);
        int resultado = cmp(elemMedio, elem);
        if (resultado == 0) {
            *pos = medio;
            return true;
        } else if (resultado < 0) {
            inicio = medio + 1;
        } else {
            fin = medio - 1;
        }
    }
    *pos = -1;
    return false;
}

int vectorInsertarAlFinal_ALU(Vector* vector, void* elem){
    // Verificar si el vector está lleno
    if (vector->ce == vector->cap) {
        vector->cap *= 2;
        vector->vec = realloc(vector->vec, vector->cap * vector->tamElem);
        if (!vector->vec) {
            return SIN_MEM;
        }
    }
    memcpy((char*)vector->vec + vector->ce * vector->tamElem, elem, vector->tamElem);
    vector->ce++;
    return TODO_OK;
}

void cambiarExtension_ALU(const char* nombreArchivoOrig, char* nombreArchivoConExtension, const char* extension){
    int i=0;
    while(nombreArchivoOrig[i]!= '.' && nombreArchivoOrig[i] != '\0'){
        nombreArchivoConExtension[i]=nombreArchivoOrig[i];
        i++;
    }
    strcat(nombreArchivoConExtension, extension);
}

int txtABin_ALU(const char* nombreArchTxt){
    FILE* archTxt = fopen(nombreArchTxt, "r");
    if(!archTxt){
        printf("Error al abrir el archivo %s", nombreArchTxt);
        return ERR_ARCHIVO;
    }
    char nombreBin[100];
    cambiarExtension_ALU(nombreArchTxt, nombreBin, ".dat");
    FILE* archBin = fopen(nombreBin, "wb");
    if(!archBin){
        fclose(archTxt);
        printf("Error al abrir el archivo %s", nombreBin);
        return ERR_ARCHIVO;
    }
    Pedido auxPedido;
    char linea[200];
    // fgets(linea, 200, archTxt);
    while(fgets(linea, sizeof(Pedido), archTxt)){
        if(sscanf(linea,"%d %s %d",&auxPedido.nroPedido, auxPedido.codReceta,&auxPedido.cantidad)==3)
            fwrite(&auxPedido, sizeof(Pedido), 1, archBin);
    }
    fclose(archTxt);
    fclose(archBin);
    return TODO_OK;
}

int cmpRecetas(const void* elem1, const void* elem2){
    Receta* receta1 = (Receta*)elem1;
    Receta* receta2 = (Receta*)elem2;
    return strcmp(receta1->codReceta, receta2->codReceta);
}

int cmpIngredientes(const void* elem1, const void* elem2){
    Ingrediente* ing1 = (Ingrediente*)elem1;
    Ingrediente* ing2 = (Ingrediente*)elem2;
    return strcmp(ing1->codIngr, ing2->codIngr);
}



