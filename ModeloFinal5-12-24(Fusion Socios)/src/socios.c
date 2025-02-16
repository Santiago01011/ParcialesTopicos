#include "socios.h"
#define STAM 48


int title(){

    printf("   _                   __\n");
    printf("  |_     _ o  _  ._   (_   _   _ o  _   _\n");
    printf("  | |_| _> | (_) | |  __) (_) (_ | (_) _>\n");
    printf("\n");
    return 1;
}

//
//int compara_fechas_res(const t_fecha *f1, const t_fecha *f2)
//
////compara dos estructuras t_fecha, devolviendo:
//FECHA_MENOR  --->  f1  < f2
//FECHA_IGUAL  --->  f1 == f2
//FECHA_MAYOR  --->  f1  > f2

/**
* Compares 2 dates and returns:
* Positive if the first is greater than the second,
* Negative if the first is not greater than the second.
* Zero if the dates are equal.
*/
int cmpDate(const void* d1, const void* d2){
    t_date* fDate = (t_date*)d1;
    t_date* sDate = (t_date*)d2;
    if ( fDate->y != sDate->y )
        return fDate->y - sDate->y;
    else if ( fDate->m != sDate->m )
        return fDate->m - sDate->m;
    else if ( fDate->d != sDate->d )
        return fDate->d - sDate->d;
    else
        return 0;
}

/**
* Compares 2 t_socios by DNI and returns:
* Positive if the first is greater than the second.
* Negative if the first is not greater than the second.
* Zero if they are equal.
*/
int cmpSocio(const void* d1, const void* d2){
    return ((t_socio*)d1)->dni - ((t_socio*)d2)->dni;
}


/**
* Reads a register from a text archive and returns a t_socio structure.
*/
int readSocioFromTXT(FILE* arch, t_socio* dest){
    char buffer[STAM];
    if (!arch){
        puts("File pointer is NULL");
        return 0;
    }
    if ( fgets(buffer, STAM, arch) ){
        if( sscanf(buffer, "%ld|%29[^|]|%d/%d/%d",
            &dest->dni, dest->name,
            &dest->inscription_date.d, &dest->inscription_date.m,
            &dest->inscription_date.y) )
                return 1;
            else
                return 0;
    }
    else
        return 0;
}


void printSocio(const void* elem){
    printf("Socio: %ld|%s|%d/%d/%d\n",
           ((t_socio*)elem)->dni, ((t_socio*)elem)->name,
           ((t_socio*)elem)->inscription_date.d, ((t_socio*)elem)->inscription_date.m, ((t_socio*)elem)->inscription_date.y);
}

int mergeClubesAyB(const int option){
    int res;
    t_socio auxB, auxA;
    t_date today;
    FILE *archA, *archB, *archF;
    today.d = 16;
    today.m = 2;
    today.y = 2025;
    archA = fopen(FILE_PATH_A, "rb");
    if(!archA){
        puts("Error opening: ");
        puts(FILE_PATH_A);
        return 0;
    }
    archB = fopen(FILE_PATH_B, "r");
    if(!archB){
        puts("Error opening: ");
        puts(FILE_PATH_B);
        fclose(archA);
        return 0;
    }
    archF = fopen(FILE_PATH_F, "wt");
    if(!archF){
        puts("Error opening: ");
        puts(FILE_PATH_F);
        fclose(archA);
        fclose(archB);
        return 0;
    }

    readSocioFromTXT(archB, &auxB);
    fread(&auxA, sizeof(t_socio), 1, archA);
    while( !feof(archB) && !feof(archA) ) {
        res = cmpSocio(&auxA, &auxB);
        if( res == 0 ) {
            if ( option == 1 ) {
                if( cmpDate(&auxA.inscription_date, &auxB.inscription_date) > 0 )
                    auxB.inscription_date = auxA.inscription_date;
            }
            if ( option == 2 ) {
                if( cmpDate(&auxA.inscription_date, &auxB.inscription_date) < 0 )
                    auxB.inscription_date = auxA.inscription_date;
            }
            if ( option == 3 ) {
                    auxB.inscription_date = today;
            }
            //printSocio(&auxB);
            fprintf(archF, "%ld|%s|%d/%d/%d\n",
                auxB.dni, auxB.name,
                auxB.inscription_date.d, auxB.inscription_date.m,
                auxB.inscription_date.y
            );
            readSocioFromTXT(archB, &auxB);
            fread(&auxA, sizeof(t_socio), 1, archA);
        }
        if ( res < 0 ) {
            if ( option == 1 ) {
                if( cmpDate(&auxA.inscription_date, &auxB.inscription_date) > 0 )
                    auxA.inscription_date = auxA.inscription_date;
            }
            if ( option == 2 ) {
                if( cmpDate(&auxA.inscription_date, &auxB.inscription_date) < 0 )
                    auxA.inscription_date = auxA.inscription_date;
            }
            if ( option == 3 ) {
                    auxA.inscription_date = today;
            }
            //printSocio(&auxA);
            fprintf(archF, "%ld|%s|%d/%d/%d\n",
                    auxA.dni, auxA.name,
                    auxA.inscription_date.d, auxA.inscription_date.m,
                    auxA.inscription_date.y
                );
            fread(&auxA, sizeof(t_socio), 1, archA);
        }
        if ( res > 0 ) {
            if ( option == 1 ) {
                if( cmpDate(&auxA.inscription_date, &auxB.inscription_date) > 0 )
                    auxB.inscription_date = auxA.inscription_date;
            }
            if ( option == 2 ) {
                if( cmpDate(&auxA.inscription_date, &auxB.inscription_date) < 0 )
                    auxB.inscription_date = auxA.inscription_date;
            }
            if ( option == 3 ) {
                    auxB.inscription_date = today;
            }
            //printSocio(&auxB);
            fprintf(archF, "%ld|%s|%d/%d/%d\n",
                    auxB.dni, auxB.name,
                    auxB.inscription_date.d, auxB.inscription_date.m,
                    auxB.inscription_date.y
                );
            readSocioFromTXT(archB, &auxB);
        }

    }
    fclose(archA);
    fclose(archB);
    fclose(archF);
    return 1;
}
