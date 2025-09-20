#include <stdio.h>
#include <stdlib.h>
#include "const.h"
#include "functions.h"



int get(tMV *MV, int OP) {
    int tipo,valor=0,mask, byte_index, i;
    int regcod, offset; // REGCOD REPRESENTADO POR 5 BITS Y EL OFFSET POR 2 BYTES.
    int inicio,cantbytes;

    tipo = OP>>24;      //ME QUEDO CON EL BYTE MAS SIGNIFICATIVO

    if (tipo==INM)
        valor = OP &0XFFFF; // INMEDIATO: VALOR DE LOS ULTIMOS 2 BYTES
    else if (tipo==REG){
        regcod= OP & 0x1F;
        valor = MV->REGS[regcod].dato;

    } else {

        cantbytes = ((MV->REGS[MAR].dato & 0xFFFF0000)>>16);
        inicio= ((MV->REGS[MAR].dato & 0xFFFF));

        for (i = inicio ; i < inicio+cantbytes; i++) {
            valor = (valor << 8) | (MV->MEMORIA[i]);
        }

        valor = MV->REGS[MBR].dato;
    }

    //ANALIZO SI ES NEGATIVO:
    if (valor & 0b1000000){
        valor= (valor ^ NMASK) - NMASK;
    }


    return valor;
}


int getIP (tMV *MV) {
    int base, offset,valor;

    base = (MV->REGS[IP].dato >> 16) & 0xFFFF;
    offset = (MV->REGS[IP].dato) & 0xFFFF;

    valor = (MV->SEGMENTTABLE[base]>>16 &0xFFFF) + offset;

    if (valor & 0b1000000){
        valor= (valor ^ NMASK) - NMASK;
    }

    return valor;

}


void set(tMV *MV, int OP, int valorNuevo) {
    int i, byte_index, tipo, valor;
    int regcod, offset;
    int inicio,cantbytes;

    tipo = OP >>24;

    if (tipo==REG){
        regcod= OP & 0X1F;
        MV->REGS[regcod].dato = valorNuevo;
    }
    else if (tipo==MEMO){
        byte_index=0;

        cantbytes = ((MV->REGS[MAR].dato & 0xFFFF0000)>>16);
        inicio= ((MV->REGS[MAR].dato & 0xFFFF));

        MV->REGS[MBR].dato = valorNuevo;

        for(i= inicio ; i < inicio+cantbytes; i++){

            MV->MEMORIA[i]= (valorNuevo >> (8 * (3 - byte_index))) & 0xFF;
            byte_index ++;
        }
    }
    else //INMEDIATO, ERROR
        invalidfunction();
}

int getsys(tMV *MV) {
    int i, tipo, valor=0;
    int inicio,cantbytes;

        cantbytes = ((MV->REGS[MAR].dato & 0xFFFF0000)>>16);
        inicio= ((MV->REGS[MAR].dato & 0xFFFF));

        if ( (inicio+cantbytes) >= (MV->SEGMENTTABLE[baseDS]&0xFFFF) )
            segmentationfault();

        else {
            for(i = inicio ; cantbytes > 0; cantbytes--){
                valor = valor<<8;
                valor |= MV->MEMORIA[i];
                i++;
            }
        }

    MV->REGS[MBR].dato = valor;
    return valor;
    
}


void setsys(tMV *MV, int valorNuevo) {
    int i, tipo, valor;
    int inicio,cantbytes;

        cantbytes = ((MV->REGS[MAR].dato & 0xFFFF0000)>>16);
        inicio= ((MV->REGS[MAR].dato & 0xFFFF))-1;

        if ( (inicio+cantbytes) >= (MV->SEGMENTTABLE[baseDS]&0xFFFF) )
            segmentationfault();

        else {

        MV->REGS[MBR].dato = valorNuevo;
        for(i= inicio + cantbytes ; cantbytes > 0; cantbytes --){
            MV->MEMORIA[i] = valorNuevo & 0xFF;   
            valorNuevo = valorNuevo >> 8;         
            i--;
        }

    }

}



void setCC(tMV *MV, int ultvalor){
    int n,z;
    int cc ;
    cc = MV->REGS[CC].dato & 0x3FFFFFFF; //los primeros 2 bits quedan en 0, lo demas igual.

    if(ultvalor == 0)
        n = (1 << 30);
    else
        n = (0 << 30);

    if(ultvalor < 0)
       z = (1 << 31);
    else
       z = (0 << 31);

    MV->REGS[CC].dato  = z | n | cc;

}
