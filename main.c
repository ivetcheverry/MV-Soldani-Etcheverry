
#include <stdio.h>
#include <stdlib.h>
#include "const.h" //ARCHIVO CON CONSTANTES GENERALES
#include "functions.h"





int main()
{
    tfunc_2op func_2op[16];
    tfunc_1op func_1op[9];

    func_2op[MOV].func= mov;
    int segmentTable[TABLA_N][TABLA_M];
    int OK=0,CSsize=0;
    int memoria[RAM];
    int CONTROL[] = { 'V','M','X','2','5' };
    validar(CONTROL, &OK, &CSsize, memoria,segmentTable);
    if (OK) {    //VALIDADO
        init_regs(memoria,CSsize);  
        ejecucion(memoria,CSsize,func_2op,func_1op);
    }
    return 0;
}


