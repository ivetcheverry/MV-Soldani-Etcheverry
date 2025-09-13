/*EN ESTE ARCHIVO ESTÁ TODO LO RELACIONADO CON EL HEADER, Y SE APROVECHA QUE EL ARCHIVO .VMX ESTA ABIERTO PARA ESCRIBIR EL CODE SEGMENT
SE ENCUENTRAN VARIAS INICIALIZACIONES Y DECLARACIONES*/


#include <stdio.h>
#include <stdlib.h>
#include "const.h"
//#include "cons.h" //ARCHIVO CON CONSTANTES


void init_regs(int memoria[],int CSsize){
    int i=CSsize;
    for (;i<CSsize+32;i++){
        memoria[i]=0;
    }
    memoria[IP]=memoria[CS]=0x0000;
    memoria[DS]=0x0100;
}

void setSegmentTable(int tabla[][TABLA_M], int CSsize){
    tabla[0][0]=0;
    tabla[0][1]=CSsize;
    tabla[1][0]=CSsize;
    tabla[1][1]=RAM-CSsize;
    printf("\n TABLA DE SEGMENTOS: \n %5x | | %5x \n %5x | | %5x",tabla[0][0],tabla[0][1],tabla[1][0],tabla[1][1]);
}


void setCodeSegment(FILE *arch,int memoria[], int size){
    int i,aux;
    for (i=0; i<size; i++){
        fread(&aux,1,1,arch);
        printf("%3x",aux);
        memoria[i]=aux;
    }
}

void validar(int CONTROL[],int *OK, int *CSsize,int memoria[],int segmentTable[][TABLA_M]) {
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
                else { //VALIDADO
                    (*OK)=1;
                    fread(&aux,1,1,arch);(*CSsize)+=aux;
                    fread(&aux,1,1,arch);(*CSsize)+=aux;
                    setCodeSegment(arch, memoria,*CSsize);
                    setSegmentTable(segmentTable, *CSsize);
                }
            }
            fclose(arch);
        }
    else
        printf("EL ARCHIVO NO SE PUDO ABRIR!!");
}
