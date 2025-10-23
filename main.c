#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "const.h"
#include "functions.h"

int main(int argsc, char *args[])
{
    tMV MV;
    int CONTROLVMX[] = { 'V','M','X','2','5' };
    int CONTROLVMI[] = { 'V','M','I','2','5' };
    int OK=0;
    //if (argsc>1) {
        init_MV(&MV, &OK, CONTROLVMX, CONTROLVMI, argsc, args );
        if (OK) {    //VALIDADO
            init_regs(&MV);
            init_funciones(&MV);
            printf("\n");
            MV.DISSASEMBLER=1;
            if (MV.DISSASEMBLER){
                ejecucion(&MV);
                printf("\n");
                MV.DISSASEMBLER=0;
            }
            if (MV.VERSION==2)
                subrutinaprincipal(&MV);
            ejecucion(&MV);
        }
    //}

    printf("\n FIN DE PROCESO.");
    return 0;
}
