#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "const.h"
#include "functions.h"

int main(int argsc, char *args[])
{
    tMV MV;
    int CONTROLVMX[] = {'V', 'M', 'X', '2', '5'};
    int CONTROLVMI[] = {'V', 'M', 'I', '2', '5'};
    int IPANTERIOR,i;
    int OK = 0;
    if (argsc > 1)
    {
        init_MV(&MV, &OK, CONTROLVMX, CONTROLVMI, argsc, args);
        if (OK)
        { // VALIDADO
            init_regs(&MV);
            init_funciones(&MV);
            printf("\n");
            //MV.DISSASEMBLER=1;
         if (MV.DISSASEMBLER)
            {
                IPANTERIOR = MV.REGS[IP].dato;
                if (MV.REGS[KS].dato >= 0)
                    mostrarConstantes(&MV);     
                MV.REGS[IP].dato = MV.REGS[CS].dato; 
                ejecucion(&MV);
                printf("\n");
                MV.REGS[IP].dato = IPANTERIOR;
                MV.DISSASEMBLER = 0;
            }
         

/*          for (i=0; i<8;i++){
                printf("%08x\n", MV.SEGMENTTABLE[i]);
            }
            printf("PS: %08x", MV.REGS[PS].dato);
*/
            if (MV.VERSION == 2)
                subrutinaprincipal(&MV);
                ejecucion(&MV);
        }
   }

    printf("\n FIN DE PROCESO.");
    return 0;
}
