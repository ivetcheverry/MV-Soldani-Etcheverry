#include <stdio.h>
#include <stdlib.h>
#include "const.h"
#include "functions.h"


int get(tMV *MV, int OP) {
    int tipo,valor,mask;
    int regcod, offset; // REGCOD REPRESENTADO POR 5 BITS Y EL OFFSET POR 2 BYTES.
    
    tipo = OP>>24;      //ME QUEDO CON EL BYTE MAS SIGNIFICATIVO

    if (tipo==INM)
        valor = OP &0XFFFF; // INMEDIATO: VALOR DE LOS ULTIMOS 2 BYTES
    else if (tipo==REG){
        regcod= OP & 0x1F;
        valor = MV->REGS[regcod].dato;
    }else {
        regcod = ((OP >> 16) & 0x1F); 
        offset = (OP & 0XFFFF);   
        valor =  MV->MEMORIA[regcod+offset+MV->SEGMENTTABLE[1][0]];
    }
    //ANALIZO SI ES NEGATIVO:
    if (valor & 0b1000000){
        valor= (valor ^ NMASK) - NMASK; 
    }
    
    return valor;

}
void set(tMV *MV, int OP, int valorNuevo) {
    int tipo, valor;
    int regcod, offset;

    tipo = OP >>24;
    
    if (tipo==REG){
        regcod= OP & 0X1F;
        MV->REGS[regcod].dato = valorNuevo;
    }else {
        regcod = (OP >> 16) & 0x1F; 
        offset = (OP & 0XFFFF);
        MV->MEMORIA[regcod+offset+MV->SEGMENTTABLE[1][0]]= valorNuevo;  
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
    
    printf("\n CC: %0x",MV->REGS[CC].dato);

}