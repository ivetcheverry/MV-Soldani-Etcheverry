#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "const.h"
#include "functions.h"

/*
ESTE ARCHIVO FUNCIONARA PARA GUARDAR DISTINTAS FUNCIONES GENERALES PARA TODOS LOS ARCHIVOS
*/

int getdireccionfisica(tMV *MV, int Puntero)
{
    int basePuntero = (Puntero >> 16 & 0xFFFF);
    int offsetPuntero = (Puntero & 0xFFFF);
    int pos;
    
    if (basePuntero != 0XFFFF) {
        pos = (MV->SEGMENTTABLE[basePuntero] >> 16) & 0xFFFF; // Obtengo el inicio
        pos += offsetPuntero;
    }
    else
    pos = -1;

    pos = analizarsigno(pos,2);
    return pos;
}

int get_tipo_mem(int OP)
{
    int valor = 4;
    int bits;
    bits = (OP & 0x00FF0000) >> 22; // dos bits mas significativos del codigo de memoria

    return (valor - bits);
}

void subrutinaprincipal(tMV *MV)
{
    int ipaux;
    int ipanterior = MV->REGS[IP].dato;

    MV->REGS[OP2].dato = 0x01000000 + PS;
    push(MV);
    MV->REGS[OP2].dato = 0x02000000+ MV->ARGC;
    push(MV);
    MV->REGS[IP].dato = -1;
    MV->REGS[OP2].dato = 0x01000000+IP;
    push(MV);

    ipaux = ipanterior & 0xFFFF0000; // base ip.  0001 0000
    ipaux |= MV->ENTRYPOINT;
    MV->REGS[IP].dato = ipaux;
    //printf("\n %08x", MV->REGS[IP].dato);
    //printf("UBICACION DEL IP: %d",getdireccionfisica(MV, MV->REGS[IP].dato));
}

/*int generarOP(int valor, int tipo) {
    int op;
    if (tipo == 1) {
        op = 0x01000000;
        op |= valor;
    }
    else if (tipo == 3) {
        op = 0x03000000;
        op |= valor;
    }


    return op;
}*/

int analizarsigno(int valor, int cantbytes)
{
    /*
    Esta funcion sirve para poder analizar a partir de la cantidad de byes que ocupa el valor a ingresar
    si tiene un '1' en el byte mas significativo, y a partir de ahi selecciona la mascara mas adecuada
    y mediante el complemento a 2, transforma el valor y lo retorna, si es necesario.
    */

    int signo_mask;
    int extend_mask;

    signo_mask = 1 << ((cantbytes * 8) - 1);
    extend_mask = ~((1 << (cantbytes * 8)) - 1);
    if (valor & signo_mask)
        valor |= extend_mask;

    return valor;
}