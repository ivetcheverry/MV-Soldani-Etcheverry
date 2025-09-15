#include <stdio.h>
#include <stdlib.h>
#include "const.h"
#include "functions.h"

void ejecucion(int memoria[], int CSsize, tfunc_2op func2[16],tfunc_1op func1[9]){
    int top1, top2, ultimos5, bits_altos, i,j,N,M, aux;

    while(memoria[IP]<CSsize){

        bits_altos = (memoria[memoria[IP]] >> 4) & 0x000F; // desplazar 4 bits y enmascarar
        top2=(bits_altos>>2)&& 0x0003;
        top1=bits_altos && 0x0003;                    //obtengo top1 y top2

        50
        0101 0000

        if(top1==0)
            top1=top2=0;    

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
        memoria[OP2] = (N)<< 24; //SET EN OP2 (PARTE ALTA) EL TIPO

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
        }
        memoria[OP1] = (M) << 24; //SET EN OP1 (PARTE ALTA) EL TIPO
        
        memoria[OPC] = memoria[i] & 0x1F;                   //obtengo tipo de operacion y mando a operar
        
        

                               //me lo lleva directamente a 4 bytes en hexa


        int aux = 0;

        for (j = 0; j < N; j++) {
            i++;  //avanzo mem

            if (j == 0) {
            // primer byte le�do: propagar signo solo si es negativo
                if ((int)memoria[i] < 0) {
                    aux = ((int)(int)memoria[i]) << (8 * (N - 1 - j));
                }
                else {
                    aux = ((int)memoria[i]) << (8 * (N - 1 - j));
                }
            }
            else {
            // resto de bytes: siempre sin signo
            aux |= ((int)memoria[i]) << (8 * (N - 1 - j));
            }
        }
        //CARO VALOR EN OP2
        memoria[OP2] |= aux;



        aux = 0;
        for (j = 0; j < M; j++) {
            i++;  //avanzo mem

        
            
            if (j == 0) {
            // primer byte leido: propagar signo solo si es negativo
                if ((int)memoria[i] < 0) {
                    aux = ((int)(int)memoria[i]) << (8 * (M - 1 - j));
                }
                else {
                    aux = ((int)memoria[i]) << (8 * (M - 1 - j));
                }
            }
            else {
            // resto de bytes: siempre sin signo
            aux |= ((int)memoria[i]) << (8 * (M - 1 - j));
            }
        }

        memoria[OP1] |= aux;


        memoria[IP]+=((op1 & 0xFF000000)>>24)+((op2 & 0xFF000000)>>24)+0x00000001;    //actualizo Ip

        if (memoria[OPC] > 0)
            if (memoria[OPC] <= 0x08) // 1 OPERANDO
                (func1[memoria[OPC]-9]).func(memoria,op1,N);
            else if (memoria[OPC]<=0x1f) // 2 OPERANDOS
                (func2[memoria[OPC]-16]).func(memoria,op1,op2,N,M);
            else stop(memoria);

        if (memoria[IP]>=CSsize || memoria[IP]<0){               //controlo que Ip este dentro del segmento
                printf("Error, Segmentation Fault");
                break;
  }

        if (top1==3 || top2==3){
      //      acceso_mem(memoria);
        }

        memoria[IP]+=1;        //avanzo a sig op
    }

}