#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "const.h"


void mostrar(tMV *MV, int OP, int j);
void init_MV(tMV *MV, int *OK, int CONTROL[], int argsc, char *args[]);

void addsegmento(tMV *MV, int inicio, int tamano, int pos);
int buscarsegmento(tMV *MV, int puntero);

void setParamSegment(tMV *MV, int argsc, char *args[]);
void setSegmentTable(tMV *MV, FILE *arch);
void setCodeSegment(FILE *arch,tMV *MV);

int getdireccionfisica(tMV *MV, int Puntero);

void init_regs(tMV *MV);
void init_funciones(tMV *MV);

void acceso_mem(tMV *MV, int OP);
void ejecucion(tMV *MV);

int get(tMV *MV, int OP);
void set(tMV *MV, int OP, int valorNuevo);
void setCC(tMV *MV, int ultvalor);
void setsys(tMV *MV, int valorNuevo);
int getsys(tMV *MV);

void setsys_buffer(tMV *MV, char *, int);
void getsys_buffer(tMV *MV, char *, int,int );

// 0 OPERANDOS
void stop(tMV *MV);
void ret(tMV *MV);

// 1 OPERANDO
void sys(tMV *MV);
void jmp(tMV *MV);
void jz(tMV *MV);
void jnz(tMV *MV);
void jn_(tMV *MV);
void jnn(tMV *MV);
void jp(tMV *MV);
void jnp(tMV *MV);
void not_(tMV *MV);
void pop(tMV *MV);
void push(tMV *MV);
void call_(tMV *MV);


// 2 OPERANDOS
void mov(tMV *MV);
void add(tMV *MV);
void sub(tMV *MV);
void mul(tMV *MV);
void div_(tMV *MV);
void cmp(tMV *MV);
void shl(tMV *MV);
void shr(tMV *MV);
void sar(tMV *MV);
void and(tMV *MV);
void or(tMV *MV);
void xor(tMV *MV);
void swap(tMV *MV);
void ldl(tMV *MV);
void ldh(tMV *MV);
void rnd(tMV *MV);

//errores:
void segmentationfault();
void invalidfunction();
void divisionzero();
void stack_overflow();
void stack_underflow();
