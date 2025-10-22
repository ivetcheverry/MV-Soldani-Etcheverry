#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "const.h"
#include "functions.h"


int main(int argsc, char *args[])
{
    tMV MV;
    int CONTROL[] = { 'V','M','X','2','5' };

    char *extension;
    

    //char NOMBREARCHIVO[15];
    int OK=0;
    if (argsc>1) {
        //strcpy(NOMBREARCHIVO,args[1]);
        init_MV(&MV, &OK, CONTROL, argsc, args);//, NOMBREARCHIVO);
        if (OK) {    //VALIDADO
            init_regs(&MV);
            init_funciones(&MV);
            printf("\n");
            if (MV.DISSASEMBLER){
                ejecucion(&MV);
                init_regs(&MV);
                printf("\n");
                MV.DISSASEMBLER=0;
            }
            ejecucion(&MV); 
        }   
    }

    printf("\n FIN DE PROCESO.");
    return 0;
}
