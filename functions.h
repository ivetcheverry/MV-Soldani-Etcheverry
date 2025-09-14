#include <stdio.h>
#include <stdlib.h>
#include "const.h"

//PROTOTIPOS A TODAS LAS FUNCIONES
void validar(int [],int *, int *, int[], int [][TABLA_M]);
void init_regs(int [],int);
int get(int memoria[], int i, int type);
void set(int memoria[], int i, int type, int valor);
void ejecucion(int memoria[], int CSsize, tfunc_2op func2[16],tfunc_1op func1[9]);
void mov(int memoria[],int a,int b, int atype, int btype);
void add(int memoria[],int a, int b, int atype, int btype);
int get(int memoria[], int i, int type);
void set(int memoria[], int i, int type, int nuevovalor);