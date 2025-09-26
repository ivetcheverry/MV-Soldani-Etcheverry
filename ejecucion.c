#include <stdio.h>
#include <stdlib.h>
#include "const.h"
#include "functions.h"


void acceso_mem (tMV *MV, int OP){


    int offset=0,regcod, sys=0;
    MV->REGS[LAR].dato= baseDS; 
    MV->REGS[LAR].dato= MV->REGS[LAR].dato<<16; 

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

    MV->REGS[LAR].dato |= offset + (MV->REGS[regcod].dato & 0XFFFF); 

    if(sys)
        MV->REGS[MAR].dato = (MV->REGS[ECX].dato & 0XFFFF0000) >> 16;
    else                                    
        MV->REGS[MAR].dato=0x4;

    MV->REGS[MAR].dato = MV->REGS[MAR].dato << 16;  
    MV->REGS[MAR].dato |= (((MV->SEGMENTTABLE[ ( MV->REGS[LAR].dato>>16) & 0xFFFF])>>16) &0xFFFF) + (MV->REGS[LAR].dato & 0XFFFF);

    /*
    printf("\n");
    printf("LAR %08x \n MAR %08x", MV->REGS[LAR].dato, MV->REGS[MAR].dato);
    printf("\n");
    */

}

    
void ejecucion(tMV *MV){

    int top1, top2, opcod, i,j, ipvalor, valor;
    ipvalor = getIP(MV);
    while(ipvalor<MV->CSsize && ipvalor>=0) {
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
                valor=MV->MEMORIA[MV->REGS[IP].dato];
                MV->REGS[OP2].dato|=valor;
            }
            MV->REGS[OP2].dato |= (top2<<24);

            for ( i=0; i < top1; i++) {

                (MV->REGS[OP1].dato)=(MV->REGS[OP1].dato<<8);
                MV->REGS[IP].dato+=1;
                valor=MV->MEMORIA[MV->REGS[IP].dato];
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


        ipvalor = getIP(MV);     
    }


}

