#include <stdio.h>
#include <stdlib.h>
#include "const.h"
#include "functions.h"


int get(tMV *MV, int OP) {
    int tipo,valor=0,mask, byte_index, i;
    int regcod, offset;
    int inicio,cantbytes;

    tipo = OP>>24;

    if (tipo==INM){
        valor = OP &0XFFFF;
        if (valor & NMASK16)
        valor = (valor ^ NMASK16) - NMASK16;
    }else if (tipo==REG){
        regcod= OP & 0x1F;
        valor = MV->REGS[regcod].dato;
    }
     else {
        acceso_mem(MV,OP);
        cantbytes = ((MV->REGS[MAR].dato & 0xFFFF0000)>>16);
        inicio= ((MV->REGS[MAR].dato & 0xFFFF));

        /*if ( (inicio+cantbytes) >= (MV->SEGMENTTABLE[baseDS]&0xFFFF))
            segmentationfault();*/

        for (i = inicio ; i < inicio+cantbytes; i++) {
            valor = (valor << 8) | (MV->MEMORIA[i]);
        }

       // Extensi�n de signo autom�tica seg�n el tama�o le�do
        int signo_mask = 1 << ((cantbytes * 8) - 1);
        int extend_mask = ~((1 << (cantbytes * 8)) - 1);
        if (valor & signo_mask)
            valor |= extend_mask;

        MV->REGS[MBR].dato = valor;
    }

    return MV->REGS[MBR].dato;

}


int getIP (tMV *MV) {
    int base, offset,valor;

    base = (MV->REGS[IP].dato >> 16) & 0xFFFF;
    offset = (MV->REGS[IP].dato) & 0xFFFF;

    valor = (MV->SEGMENTTABLE[base]>>16 &0xFFFF) + offset;

    if (valor & NMASK16)
        valor= (valor ^ NMASK16) - NMASK16;

    return valor;

}


void set(tMV *MV, int OP, int valorNuevo) {
    int i, tipo, valor;
    int regcod, offset;
    int inicio,cantbytes;

    tipo = OP >>24;

    if (tipo==REG){
        regcod= OP & 0X1F;
        MV->REGS[regcod].dato = valorNuevo;
    }
    else if (tipo==MEMO){
        acceso_mem(MV,OP);
        cantbytes = ((MV->REGS[MAR].dato & 0xFFFF0000)>>16);
        inicio= ((MV->REGS[MAR].dato & 0xFFFF))-1;

        /*if ( (inicio+cantbytes) >= (MV->SEGMENTTABLE[baseDS]&0xFFFF))
            segmentationfault();*/

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

int getsys(tMV *MV) {
    int i, tipo, valor=0;
    int inicio,cantbytes;

        cantbytes = ((MV->REGS[MAR].dato & 0xFFFF0000)>>16);
        inicio= ((MV->REGS[MAR].dato & 0xFFFF));

        /*if ( (inicio+cantbytes) >= (MV->SEGMENTTABLE[baseDS]&0xFFFF))
            segmentationfault();*/

            for(i = inicio ; cantbytes > 0; cantbytes--){
                valor = valor<<8;
                valor |= MV->MEMORIA[i];
                i++;
            }



    MV->REGS[MBR].dato = valor;
    return MV->REGS[MBR].dato;

}


void setsys(tMV *MV, int valorNuevo) {
    int i, tipo, valor;
    int inicio,cantbytes;

        cantbytes = ((MV->REGS[MAR].dato & 0xFFFF0000)>>16);
        inicio= ((MV->REGS[MAR].dato & 0xFFFF))-1;

        /*if ( (inicio+cantbytes) >= (MV->SEGMENTTABLE[baseDS]&0xFFFF))
            segmentationfault();*/

        MV->REGS[MBR].dato = valorNuevo;

        for(i= inicio + cantbytes ; cantbytes > 0; cantbytes --){
            MV->MEMORIA[i] = valorNuevo & 0xFF;
                valorNuevo = valorNuevo >> 8;
            i--;
        }

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

}
