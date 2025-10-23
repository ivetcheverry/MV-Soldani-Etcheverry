#include <stdio.h>
#include <stdlib.h>
#include "const.h"
#include "functions.h"


int get_tipo_mem(int OP){
    int valor=4;

    OP=(OP&0x00FF0000)>>22; //dos bits mas significativos del codigo de memoria
    valor-=OP;

    return valor;
}

void acceso_mem (tMV *MV, int OP){
    int offset=0,regcod,base, sys=0;
    int cantbytes=0; int inicio=0;
    int base_segmento, size_segmento;


    if ( ( (OP & 0xFF000000) >>24 ) == 3) {
        regcod = (OP & 0xFF0000) >> 16;
        offset=  (OP & 0xFFFF);
        if (offset & NMASK16)
        offset = (offset ^ NMASK16) - NMASK16;
    }
    else {      //SYS
        regcod = EDX;
        sys=1;
    }

    base = ((MV->REGS[regcod].dato>>16 ) &0xFFFF);

    if (base>=0 && base <= (MV->REGS[SS].dato >>16)) { //ya que siempre habra SS
        MV->REGS[LAR].dato = base;
        MV->REGS[LAR].dato = MV->REGS[LAR].dato<<16;
        MV->REGS[LAR].dato |= ((offset + (MV->REGS[regcod].dato & 0XFFFF))&0xffff);
    } else
        segmentationfault();

    if(sys)
        MV->REGS[MAR].dato = (MV->REGS[ECX].dato & 0XFFFF0000) >> 16;
    else
        MV->REGS[MAR].dato = 0x0 | get_tipo_mem(MV->REGS[OP2].dato);    //obtengo cantidad de bytes a escribir/leer en memoria

    MV->REGS[MAR].dato = MV->REGS[MAR].dato << 16;

    MV->REGS[MAR].dato |= ( ( (MV->SEGMENTTABLE[( MV->REGS[LAR].dato>>16) & 0xFFFF])>>16 ) &0xFFFF) + (MV->REGS[LAR].dato & 0XFFFF );

    cantbytes = ((MV->REGS[MAR].dato & 0xFFFF0000)>>16);
    inicio = ((MV->REGS[MAR].dato & 0xFFFF));


    if  (inicio+cantbytes > (MV->SEGMENTTABLE[base]&0xFFFF)  +  ((MV->SEGMENTTABLE[base]>>16)&0xFFFF) || (inicio < ((MV->SEGMENTTABLE[base]>>16)&0xFFFF)) )
        segmentationfault();



    /*
    printf("\n");
    printf("LAR %08x \n MAR %08x", MV->REGS[LAR].dato, MV->REGS[MAR].dato);
    printf("\n");
    */

}


void ejecucion(tMV *MV){

    int top1, top2, opcod, i,j, ipvalor, valor;

    int limsup,liminf;
    int base;
    
    base = (MV->REGS[IP].dato & 0xFFFF0000)>>16;

    liminf = (MV->SEGMENTTABLE[base]&0xFFFF0000) >> 16;

    limsup = (MV->SEGMENTTABLE[base]&0xFFFF0000) >> 16;
    limsup += MV->SEGMENTTABLE[base]&0xFFFF;
    

    ipvalor = getdireccionfisica(MV,MV->REGS[IP].dato);
    MV->DISSASEMBLER=1;

    while(ipvalor<limsup && ipvalor>=liminf) {
        valor=MV->MEMORIA[ipvalor];
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
                MV->REGS[OP2].dato|=valor;
            }
            MV->REGS[OP2].dato |= (top2<<24);

            for ( i=0; i < top1; i++) {

                (MV->REGS[OP1].dato)=(MV->REGS[OP1].dato<<8);
                MV->REGS[IP].dato+=1;
                valor=MV->MEMORIA[getdireccionfisica(MV,MV->REGS[IP].dato)];
                MV->REGS[OP1].dato|=valor;
            }
            MV->REGS[OP1].dato |= (top1<<24);

        int aux= MV->REGS[OPC].dato;
        MV->REGS[IP].dato++;

        if ( (aux >= 0 && aux<=8) || (aux>=15 && aux <= 31) ){
            if (MV->DISSASEMBLER) {
                printf("\n[%04X] %4s",ipvalor,( MV->FUNCIONES[aux]).nombre);

                if (aux >= 1 && aux <=7)
                    j = 1;
                else
                    j=0;

                mostrar(MV,MV->REGS[OP1].dato,j);
                mostrar(MV,MV->REGS[OP2].dato,j);
            } else
                MV->FUNCIONES[aux].func(MV);
                /*printf("\nOP1: %0x", MV->REGS[OP1].dato);
                printf("\nOP2: %0x\n", MV->REGS[OP2].dato);
                printf("\nDS %08x", MV->REGS[DS].dato);
                printf("\nEDX %08x", MV->REGS[EDX].dato);
                printf("\nECX %08x", MV->REGS[ECX].dato);*/

        }
        else
            invalidfunction();

        ipvalor = getdireccionfisica(MV,MV->REGS[IP].dato);
    }


}

