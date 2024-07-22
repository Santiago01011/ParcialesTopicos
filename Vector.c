#include "Vector.h"


int vectorCrear(Vector* vec, size_t tamElem){
    vec->vec = malloc(tamElem * CAPACIDAD_INICIAL);
    if(!vec->vec)
        return SIN_MEM;
    vec->ce = 0;
    vec->cap = CAPACIDAD_INICIAL;
    vec->tamElem = tamElem;
    return TODO_OK;
}

void vectorDestruir(Vector* vec){
    if(vec->vec)
        free(vec->vec);
}

int vectorInsertarOrd(Vector* vec, void* elem, Cmp comparar, Actualizar actu){
    if(vec->ce  == 0){
        vectorInsertarAlFinal_ALU(vec, elem);
        return TODO_OK;
    }
    int pos;
	if(buscarEnVectorOrd_ALU(vec, elem, comparar, &pos)){
		actu(vec->vec + pos * vec->tamElem, elem);
	}else{
		memmove(vec->vec + (pos + 1) * vec->tamElem, vec->vec + pos * vec->tamElem, (vec->ce - pos) * vec->tamElem);
		vectorInsertarEnPos(vec, elem, pos);
	}
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

int vectorInsertarEnPos(Vector* vec, void* elem, int pos){
	if(vec->ce == vec->cap){
		vec->cap *= 2;
		vec->vec = realloc(vec->vec, vec->cap * vec->tamElem);
		if(!vec->vec)
			return -22;
	}
	memcpy(vec->vec + pos * vec->tamElem, elem, vec->tamElem);
	vec->ce++;
	return TODO_OK;
}

bool vectorEliminarElem(Vector*vector, int elem){

    int* posElim = vectorOrdBuscar(vector, elem); ///Busca la posicion del elemento a eliminar
    int* ult = vector->vec + vector->ce - 2;
    if(!posElim)
        return false;  ///Si no lo encuentra devuelve false

    for(int* i=posElim; i < ult; i++)
        *i = *(i+1); ///Desplaza los elementos a la izquierda
        ///vec[i]= vec[i+1];
    vector->ce--;

    if(vector->ce*4 <= vector->cap){ ///Si la cantidad de elementos es menor al 25% de la capacidad
        vector->cap /= 2; ///Se reduce la capacidad a la mitad
        vector->vec = realloc(vector->vec, vector->cap * sizeof(int)); ///Se redimensiona el vector
    }


    return true; ///Devuelve true si elimino el elemento
}

int vectorVaciar(Vector*vec){
    if(!vec->vec)
        return -1;
    vec->ce = 0;
    return TODO_OK;
}

void vectorEliminar(Vector*vector){
    if(vector->vec){
        free(vector->vec);
    }
}

bool buscarEnVectorOrd_ALU(const Vector* v, void* elem, Cmp cmp, int* pos){
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
    *pos = inicio;
    return false;
}
/*
void vectorOrdenarInser(Vector*vector){
    int j;
    for(int i=1; i<vector->ce; i++){
        int actual=vector->vec[i];
        for(j=i; j>0 && vector->vec[j-1]>actual; j--)
            vector->vec[j]=vector->vec[j-1];
        vector->vec[j]=actual;
    }
}
*/
int vectorCargarDeArchivo_ALU(const char* nombreArch, Vector* vector, size_t tamElem){
    FILE* arch = fopen(nombreArch, "rb");
    if(!arch){
        printf("Error al abrir el archivo %s", nombreArch);
        return -11;
    }
    void* elem = malloc(tamElem);
    if(!elem){
        printf("Error al asignar memoria para cargar archivo");
        fclose(arch);
        return SIN_MEM;
    }
    while(fread(elem, tamElem, 1, arch)==1){
        if(!vectorInsertarAlFinal_ALU(vector, elem)){
            fclose(arch);
            return -31;
        }
    }
    free(elem);
    fclose(arch);
    return TODO_OK;
}
