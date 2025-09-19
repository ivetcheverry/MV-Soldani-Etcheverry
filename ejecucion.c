#include <stdio.h>
#include <stdlib.h>
#include "const.h"
#include "functions.h"


void acceso_mem (tMV *MV){

    int offset, base,regcod;

    //FORMATO SEGMENT TABLE { BASE: 0000 SIZE: 002D, RESULTADO: 0x 00 00 00 2D }

    MV->REGS[LAR].dato= baseDS; 
    MV->REGS[LAR].dato= MV->REGS[LAR].dato<<16; 
    
    if (((MV->REGS[OP1].dato & 0xFF000000) >>24) == 3){  

        regcod = (MV->REGS[OP1].dato & 0xFF0000) >> 16;
        offset=  (MV->REGS[OP1].dato & 0xFFFF) + MV->REGS[regcod].dato;
    }
    else {
        if (((MV->REGS[OP2].dato & 0xFF000000) >>24) == 3){
            regcod = (MV->REGS[OP1].dato & 0xFF0000) >> 16;
            offset=  (MV->REGS[OP2].dato & 0xFFFF) + MV->REGS[regcod].dato;
        }

    }


    MV->REGS[LAR].dato |= offset;                                 //setteo LAR

    if(MV->REGS[OPC].dato==00) // SYS READ O WRITE
        MV->REGS[MAR].dato = (MV->REGS[ECX].dato & 0XFFFF0000) >> 16;
    else                                    
        MV->REGS[MAR].dato=0x4;

    MV->REGS[MAR].dato = MV->REGS[MAR].dato << 16;             

    MV->REGS[MAR].dato  |= offset;
    
    //setteo MBR en get y set

    }

    
void ejecucion(tMV *MV){

    int top1, top2, opcod, i, ipvalor, valor;
    MV->DISSASEMBLER=1;
    ipvalor = getIP(MV);
    while(ipvalor<MV->CSsize && ipvalor>=0) {

        valor=MV->MEMORIA[ipvalor];                 //TRABAJAMOS CON EL DATO DADO POR EL VALOR DE LA EXPRESION, Y LO GUARDAMOS EN UNA VARIABLE 'VALOR' PARA FACILITAR EL MANEJO
        MV->REGS[OP2].dato=MV->REGS[OP1].dato=0;    //INICIALIZO PARA NO TENER PROBLEMAS LUEGO

//        printf("\n\nSIGUIENTE INSTRUCCION:%5x \n",valor);         // PARA VISUALIZAR LECTURA
        opcod = valor & 0x1F;                  // ENMASCARO PARA TENER LOS ULTIMOS 5
        top2=(valor>>6) &0X3;                 //  OBTENGO TIPO DE OP2
        top1=(valor >> 4) & 0x3;              //  OBTENGO TIPO DE OP1

        MV->REGS[OPC].dato = opcod;                   //obtengo tipo de operacion y mando a operar

//- LECTURA DE VALORES (SE UTILIZA IP) -----------------------------------------------------------------------------------------

//
        
            for (i=0; i < top2; i++) {
                (MV->REGS[OP2].dato)=(MV->REGS[OP2].dato<<8); //MUEVO UN BYTE
                MV->REGS[IP].dato+=1;
                valor=MV->MEMORIA[MV->REGS[IP].dato];
 //               printf("%4x",valor);
                MV->REGS[OP2].dato|=valor;
            }
            MV->REGS[OP2].dato |= (top2<<24);


            for ( i=0; i < top1; i++) {

                (MV->REGS[OP1].dato)=(MV->REGS[OP1].dato<<8); //MUEVO UN BYTE
                MV->REGS[IP].dato+=1;
                valor=MV->MEMORIA[MV->REGS[IP].dato];
 //               printf("%4x",valor);
                MV->REGS[OP1].dato|=valor;
            }
            MV->REGS[OP1].dato |= (top1<<24);


        MV->REGS[IP].dato++;

        if (top1==3 || top2==3)
            acceso_mem(MV);
        
        int aux= MV->REGS[OPC].dato;
        if ( (aux >= 0 && aux<=8) || (aux>=15 && aux <= 31) ){ //FUNCION EXISTE?
            if (MV->DISSASEMBLER){
                printf("\n[%04X] %s",ipvalor,( MV->FUNCIONES[aux]).nombre);
                mostrar(MV,MV->REGS[OP1].dato,MV->REGS[OP2].dato);
            }
            MV->FUNCIONES[aux].func(MV);
        }
        else
            invalidfunction();

         
        
        ipvalor = getIP(MV);

    } //FIN DEL WHILE EJECUCION;


}

