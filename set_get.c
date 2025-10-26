#include <stdio.h>
#include <stdlib.h>
#include "const.h"
#include "functions.h"


int get(tMV *MV, int OP) {
    int tipo,valor=0,mask, byte_index, i;
    int regcod, offset;
    int opreg;
    int inicio,cantbytes;

    tipo = OP>>24;

    if (tipo==INM){
        valor = analizarsigno(OP & 0XFFFF,2);
        
    }else if (tipo==REG){
        regcod= OP & 0x1F;  
        valor = MV->REGS[regcod].dato;

        opreg = (OP & 0b11000000)>>6;
        switch (opreg) {
            case 0b11: //AX
                valor=valor & 0xFFFF;
                break;

            case 0b10: // AH                             
                valor=(valor & 0xFF00)>>8;        
                break;

            case 0b01:  // AL
                valor=valor & 0xFF;          
                break;

            default:   // EAX
                break;
        }

    }
     else {
        acceso_mem(MV,OP);

        cantbytes = ((MV->REGS[MAR].dato & 0xFFFF0000)>>16);
        inicio= ((MV->REGS[MAR].dato & 0xFFFF));

        for (i = inicio + 1 ; cantbytes>0; cantbytes--) {
            valor = (valor<<8) |(MV->MEMORIA[i]);
            i++;
        }

        //valor = analizarsigno(valor,cantbytes);

        MV->REGS[MBR].dato = valor;
    }

    return valor;

}


void set(tMV *MV, int OP, int valorNuevo) {
    int i, tipo, valor;
    int regcod, offset;
    int opreg, valorviejo;
    int inicio,cantbytes;

    tipo = OP >>24;

    if (tipo==REG){
        regcod= OP & 0X1F;

        valorviejo=MV->REGS[regcod].dato;
        opreg=(OP & 0b11000000)>>6;

        switch (opreg) {
            case 0b11: // AX
                valorviejo= valorviejo & 0xFFFF0000;
                valorNuevo = valorNuevo & 0xFFFF;           
                break;

            case 0b10:  // AH
                valorviejo= valorviejo & 0xFFFF00FF;
                valorNuevo= (valorNuevo << 8) & 0xFF00; 
                break;

            case 0b01: // AL
                valorviejo= valorviejo & 0xFFFFFF00;
                valorNuevo= valorNuevo & 0xFF;           
                break;

            case 0b00: //EAX
                valorviejo= 0x0;             
                break;
        }

        MV->REGS[regcod].dato = valorviejo | valorNuevo;            //queda lo viejo y pone lo nuevo en caso de no usar todo el registro
    }
    else if (tipo==MEMO){
        acceso_mem(MV,OP);

        cantbytes = ((MV->REGS[MAR].dato & 0xFFFF0000)>>16);
        inicio= ((MV->REGS[MAR].dato & 0xFFFF));

        MV->REGS[MBR].dato = valorNuevo;

        for(i= inicio + cantbytes ; cantbytes > 0; cantbytes --){
            MV->MEMORIA[i] = valorNuevo & 0xFF;
            valorNuevo = valorNuevo >> 8;
            i--;
        }
    }
    else //INMEDIATO, ERROR
        invalidfunction();
}


void setCC(tMV *MV, int ultvalor){
    int n,z;
    int cc ;
    cc = MV->REGS[CC].dato & 0x3FFFFFFF;

    if(ultvalor == 0)
        n = (1 << 30);
    else
        n = (0 << 30);

    if(ultvalor < 0)
       z = (1 << 31);
    else
       z = (0 << 31);

    MV->REGS[CC].dato  = z | n | cc;
    //printf("\nCC: %08x",MV->REGS[CC].dato);

}
