#include <stdio.h>
#include <stdlib.h>
#include "const.h"
#include "functions.h"

void ejecucion(tMV *MV){

    int top1, top2, opcod, i,j, ipvalor, valor,datos_index;

    int limsup,liminf;
    int base;

    int datos[8];

    base = (MV->REGS[IP].dato & 0xFFFF0000)>>16;

    liminf = (MV->SEGMENTTABLE[base]&0xFFFF0000) >> 16;

    limsup = (MV->SEGMENTTABLE[base]&0xFFFF0000) >> 16;
    limsup += MV->SEGMENTTABLE[base]&0xFFFF;

    ipvalor = getdireccionfisica(MV,MV->REGS[IP].dato);

    while(ipvalor<limsup && ipvalor>=liminf) {
        datos_index=0;

        if (MV->DISSASEMBLER){
            (ipvalor == getdireccionfisica(MV, MV->REGS[CS].dato+MV->ENTRYPOINT))?printf(">"): printf(" ");
            printf("[%04X]",ipvalor);
        }

        valor=MV->MEMORIA[ipvalor];
        datos[datos_index++]=valor;

        MV->REGS[OP2].dato=MV->REGS[OP1].dato=0;

//      printf("\n\nSIGUIENTE INSTRUCCION:%5x \n",valor);
        opcod = valor & 0x1F;
        top2=(valor>>6) &0X3;
        top1=(valor >> 4) & 0x3;

        MV->REGS[OPC].dato = opcod;

            for (i=0; i < top2; i++) {
                (MV->REGS[OP2].dato)=(MV->REGS[OP2].dato<<8);
                MV->REGS[IP].dato+=1;
                valor=MV->MEMORIA[getdireccionfisica(MV,MV->REGS[IP].dato)];
                datos[datos_index++]=valor;
                MV->REGS[OP2].dato|=valor;

            }
            MV->REGS[OP2].dato |= (top2<<24);

            for ( i=0; i < top1; i++) {
                (MV->REGS[OP1].dato)=(MV->REGS[OP1].dato<<8);
                MV->REGS[IP].dato+=1;
                int direc = getdireccionfisica(MV,MV->REGS[IP].dato);
                valor=MV->MEMORIA[direc];
                datos[datos_index++]=valor;
                MV->REGS[OP1].dato|=valor;
            }
            MV->REGS[OP1].dato |= (top1<<24);

        int aux= MV->REGS[OPC].dato;
        MV->REGS[IP].dato++;

        if ( (aux >= 0 && aux<=8) || (aux>=11 && aux <= 31) ){
            if (MV->DISSASEMBLER) {
                //(ipvalor == getdireccionfisica(MV, MV->REGS[CS].dato+MV->ENTRYPOINT))?printf(">"): printf(" ");
                //printf("[%04X]%6s",ipvalor,( MV->FUNCIONES[aux]).nombre);

                mostrarhexa(datos, datos_index);

                if (aux >= 1 && aux <=7)
                    j = 1;
                else
                    j=0;
                printf("%6s",( MV->FUNCIONES[aux]).nombre);
                mostrar(MV,MV->REGS[OP1].dato,j);
                mostrar(MV,MV->REGS[OP2].dato,j);
                printf("\n");
             } else{
                MV->FUNCIONES[aux].func(MV);
            }
        }           
        else
            invalidfunction();

        ipvalor = getdireccionfisica(MV,MV->REGS[IP].dato);

        if (MV->UNPASO && ipvalor>liminf && ipvalor < limsup){
            MV->UNPASO=0;
            break;
        }


    }

}

