#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "const.h"

//PROTOTIPOS A TODAS LAS FUNCIONES

void mostrar(tMV *MV, int OPA, int OPB);
void init_MV(tMV *MV, int *OK, int CONTROL[], int VERSION, int argsc, char *args[]);

void setSegmentTable(tMV *MV);
void setCodeSegment(FILE *arch,tMV *MV);

void init_regs(tMV *MV);
void init_funciones(tMV *MV);

void ejecucion(tMV *MV);

int get(tMV *MV, int OP);
int getIP (tMV *MV);
void set(tMV *MV, int OP, int valorNuevo);
void setCC(tMV *MV, int ultvalor);
void setsys(tMV *MV, int valorNuevo);
int getsys(tMV *MV);

void sys(tMV *MV);
void jmp(tMV *MV);
void jz(tMV *MV);
void jnz(tMV *MV);
void jn_(tMV *MV);
void jnn(tMV *MV);
void jp(tMV *MV);
void jnp(tMV *MV);
void not_(tMV *MV);

void mov(tMV *MV);
void add(tMV *MV);
void sub(tMV *MV);
void mul(tMV *MV);
void div_(tMV *MV);
void cmp(tMV *MV);
void shl(tMV *MV);
void shr(tMV *MV);

void stop(tMV *MV);

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