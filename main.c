
#include <stdio.h>
#include <stdlib.h>
#include "cons.h" //ARCHIVO CON CONSTANTES

void validar(int [],int *, int *, int[]);
void init_regs(int [],int);

int main()
{
    int OK=0,CSsize=0;
    int memoria[16328];
    int CONTROL[] = { 'V','M','X','2','5' };
    validar(CONTROL, &OK, &CSsize, memoria);
    if (OK) {    //VALIDADO
        init_regs(memoria,CSsize);
    }
    return 0;
}


