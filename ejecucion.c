#include <stdio.h>
#include <stdlib.h>
#include "const.h"
#include "functions.h"

void ejecucion(tMV *MV){

    int top1, top2, opcod, i,j,N,M, aux, valor;

    while(MV->REGS[IP].dato<MV->CSsize && MV->REGS[IP].dato>=0) {

        valor=MV->MEMORIA[MV->REGS[IP].dato]; //TRABAJAMOS CON EL DATO DADO POR EL VALOR DE LA EXPRESION, Y LO GUARDAMOS EN UNA VARIABLE 'VALOR' PARA FACILITAR EL MANEJO
        MV->REGS[OP2].dato=MV->REGS[OP1].dato=0;    //INICIALIZO PARA NO TENER PROBLEMAS LUEGO

        printf("\n\nSIGUIENTE INSTRUCCION:%5x \n",valor); // PARA VISUALIZAR LECTURA
        opcod = valor & 0x1F;                  // ENMASCARO PARA TENER LOS ULTIMOS 5
        top2=(valor>>6) &0X3;                 //  OBTENGO TIPO DE OP2
        top1=(valor >> 4) & 0x3;              //  OBTENGO TIPO DE OP1

        /*
        if (top1 == 0) // NINGUN OPERANDO
            MV->REGS[OP2].dato=MV->REGS[OP1].dato=0;   
        else 
        {

        //IDENTIFICO TIPOS DE DATOS
        switch (top2){
            case REG:                       
                N=1;
                break;
            case INM:
                N=2;
                break;
            case MEM:
                N=3;
                break;
            default:
                N=0;
                break;
        }

        switch (top1){
            case REG:
                M=1;
                break;
            case INM:
                M=2;
                break;
            case MEM:
                M=3;
                break;
            default:
                M=0;
                break;
        }*/

        MV->REGS[OPC].dato = opcod;                   //obtengo tipo de operacion y mando a operar

//- LECTURA DE VALORES (SE UTILIZA IP) -----------------------------------------------------------------------------------------
        printf("LECTURA: ");

        for (i=0; i < top2; i++) {

            (MV->REGS[OP2].dato)=(MV->REGS[OP2].dato<<8); //MUEVO UN BYTE
            MV->REGS[IP].dato+=1;
            valor=MV->MEMORIA[MV->REGS[IP].dato];
            printf("%4x",valor);
            MV->REGS[OP2].dato|=valor;
        }
        
        if (MV->REGS[IP].dato>=MV->CSsize)
            printf("SEGMENTATION FAULT!");
        else
            MV->REGS[OP2].dato |= (top2<<24);


        for ( i=0; i < top1; i++) {

            (MV->REGS[OP1].dato)=(MV->REGS[OP1].dato<<8); //MUEVO UN BYTE
            MV->REGS[IP].dato+=1;
            valor=MV->MEMORIA[MV->REGS[IP].dato];
            printf("%4x",valor);
            MV->REGS[OP1].dato|=valor;
        }
        if (MV->REGS[IP].dato>=MV->CSsize){
            printf("SEGMENTATION FAULT!");
        }
        else 
            MV->REGS[OP1].dato |= (top1<<24);

        MV->REGS[IP].dato++;
        printf("\nOP1: %6x \n",MV->REGS[OP1].dato);
        printf("OP2: %6x \n",MV->REGS[OP2].dato);
        printf("OPC: %6x",MV->REGS[OPC].dato);
        


        int aux= MV->REGS[OPC].dato;
        if ( (aux >= 0 && aux<=8) || (aux>=15 && aux <= 31) ){ //FUNCION EXISTE?

                
                if (MV->DISSASEMBLER) {
                    printf("\n%s",( MV->FUNCIONES[aux]).nombre);
                    mostrar(MV,MV->REGS[OP1].dato,MV->REGS[OP2].dato);
                    printf("\n  ---\n");
                }
                MV->FUNCIONES[aux].func(MV);
            }
            else 
                printf("\nERROR, FUNCION NO EXISTE");     
                
                
    }
    
        if (MV->REGS[IP].dato>=MV->CSsize)          //controlo que Ip este dentro del segmento
                printf("Error, Segmentation Fault");

        /*
        if (top1==3 || top2==3){
      //      acceso_mem(memoria);
        }
        */
}

