/*EN ESTE ARCHIVO ESTÁ TODO LO RELACIONADO CON EL HEADER, Y SE APROVECHA QUE EL ARCHIVO .VMX ESTA ABIERTO PARA ESCRIBIR EL CODE SEGMENT
SE ENCUENTRAN VARIAS INICIALIZACIONES Y DECLARACIONES*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "const.h"
#include "functions.h"



void init_funciones(tMV *MV){
  tfunc *f = MV->FUNCIONES;
  strcpy(f[SYS].nombre,  "SYS");   f[SYS].func  = sys;
  strcpy(f[JMP].nombre,  "JMP");   f[JMP].func  = jmp;
  strcpy(f[JZ].nombre,   "JZ");    f[JZ].func   = jz;
  strcpy(f[JP].nombre,   "JP");    f[JP].func   = jp;
  strcpy(f[JN_].nombre,   "JN");   f[JN_].func   = jn_;
  strcpy(f[JNZ].nombre,  "JNZ");   f[JNZ].func  = jnz;
  strcpy(f[JNP].nombre,  "JNP");   f[JNP].func  = jnp;
  strcpy(f[JNN].nombre,  "JNN");   f[JNN].func  = jnn;
  strcpy(f[NOT_].nombre, "NOT");   f[NOT_].func = not_;

  strcpy(f[STOP].nombre, "STOP");  f[STOP].func = stop;

  strcpy(f[MOV].nombre,  "MOV");   f[MOV].func  = mov;
  strcpy(f[ADD].nombre,  "ADD");   f[ADD].func  = add;
  strcpy(f[SUB].nombre,  "SUB");   f[SUB].func  = sub;
  strcpy(f[MUL].nombre,  "MUL");   f[MUL].func  = mul;
  strcpy(f[DIV_].nombre, "DIV");   f[DIV_].func  = div_;
  strcpy(f[CMP].nombre,  "CMP");   f[CMP].func  = cmp;
  strcpy(f[SHL].nombre,  "SHL");   f[SHL].func  = shl;
  strcpy(f[SHR].nombre,  "SHR");   f[SHR].func  = shr;
 /* strcpy(f[SAR].nombre,  "SAR");   f[SAR].func  = sar;
  strcpy(f[AND].nombre,  "AND");   f[AND].func  = and;
  strcpy(f[OR].nombre,   "OR");    f[OR].func   = or;
  strcpy(f[XOR].nombre,  "XOR");   f[XOR].func  = xor;
  strcpy(f[SWAP].nombre, "SWAP");  f[SWAP].func = swap;
  strcpy(f[LDL].nombre,  "LDL");   f[LDL].func  = ldl;
  strcpy(f[LDH].nombre,  "LDH");   f[LDH].func  = ldh;
  strcpy(f[RND].nombre,  "RND");   f[RND].func  = rnd;*/
}


void init_regs(tMV *MV){
    int i;
    for (i=0;i<32;i++){
        MV->REGS[i].dato=0;
    }
    MV->REGS[IP].dato = MV->REGS[CS].dato=MV->SEGMENTTABLE[0][0];
    MV->REGS[DS].dato = MV->SEGMENTTABLE[1][0];

    strcpy(MV->REGS[LAR].nombre, "LAR");
    strcpy(MV->REGS[MAR].nombre, "MAR");
    strcpy(MV->REGS[MBR].nombre, "MBR");

    strcpy(MV->REGS[IP].nombre, "IP");
    strcpy(MV->REGS[OPC].nombre, "OPC");
    strcpy(MV->REGS[OP1].nombre, "OP1");
    strcpy(MV->REGS[OP2].nombre, "OP2");

    // Bloque de registros generales
    strcpy(MV->REGS[EAX].nombre, "EAX");
    strcpy(MV->REGS[EBX].nombre, "EBX");
    strcpy(MV->REGS[ECX].nombre, "ECX");
    strcpy(MV->REGS[EDX].nombre, "EDX");
    strcpy(MV->REGS[EEX].nombre, "EEX");
    strcpy(MV->REGS[EFX].nombre, "EFX");

    // Acumulador y código de condición
    strcpy(MV->REGS[AC].nombre, "AC");
    strcpy(MV->REGS[CC].nombre, "CC");

    // Segmentos
    strcpy(MV->REGS[CS].nombre, "CS");
    strcpy(MV->REGS[DS].nombre, "DS");


}

void setSegmentTable(tMV *MV){
    MV->SEGMENTTABLE[0][0]=0;
    MV->SEGMENTTABLE[0][1]=MV->CSsize;
    MV->SEGMENTTABLE[1][0]=MV->CSsize;
    MV->SEGMENTTABLE[1][1]=RAM-(MV->CSsize);
    printf("\n TABLA DE SEGMENTOS: \n %5x | | %5x \n %5x | | %5x",MV->SEGMENTTABLE[0][0],MV->SEGMENTTABLE[0][1],MV->SEGMENTTABLE[1][0],MV->SEGMENTTABLE[1][1]);
}


void setCodeSegment(FILE *arch,tMV *MV){
    int i,aux;
    for (i=0; i<MV->CSsize ;i++){
        fread(&aux,1,1,arch);
        printf("%3x",aux);
        MV->MEMORIA[i]=aux;
    }
}

void init_MV(tMV *MV, int *OK, int CONTROL[], int VERSION) {

    FILE *arch = fopen("sample.vmx","rb");
    int aux,i=0;
    if (arch){
            fread(&aux,1,1,arch); //CONTROLO LOS CARACTERES "VMX25"
            while (i<5 && aux-CONTROL[i]==0 ) {
                printf("%3c",aux);
                i++;
                fread(&aux,1,1,arch);
            }
            if (i<5){
                printf("\n ERROR, ARCHIVO NO VALIDO!");
            }
            else{//CONTROLO VERSION
                printf("\n VERSION: %d \n", aux);
                if (aux!=VERSION)
                    printf("\n VERSION NO SOPORTADA!");
                else { //VALIDO

                    (*OK)=1; 
                    fread(&aux,1,1,arch);(MV->CSsize)+=aux;
                    fread(&aux,1,1,arch);(MV->CSsize)+=aux;
                    setCodeSegment(arch, MV);
                    setSegmentTable(MV);
                }
            }
            fclose(arch);
        }
    else
        printf("EL ARCHIVO NO SE PUDO ABRIR!!");
}
