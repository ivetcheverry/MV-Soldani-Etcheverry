//ARCHIVO CON INICIALIZACIONES,SETTEOS,ETC
#include <stdio.h>
#include <stdlib.h>
#include "cons.h"
//#include "cons.h" //ARCHIVO CON CONSTANTES


void init_regs(int memoria[],int CSsize){
    int i=CSsize;
    for (;i<CSsize+32;i++){
        memoria[i]=0;
    }
}

void setCodeSegment(FILE *arch,int memoria[], int size){
    int i,aux;
    for (i=0; i<size; i++){
        fread(&aux,1,1,arch);
        printf("%3x",aux);
        memoria[i]=aux;
    }
}

void validar(int CONTROL[],int *OK, int *CSsize,int memoria[]) {
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
                }
            }
            fclose(arch);
        }
    else
        printf("EL ARCHIVO NO SE PUDO ABRIR!!");
}
