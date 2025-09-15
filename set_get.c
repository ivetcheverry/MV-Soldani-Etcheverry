#include <stdio.h>
#include <stdlib.h>
#include "const.h"
#include "functions.h"


int get(int memoria[], int op1) {
    int aux;

    int valor = op1 & 0x00FFFFFF;
    int tipo = (op1 >> 24) & 0xFF;

        if (tipo == INM)
            aux=valor;
        else if (tipo == REG) 
            aux=memoria[valor]; 
        else   // CONSULTAR ACCESO MEMORIA
            aux=memoria[memoria[valor]];

    return aux; 

}


void set(int memoria[], int op1, int nuevovalor) {

    int tipo = (op1 >> 24) & 0xFF;

    if (tipo == REG)
        memoria[i] = nuevovalor;
    else                // CONSULTAR ACCESO MEMORIA
        memoria[memoria[i]] = nuevovalor;
}