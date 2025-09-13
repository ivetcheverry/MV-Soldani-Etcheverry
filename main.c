
#include <stdio.h>
#include <stdlib.h>
#include "cons.h" //ARCHIVO CON CONSTANTES

//PROTOTIPOS A TODAS LAS FUNCIONES
void validar(int [],int *, int *, int[], int [][TABLA_M]);
void init_regs(int [],int);



//TYPEDEF DE ALGUNOS TIPOS GLOBALES
typedef void (*tfunc_2Op)(int memoria[],int a,int b, int atype, int btype);
typedef void (*tfunc_1Op)(int memoria[],int a, int atype);



int main()
{
    tfunc_2Op func_2op[16];
    tfunc_1Op func_1op[9];
    int segmentTable[TABLA_N][TABLA_M];
    int OK=0,CSsize=0;
    int memoria[RAM];
    int CONTROL[] = { 'V','M','X','2','5' };
    validar(CONTROL, &OK, &CSsize, memoria,segmentTable);
    if (OK) {    //VALIDADO
        init_regs(memoria,CSsize);  
    }
    return 0;
}


