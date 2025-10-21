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

  strcpy(MV->FUNCIONES[PUSH].nombre,  "PUSH");   MV->FUNCIONES[RND].func  = push;
  strcpy(MV->FUNCIONES[POP].nombre,  "POP");   MV->FUNCIONES[RND].func  = pop;
  strcpy(MV->FUNCIONES[CALL_].nombre,  "CALL");   MV->FUNCIONES[RND].func  = call_;
  strcpy(MV->FUNCIONES[RET].nombre,  "RET");   MV->FUNCIONES[RND].func  = ret;

}


void init_regs(tMV *MV){
    int i;
    for (i=0;i<32;i++)
        MV->REGS[i].dato=0;

    strcpy(MV->REGS[LAR].nombre, "LAR");
    strcpy(MV->REGS[MAR].nombre, "MAR");
    strcpy(MV->REGS[MBR].nombre, "MBR");

    strcpy(MV->REGS[IP].nombre, "IP");
    strcpy(MV->REGS[OPC].nombre, "OPC");
    strcpy(MV->REGS[OP1].nombre, "OP1");
    strcpy(MV->REGS[OP2].nombre, "OP2");

    //PILA
    strcpy(MV->REGS[SP].nombre, "SP");
    strcpy(MV->REGS[BP].nombre, "BP");

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

// PARA VERSION 1: USAR ESTA FUNCION
void setSegmentTable(tMV *MV, FILE *arch){

    //FORMATO SEGMENT TABLE { BASE: 0000 SIZE: 002D, RESULTADO: 0x0000002D }

    int aux, aux2=0;

    fread(&aux,1,1,arch); aux2+=aux;
    fread(&aux,1,1,arch); aux2+=aux;

    MV->SEGMENTTABLE[0] = 0;
    MV->SEGMENTTABLE[0] |= aux2;
    MV->SEGMENTTABLE[1] = (aux2)<<16;
    MV->SEGMENTTABLE[1] |= (RAM-aux2);



    //printf("\n\n TABLA DE SEGMENTOS: \n\n %5x | | %5x \n %5x | | %5x",(MV->SEGMENTTABLE[0]>>16) & 0xFFFF, (MV->SEGMENTTABLE[0])&0xFFFF, (MV->SEGMENTTABLE[1]>>16) & 0xFFFF, (MV->SEGMENTTABLE[1])&0xFFFF);

}

void setCodeSegment(FILE *arch, tMV *MV) { 
    // vuelca el codigo al code segment
    int i,aux,posCS;
    posCS = buscarCS();
    for (i=0; i<posCS ;i++){
        fread(&aux,1,1,arch);
        /*
        printf("%3x",aux);
        if (i>0 && i%15 == 0)       //escribe lo que lee del .vmx
            printf("\n");
        */
        MV->MEMORIA[i]=aux;
    }
}


void setTabla(tMV *MV, FILE *arch, int sizePS, int PSpointer ){
    int aux,j,cant_segmentos=0;
    int tam,inicio=0;
    if ((sizePS)>0){           
        MV->SEGMENTTABLE[cant_segmentos] = inicio|((sizePS));
        inicio += (sizePS);
        MV->REGS[PS].dato = 0|(PSpointer);
        cant_segmentos++; //cada vez que se agrega un segmento, se aumenta cant_segmentos en 1
    }

    if (MV->ver==1)
        setSegmentTable(MV,arch);
    else
    {
        //ESTOS FREAD LEERAN EL HEADER DEL VMX, Y EN CADA REGISTRO GUARDARÁ TEMPORALMENTE EL TAMAÑO DE CADA SEGMENTO

            MV->REGS[IP].dato = MV->REGS[CS].dato =MV->REGS[DS].dato =MV->REGS[ES].dato =MV->REGS[SS].dato = MV->REGS[KS].dato = 0;

            fread(&aux,1,1,arch); MV->REGS[CS].dato += aux;
            fread(&aux,1,1,arch); MV->REGS[CS].dato += aux;

            fread(&aux,1,1,arch); MV->REGS[DS].dato += aux;
            fread(&aux,1,1,arch); MV->REGS[DS].dato += aux;

            fread(&aux,1,1,arch); MV->REGS[ES].dato += aux;
            fread(&aux,1,1,arch); MV->REGS[ES].dato += aux;

            fread(&aux,1,1,arch); MV->REGS[SS].dato += aux;
            fread(&aux,1,1,arch); MV->REGS[SS].dato += aux;

            fread(&aux,1,1,arch); MV->REGS[KS].dato += aux;
            fread(&aux,1,1,arch); MV->REGS[KS].dato += aux;

        //ANALIZAMOS UNO POR UNO SI LOS SEGMENTOS EXISTEN, DE SER ASI SE AGREGAN A LA TABLA, SE CALCULA SU DIRECCION LOGICA    

            if (MV->REGS[KS].dato <= 0) 
                MV->REGS[KS].dato = -1;
            else {
                MV->SEGMENTTABLE[cant_segmentos] = (inicio<<16)||MV->REGS[KS].dato;
                inicio += MV->REGS[KS].dato;
                MV->REGS[KS].dato = cant_segmentos<<16;
                cant_segmentos++;
            }

            if (MV->REGS[CS].dato <= 0) 
                MV->REGS[IP].dato = MV->REGS[CS].dato = -1;
            else {
                MV->SEGMENTTABLE[cant_segmentos] = (inicio<<16)||MV->REGS[CS].dato;
                inicio += MV->REGS[CS].dato;
                MV->REGS[IP].dato = MV->REGS[CS].dato = cant_segmentos<<16;
                cant_segmentos++;
            }

            if (MV->REGS[DS].dato <= 0) 
                MV->REGS[DS].dato = -1;
            else {
                MV->SEGMENTTABLE[cant_segmentos] = (inicio<<16)||MV->REGS[DS].dato;
                inicio += MV->REGS[DS].dato;
                MV->REGS[DS].dato = cant_segmentos<<16;
                cant_segmentos++;
            }

            if (MV->REGS[ES].dato <= 0) 
                MV->REGS[ES].dato = -1;
            else {
                MV->SEGMENTTABLE[cant_segmentos] = (inicio<<16)||MV->REGS[ES].dato;
                inicio += MV->REGS[ES].dato;
                MV->REGS[ES].dato = cant_segmentos<<16;
                cant_segmentos++;
            }
            
            if (MV->REGS[SS].dato <= 0) 
                MV->REGS[SS].dato = -1;
            else {
                MV->SEGMENTTABLE[cant_segmentos] = (inicio<<16)||MV->REGS[SS].dato; 
                inicio += MV->REGS[SS].dato;
                MV->REGS[SP].dato = (cant_segmentos<<16)||inicio+1; //SP APUNTA INICIALMENTE FUERA DEL STACK SEGMENT
                MV->REGS[SS].dato = cant_segmentos<<16;
            }
            
            //LECTURA ENTRY POINT 

            fread(&aux,1,1,arch); MV->REGS[IP].dato += aux;
            fread(&aux,1,1,arch); MV->REGS[IP].dato += aux;

        }
}

    

void setParamSegment(tMV *MV, int argsc, char *args[], int *sizePS, int *PSpointer){
    int j,w,pos,i = 0;
    int aux = argsc;
    int offsets[100];
    while (i< argsc && args[i]!="-p"){
        i++;
        aux--;
    }
    if (args[i]!="-p") {
        i++;
        pos = 0;
        for (w=0;w<aux;w++) {
            offsets[w]=pos;
            j=0;
            while (j != '\0') { 
                MV->MEMORIA[pos] = args[i][j]; //accedo a la palabra i, y a la letra j
                pos++; j++;
            }
            MV->MEMORIA[pos]='\0';
            pos++;
        }
        (*PSpointer)=pos;
        for (w=0;w<aux;w++){
            for (i=0;i<2;i++){
            MV->MEMORIA[pos]=0x00;
            pos++;
            }
            MV->MEMORIA[pos]= (offsets[w] && 0x0000FF00)>>8;
            pos++;
            MV->MEMORIA[pos]=(offsets[w] && 0xFF);
            pos++;
        }
    }
    (*sizePS)=pos;
}


void init_MV(tMV *MV, int *OK, int CONTROL[], int VERSION1,int VERSION2, int argsc, char *args[]){//,char ANOMBREARCHIVO[]) {

    FILE *arch = fopen(NOMBREARCHIVO,"rb");
    int aux,i=0;
    int sizePS=0;
    int PSpointer=0;
    if (arch){
        printf("\n");
            fread(&aux,1,1,arch); //CONTROLO LOS CARACTERES "VMX25"
            while (i<5 && aux-CONTROL[i]==0 ) {
                //printf("%3c",aux);
                i++;
                fread(&aux,1,1,arch);
            }
            if (i<5){
                printf("\n ERROR, ARCHIVO NO VALIDO!");
            }   
            else{ //CONTROLO VERSIONES
                //printf("\n VERSION: %d \n\n", aux);
                if (aux!=VERSION1 && aux !=VERSION2)
                    printf("\n VERSION NO SOPORTADA!");
                else { //VALIDO
                    MV->ver=aux;
                    (*OK)=1;
                    
                    setParamSegment(MV,argsc,args,&sizePS, &PSpointer);
                    setTabla(MV,arch,sizePS,PSpointer);
                    if (argsc > 2 && strcmp(args[2], "-d") == 0)
                        MV->DISSASEMBLER=1;
                    else
                        MV->DISSASEMBLER=0;
                    setCodeSegment(arch, MV);
                }
            }
            fclose(arch);
        }
    else
        printf("EL ARCHIVO NO SE PUDO ABRIR!!");
}
