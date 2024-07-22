/***************************************************************************************
******************************* Complete sus datos *************************************
****************************************************************************************
* Apellido, Nombres:
*
* DNI:
*
****************************************************************************************/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../Cadena/Cadena.h"

#define TDA_LISTA_IMPL_DINAMICA_VEC
#include "../TDALista/TDALista.h"

#include "../SolucionBibliotecaTopicos/SolucionBiblioteca.h"

#define NOMBRE_ARCH_LIBROS "Libros.dat"
#define NOMBRE_ARCH_LIBROS_INDICE "Libros.idx"
#define NOMBRE_ARCH_MOVIMIENTOS "Movimientos.txt"

typedef void (*print_callback)(void*);


bool listaCrear_ALU(Lista* pl, size_t tamElem);
void listaDestruir_ALU(Lista* pl);
int cargarListaMovimientosLibro_ALU(Lista* plistaMov, const char* nombreArchMov);
int InsertarEnTdaOrdenado(Lista* vec, void* elem, Cmp comparar, Actualizar Actualizar);
int vectorCargarDeArchivo_ALU(const char* nombreArch, Lista* vector, size_t tamElem);
int vectorInsertarAlFinal_ALU(Lista* vector, void* elem);
bool buscarEnVectorOrd_ALU(const Lista* v, void* elem, Cmp cmp, int* pos);
int mostrarArchGen_ALU(const char* nombreArch, size_t tamElem, print_callback print);
int actualizarArchivoLibros_ALU(const char* nombreArchLibros, const char* nombreArchLibrosIdx, Lista* plistaMov);
int normalizarNombresLibros_ALU(const char* nombreArchLibros);


void printLibro(void* elem){
	Libro* a = (Libro*)elem;
	printf("Cod: %s\tNombre: %s\t\t\tCant: %d\n", a->codigo, a->nombre, a->cantidad);
}
void printMov(void* elem){
	MovLibro* a = (MovLibro*)elem;
	printf("CodL: %s\tCant: %d\n",a->codigoLibro, a->cantidad);
}
void printIdx(void* elem){
	IndLibro* a = (IndLibro*)elem;
	printf("Cod: %s\tReg: %ld\n",a->codigo, a->nroReg);
}
int cmpMov(const void* e1, const void* e2){
	MovLibro* a = (MovLibro*)e1;
	MovLibro* b = (MovLibro*)e2;
	return strcmp(a->codigoLibro, b->codigoLibro);
}
int cmpIdx(const void* e1, const void* e2){
	IndLibro* a = (IndLibro*)e1;
	IndLibro* b = (IndLibro*)e2;
	return strcmp(a->codigo, b->codigo);
}
void actuMov(void* actualizado, const void* actualizador){
	MovLibro* a = (MovLibro*)actualizado;
	MovLibro* b = (MovLibro*)actualizador;
	a->cantidad += b->cantidad;
}


int main()
{

	mostrarArchGen_ALU(NOMBRE_ARCH_LIBROS, sizeof(Libro), printLibro);

	Lista listaMov;
    if(!listaCrear_ALU(&listaMov, sizeof(MovLibro)))
		return 1;
	//listaCrear(&listaMov, sizeof(MovLibro));

	///Debe generar la lista de movimientos, consolidando los movimientos de los libros, a fin de que quede 1 s�lo elemento en la lista por cada libro.
	///Aumentando o disminuyendo la cantidad seg�n se trate de una devoluci�n o un pr�stamo respectivamente.
	puts("");
	if(cargarListaMovimientosLibro_ALU(&listaMov, NOMBRE_ARCH_MOVIMIENTOS)){
		return 2;
	}
	//cargarListaMovimientosLibro(&listaMov, NOMBRE_ARCH_MOVIMIENTOS);


	puts("");
	///Debe actualizar el archivo binario de Libros, sumando o restando la cantidad.
	///La cantidad en el archivo no puede quedar negativa. Si eso sucede, se deber� descartar el movimiento y continuar con el siguiente.
	///Para acceder a cada registro, debe hacer uso del archivo �ndice, que tiene cada registro, el c�digo del libro y su ubicaci�n en el archivo(Nro de registro empezando por el 0).
	///Debe cargar el �ndice en memoria, para trabajar con �l.

	actualizarArchivoLibros_ALU(NOMBRE_ARCH_LIBROS, NOMBRE_ARCH_LIBROS_INDICE, &listaMov);
	//actualizarArchivoLibros(NOMBRE_ARCH_LIBROS, NOMBRE_ARCH_LIBROS_INDICE, &listaMov);

	puts("");

	///Debe normalizar los nombres de los libros en el archivo.
	///Dejando s�lo 1 espacio entre palabras, sin espacios ni caracteres antes de la primer pal�bra y despu�s de la �ltima.
	///Quedando la primer letra de cada palabra en may�scula y el resto en min�scula.

	normalizarNombresLibros_ALU(NOMBRE_ARCH_LIBROS);
	//normalizarNombresLibros(NOMBRE_ARCH_LIBROS);

	mostrarArchGen_ALU(NOMBRE_ARCH_LIBROS, sizeof(Libro), printLibro);

	listaDestruir_ALU(&listaMov);
	//listaDestruir(&listaMov);

    return 0;
}

bool listaCrear_ALU(Lista* pl, size_t tamElem){
	pl->vec = malloc(tamElem * 10);
	if(!pl->vec){
		return false;
	}
	pl->cap=10;
	pl->ce=0;
	pl->tamElem=tamElem;
    return true;
}

