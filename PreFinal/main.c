#include "main.h"

int main()
{
    tMat mat;
//    mainMenu("\nSelect an option\n"
//             "-A: Create an integer matrix\n"
//             "-B: Create an floats matrix\n"
//             "option: ");
    if( !loadMatFromArch("matrix.txt", &mat) )
        return 5;
    printMat(&mat, printMatFloat);
    puts("Inferior triangle");
    printMatInfTri(&mat, printMatFloat);
    return 0;
}
