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

    pos = (MV->SEGMENTTABLE[basePuntero] >> 16) & 0xFFFF; // Obtengo el inicio
    pos += offsetPuntero;

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
    int ip_anterior = MV->REGS[IP].dato;

    MV->REGS[OP2].dato = MV->REGS[PS].dato;
    push(MV);
    MV->REGS[OP2].dato = MV->ARGC;
    push(MV);
    MV->REGS[OP2].dato = -1;
    push(MV);

    MV->REGS[IP].dato = ip_anterior;
}

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