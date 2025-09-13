
#include <stdio.h>
#include <stdlib.h>
#include "const.h" //ARCHIVO CON CONSTANTES GENERALES

//PROTOTIPOS A TODAS LAS FUNCIONES
void validar(int [],int *, int *, int[], int [][TABLA_M]);
void init_regs(int [],int);


typedef struct tfunc_2op {
    char nombre[5];
    void (*tfunc_2Op)(int memoria[],int a,int b, int atype, int btype);//vector con punteros a funciones void con 2 parámetros
}tfunc_2op ;

typedef struct  tfunc_1op  {
    char nombre[5];
    void (*tfunc_1Op)(int memoria[],int a, int atype); //vector con punteros a funciones void con 1 parámetros
}tfunc_1op ;


int main()
{
    tfunc_2op func_2op[16];
    tfunc_1op func_1op[9];
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


