#include <stdio.h>
#include <stdlib.h>
#include "const.h"
#include "functions.h"

void add(int memoria[],int a, int b, int atype, int btype){
    if (atype==REG)
        if (btype==INM) //tipo INSTANTANEO
            memoria[a]+=b;
        else if (btype==REG) //tipo REGISTRO
            memoria[a]+=memoria[b];
        else //tipo MEMORIA
            memoria[a]+=memoria[memoria[b]];
    
    else //es tipo Memoria
        if (btype==INM) //tipo INSTANTANEO
            memoria[memoria[a]]+=b;
        else if (btype==REG) //tipo REGISTRO
            memoria[memoria[a]]+=memoria[b];
        else //tipo MEMORIA
            memoria[memoria[a]]+=memoria[memoria[b]];
}

void mov(int memoria[],int a,int b, int atype, int btype){
    set(memoria,a,atype,get(memoria,b,btype));
}