int cargarListaMovimientosLibro_ALU(Lista* plistaMov, const char* nombreArchMov){
	char linea[300];
	char opc;
	MovLibro auxMov;
	FILE* arch = fopen(nombreArchMov, "r");
	if(!arch){
		printf("Error al abrir el archivo %s", nombreArchMov);
		return -12;
	}
	while(fgets(linea, 300, arch)){
		if(sscanf(linea, "%[^|]|%[^|]|%c", auxMov.codigoLibro, auxMov.codigoSocio, &opc)==3){
			auxMov.cantidad = 0;
			if(opc=='P') auxMov.cantidad--;
			else auxMov.cantidad++;
			InsertarEnTdaOrdenado(plistaMov, &auxMov, cmpMov, actuMov);
		}
	}
	return TODO_OK;
}

int actualizarArchivoLibros_ALU(const char* nombreArchLibros, const char* nombreArchLibrosIdx, Lista* plistaMov){
	IndLibro* auxIdx;
	Libro auxL;
	Lista vecIdx;
	int pos;
	listaCrear_ALU(&vecIdx, sizeof(IndLibro));
	FILE* archL = fopen(nombreArchLibros, "r+b");
	if(!archL){
		return -13;
	}
	vectorCargarDeArchivo_ALU(nombreArchLibrosIdx, &vecIdx, vecIdx.tamElem);
	//mostrarArchGen_ALU(nombreArchLibrosIdx, sizeof(IndLibro), printIdx);
	for(int i=0; i < plistaMov->ce; i++){
		MovLibro* ptrMovm = plistaMov->vec + (i*plistaMov->tamElem);
		buscarEnVectorOrd_ALU(&vecIdx, ptrMovm->codigoLibro, cmpIdx, &pos);
		auxIdx = vecIdx.vec + vecIdx.tamElem*pos;
		fseek(archL, auxIdx->nroReg*sizeof(Libro), SEEK_SET);
		fread(&auxL, sizeof(Libro), 1, archL);
		auxL.cantidad += ptrMovm->cantidad;
		if(auxL.cantidad >= 0){
			fseek(archL, auxIdx->nroReg*sizeof(Libro), SEEK_SET);
			fwrite(&auxL, sizeof(Libro), 1, archL);
		}else
			printf("Sin stock en %s\n", auxL.nombre);

	}
	fclose(archL);
	return TODO_OK;
}

bool buscarEnVectorOrd_ALU(const Lista* v, void* elem, Cmp cmp, int* pos){
    int inicio = 0, fin = v->ce - 1;
    while(inicio <= fin){
        int medio = inicio + (fin - inicio) / 2;
        void* elemMedio =v->vec + (medio * v->tamElem);
        int resultado = cmp(elemMedio, elem);
        if (resultado == 0){
            *pos = medio;
            return true;
        }
        else if(resultado < 0)
            inicio = medio + 1;
        else
            fin = medio - 1;
    }
    *pos = -1;
    return false;
}

int normalizarNombresLibros_ALU(const char* nombreArchLibros){
	FILE* arch = fopen(nombreArchLibros, "r+b");
	if(!arch){
		printf("Error al abrir %s", nombreArchLibros);
		return -14;
	}
	char nombreNuevo[101];
	Libro auxL;
	fread(&auxL, sizeof(Libro), 1, arch);
	while(!feof(arch)){
        memset(nombreNuevo, '\0', sizeof(nombreNuevo)); // Reinicia nombreNuevo al inicio de cada nombre
        char *p = auxL.nombre; // Inicializa el puntero al inicio del nombre del libro
        int i = 0; // Reinicia el índice para nombreNuevo
        // Copia el nombre del libro a nombreNuevo
        while(*p != '\0'){
			if(*p != ' '){
				nombreNuevo[i] = *p;
				p++;
				i++;
			}
			else
				p++;
        }
        strcpy(auxL.nombre, nombreNuevo);
        fseek(arch, -1 * sizeof(Libro), SEEK_CUR);
        fwrite(&auxL, sizeof(Libro), 1, arch);
        fflush(arch);
        fread(&auxL, sizeof(Libro), 1, arch); // Lee el siguiente libro
    }

    fclose(arch);
	return TODO_OK;
}

void listaDestruir_ALU(Lista* pl){
  if(pl){
	free(pl->vec);
  }
}

int mostrarArchGen_ALU(const char* nombreArch, size_t tamElem, print_callback print){

	FILE* arch = fopen(nombreArch, "rb");
	if(!arch){
		printf("Error al abrir el archivo %s", nombreArch);
		return -11;
	}
	void* elem = malloc(tamElem);
	if(!elem){
		puts("Error al asignar memoria");
		fclose(arch);
		return -21;
	}
	printf("-------------%s-------------\n", nombreArch);
	while(fread(elem, tamElem, 1, arch)){
		print(elem);
	}
	puts("-----------------Fin-----------------");
	fclose(arch);
	free(elem);
	return TODO_OK;
}

int InsertarEnTdaOrdenado(Lista* vec, void* elem, Cmp comparar, Actualizar Actualizar){
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

int vectorInsertarAlFinal_ALU(Lista* vector, void* elem) {
    if (vector->ce == vector->cap) {
        vector->cap *= 2;
        vector->vec = realloc(vector->vec, vector->cap * vector->tamElem);
        if (!vector->vec) return -22;
    }
    memcpy(vector->vec + vector->ce * vector->tamElem, elem, vector->tamElem);
    vector->ce++;
    return TODO_OK;
}

int vectorCargarDeArchivo_ALU(const char* nombreArch, Lista* vector, size_t tamElem){
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
    free(elem);
    return TODO_OK;
}
