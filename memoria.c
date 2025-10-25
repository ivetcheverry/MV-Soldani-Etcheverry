#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "const.h"
#include "functions.h"

/*
ARCHIVO PARA GUARDAR FUNCIONES ESPECIFICOS DE LOS ACCESOS A MEMORIA
*/

void acceso_mem (tMV *MV, int OP){
    int offset=0,regcod,base, sys=0;
    int cantbytes=0; int inicio=0;  
    int base_segmento, size_segmento;


    if ( ( (OP & 0xFF000000) >>24 ) == 3) {
        regcod = (OP & 0xFF0000) >> 16;
        offset=  analizarsigno(OP & 0xFFFF,2);
    }
    else {      //SYS
        regcod = EDX;
        sys=1;
    }

    base = ((MV->REGS[regcod].dato>>16 ) &0xFFFF);

   // if (base>=0 && base <= (MV->REGS[SS].dato >>16)) { //ya que siempre habra SS
        MV->REGS[LAR].dato = base;
        MV->REGS[LAR].dato = MV->REGS[LAR].dato<<16;
        MV->REGS[LAR].dato |= ((offset + (MV->REGS[regcod].dato & 0XFFFF))&0xffff);


    //printf("%08x",MV->REGS[LAR].dato);
    if(sys)
        MV->REGS[MAR].dato = (MV->REGS[ECX].dato & 0XFFFF0000) >> 16;
    else
        MV->REGS[MAR].dato = 0x0 | get_tipo_mem(MV->REGS[OP2].dato);    //obtengo cantidad de bytes a escribir/leer en memoria

    //printf("%08x",MV->REGS[MAR].dato);

    MV->REGS[MAR].dato = MV->REGS[MAR].dato << 16;

    MV->REGS[MAR].dato |= getdireccionfisica(MV,MV->REGS[LAR].dato);

    //printf("%08x",MV->REGS[MAR].dato);

    cantbytes = ((MV->REGS[MAR].dato & 0xFFFF0000)>>16);
    inicio = ((MV->REGS[MAR].dato & 0xFFFF));


    if (base != MV->REGS[SS].dato>>16){

    if  (inicio+cantbytes >= (MV->SEGMENTTABLE[base]&0xFFFF)  +  ((MV->SEGMENTTABLE[base]>>16)&0xFFFF)) 
        segmentationfault();
    else if (inicio < ((MV->SEGMENTTABLE[base]>>16)&0xFFFF))
        segmentationfault();

    }


    /*
    printf("\n");
    printf("LAR %08x \n MAR %08x", MV->REGS[LAR].dato, MV->REGS[MAR].dato);
    printf("\n");
    */

}


int getsys(tMV *MV) {
    int i, tipo, valor=0;
    int inicio,cantbytes;

        cantbytes = ((MV->REGS[MAR].dato & 0xFFFF0000)>>16);
        inicio= ((MV->REGS[MAR].dato & 0xFFFF));

            for(i = inicio ; cantbytes > 0; cantbytes--){
                valor = valor<<8;
                valor |= MV->MEMORIA[i];
                i++;
            }



    MV->REGS[MBR].dato = valor;
    return MV->REGS[MBR].dato;

}


void getsys_buffer(tMV *MV, char *buffer, int inicio, int max_len) {
    int i = 0;

    // Leer desde memoria hasta '\0' o fin de segmento o límite máximo
    while (i < max_len) {
        unsigned char c = MV->MEMORIA[inicio + i];
        buffer[i] = c;
        if (c == '\0') break;
        i++;
    }

    // Asegurar fin de cadena
    buffer[i] = '\0';
}



void setsys(tMV *MV, int valorNuevo) {
    int i, tipo, valor;
    int inicio,cantbytes;

        cantbytes = ((MV->REGS[MAR].dato & 0xFFFF0000)>>16);
        inicio= ((MV->REGS[MAR].dato & 0xFFFF))-1;

        MV->REGS[MBR].dato = valorNuevo;

        for(i= inicio + cantbytes ; cantbytes > 0; cantbytes --){
            MV->MEMORIA[i] = valorNuevo & 0xFF;
                valorNuevo = valorNuevo >> 8;
            i--;
        }

}


void setsys_buffer(tMV *MV, char *buffer, int length) {
    int inicio, cantbytes;

    // Obtener datos de MAR (inicio y cantidad)
    cantbytes = ((MV->REGS[MAR].dato & 0xFFFF0000) >> 16);
    inicio = ((MV->REGS[MAR].dato & 0xFFFF));

    // Si ECX = -1 → cantbytes = 0xFFFF → sin límite
    if (cantbytes == 0xFFFF || cantbytes == -1) {
        cantbytes = length;  // usar la cantidad real leída
    }

    if (length > cantbytes) length = cantbytes;  // seguridad

    // Escribir en memoria byte a byte
    for (int i = 0; i < length; i++)
        MV->MEMORIA[inicio + i] = (unsigned char) buffer[i];

    MV->MEMORIA[inicio + length] = '\0';  // terminar string

    // Actualizar MBR con último valor escrito (opcional)
    MV->REGS[MBR].dato = (unsigned char) buffer[length - 1];
}