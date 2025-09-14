#include <stdio.h>
#include <stdlib.h>
#include "const.h"
#include "functions.h"


int get(int memoria[], int i, int type) {
    int aux;
    
        if (type == INM)
            aux=i;
        else if (type == REG) 
            aux=memoria[i]; 
        else
            aux=memoria[memoria[i]];

    return aux; 
}

void set(int memoria[], int i, int type, int nuevovalor) {
    if (type == REG)
        memoria[i] = nuevovalor;
    else
        memoria[memoria[i]] = nuevovalor;
}