/*EN ESTE ARCHIVO ESTÁ TODO LO RELACIONADO CON EL HEADER, Y SE APROVECHA QUE EL ARCHIVO .VMX ESTA ABIERTO PARA ESCRIBIR EL CODE SEGMENT
SE ENCUENTRAN VARIAS INICIALIZACIONES Y DECLARACIONES*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "const.h"
#include "functions.h"



void init_funciones(tMV *MV){
  strcpy(MV->FUNCIONES[SYS].nombre,  "SYS");   MV->FUNCIONES[SYS].func  = sys;
  strcpy(MV->FUNCIONES[JMP].nombre,  "JMP");   MV->FUNCIONES[JMP].func  = jmp;
  strcpy(MV->FUNCIONES[JZ].nombre,   "JZ");    MV->FUNCIONES[JZ].func   = jz;
  strcpy(MV->FUNCIONES[JP].nombre,   "JP");    MV->FUNCIONES[JP].func   = jp;
  strcpy(MV->FUNCIONES[JN_].nombre,   "JN");   MV->FUNCIONES[JN_].func   = jn_;
  strcpy(MV->FUNCIONES[JNZ].nombre,  "JNZ");   MV->FUNCIONES[JNZ].func  = jnz;
  strcpy(MV->FUNCIONES[JNP].nombre,  "JNP");   MV->FUNCIONES[JNP].func  = jnp;
  strcpy(MV->FUNCIONES[JNN].nombre,  "JNN");   MV->FUNCIONES[JNN].func  = jnn;
  strcpy(MV->FUNCIONES[NOT_].nombre, "NOT");   MV->FUNCIONES[NOT_].func = not_;

  strcpy(MV->FUNCIONES[STOP].nombre, "STOP");  MV->FUNCIONES[STOP].func = stop;

  strcpy(MV->FUNCIONES[MOV].nombre,  "MOV");   MV->FUNCIONES[MOV].func  = mov;
  strcpy(MV->FUNCIONES[ADD].nombre,  "ADD");   MV->FUNCIONES[ADD].func  = add;
  strcpy(MV->FUNCIONES[SUB].nombre,  "SUB");   MV->FUNCIONES[SUB].func  = sub;
  strcpy(MV->FUNCIONES[MUL].nombre,  "MUL");   MV->FUNCIONES[MUL].func  = mul;
  strcpy(MV->FUNCIONES[DIV_].nombre, "DIV");   MV->FUNCIONES[DIV_].func  =div_;
  strcpy(MV->FUNCIONES[CMP].nombre,  "CMP");   MV->FUNCIONES[CMP].func  = cmp;
  strcpy(MV->FUNCIONES[SHL].nombre,  "SHL");   MV->FUNCIONES[SHL].func  = shl;
  strcpy(MV->FUNCIONES[SHR].nombre,  "SHR");   MV->FUNCIONES[SHR].func  = shr;
  strcpy(MV->FUNCIONES[SAR].nombre,  "SAR");   MV->FUNCIONES[SAR].func  = sar;
  strcpy(MV->FUNCIONES[AND].nombre,  "AND");   MV->FUNCIONES[AND].func  = and;
  strcpy(MV->FUNCIONES[OR].nombre,   "OR");    MV->FUNCIONES[OR].func   = or;
  strcpy(MV->FUNCIONES[XOR].nombre,  "XOR");   MV->FUNCIONES[XOR].func  = xor;
  strcpy(MV->FUNCIONES[SWAP].nombre, "SWAP");  MV->FUNCIONES[SWAP].func = swap;
  strcpy(MV->FUNCIONES[LDL].nombre,  "LDL");   MV->FUNCIONES[LDL].func  = ldl;
  strcpy(MV->FUNCIONES[LDH].nombre,  "LDH");   MV->FUNCIONES[LDH].func  = ldh;
  strcpy(MV->FUNCIONES[RND].nombre,  "RND");   MV->FUNCIONES[RND].func  = rnd;

}


void init_regs(tMV *MV){
    int i;
    for (i=0;i<32;i++)
        MV->REGS[i].dato=0;
    MV->REGS[IP].dato = MV->REGS[CS].dato = MV->SEGMENTTABLE[0][0];
    MV->REGS[DS].dato = MV->SEGMENTTABLE[1][0];
    MV->REGS[IP].dato = MV->SEGMENTTABLE[0][0];

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

void init_MV(tMV *MV, int *OK, int CONTROL[], int VERSION, int argsc, char *args[]) {

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
                    if (argsc > 1 && strcmp(args[1], "-d") == 0) 
                        MV->DISSASEMBLER=1;
                    else
                        MV->DISSASEMBLER=0;
                    setCodeSegment(arch, MV);
                    setSegmentTable(MV);
                }
            }
            fclose(arch);
        }
    else
        printf("EL ARCHIVO NO SE PUDO ABRIR!!");
}
