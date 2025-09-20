#include <stdio.h>
#include <stdlib.h>
#include "const.h" //ARCHIVO CON CONSTANTES GENERALES
#include "functions.h"


int main(int argsc, char *args[])
{
    tMV MV;
    int VERSION = 1;
    int CONTROL[] = { 'V','M','X','2','5' };
    int OK=0;
    init_MV(&MV, &OK, CONTROL,VERSION, argsc, args);
    if (OK) {    //VALIDADO
        init_regs(&MV);  
        init_funciones(&MV);
        printf("\n\n-EJECUCION!--------------------");
        ejecucion(&MV);
    }

    printf("\n FIN DE PROCESO.");
    return 0;
}
