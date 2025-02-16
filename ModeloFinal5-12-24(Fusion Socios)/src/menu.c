#include "main.h"

int mainMenu(const char * msg){
    t_socio aux;
    FILE *arch;
    char opt;
    do{
    title();
    puts(msg);
    fflush(stdin);
    opt = getchar();
    opt = toupper(opt);

    switch(opt){
        case 'A':
            arch = fopen(FILE_PATH_A, "rb");
            puts("Showing");
            puts(FILE_PATH_A);
            if(!arch){
                puts("Error opening ");
                puts(FILE_PATH_A);
                return 0;
            }
            while( fread(&aux, sizeof(t_socio), 1, arch) )
                printSocio(&aux);
            fclose(arch);
            break;
        case 'B':
            puts("Showing");
            puts(FILE_PATH_B);
            arch = fopen(FILE_PATH_B, "r");
            if(!arch){
                puts("Error opening ");
                puts(FILE_PATH_B);
                return 0;
            }
            while( readSocioFromTXT(arch, &aux) )
                printSocio(&aux);
            fclose(arch);
            break;
        case 'M':
            puts("------------Merge options------------\n"
                 "[A] Keep the newest registration date\n"
                 "[B] Keep the oldest registration date\n"
                 "[C] Set the registration date to today\n"
                );
            fflush(stdin);
            opt = getchar();
            opt = toupper(opt);
            switch(opt){
                case 'A':
                    mergeClubesAyB(1);
                    break;
                case 'B':
                    mergeClubesAyB(2);
                    break;
                case 'C':
                    mergeClubesAyB(3);
                    break;
            }
            puts("Showing");
            puts(FILE_PATH_F);
            arch = fopen(FILE_PATH_F, "r");
            if(!arch){
                puts("Error opening ");
                puts(FILE_PATH_F);
                return 0;
            }
            while( readSocioFromTXT(arch, &aux) )
                printSocio(&aux);
            fclose(arch);
        case 'F':
            break;
        default:
            puts("Not supported option");
            break;
    }

    } while ( opt != 'F' );
    return 1;
}
