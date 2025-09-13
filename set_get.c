#include <stdio.h>
#include <stdlib.h>
#include "const.h"

int get(int memoria[], int i, int type) {
    int aux;
    
        if (type == INST)
            aux=i;
        else if (type == REG) 
            aux=memoria[i]; 
        else
            aux=memoria[memoria[i]];

    return aux; 
}

void set(int memoria[], int i, int type, int valor) {
    if (type == REG)
        memoria[i] = valor;
    else
        memoria[memoria[i]] = valor;
}