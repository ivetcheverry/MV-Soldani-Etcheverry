#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "const.h"
#include "functions.h"

void init_funciones(tMV *MV)
{
    strcpy(MV->FUNCIONES[SYS].nombre, "SYS");
    MV->FUNCIONES[SYS].func = sys;
    strcpy(MV->FUNCIONES[JMP].nombre, "JMP");
    MV->FUNCIONES[JMP].func = jmp;
    strcpy(MV->FUNCIONES[JZ].nombre, "JZ");
    MV->FUNCIONES[JZ].func = jz;
    strcpy(MV->FUNCIONES[JP].nombre, "JP");
    MV->FUNCIONES[JP].func = jp;
    strcpy(MV->FUNCIONES[JN_].nombre, "JN");
    MV->FUNCIONES[JN_].func = jn_;
    strcpy(MV->FUNCIONES[JNZ].nombre, "JNZ");
    MV->FUNCIONES[JNZ].func = jnz;
    strcpy(MV->FUNCIONES[JNP].nombre, "JNP");
    MV->FUNCIONES[JNP].func = jnp;
    strcpy(MV->FUNCIONES[JNN].nombre, "JNN");
    MV->FUNCIONES[JNN].func = jnn;
    strcpy(MV->FUNCIONES[NOT_].nombre, "NOT");
    MV->FUNCIONES[NOT_].func = not_;

    strcpy(MV->FUNCIONES[STOP].nombre, "STOP");
    MV->FUNCIONES[STOP].func = stop;

    strcpy(MV->FUNCIONES[MOV].nombre, "MOV");
    MV->FUNCIONES[MOV].func = mov;
    strcpy(MV->FUNCIONES[ADD].nombre, "ADD");
    MV->FUNCIONES[ADD].func = add;
    strcpy(MV->FUNCIONES[SUB].nombre, "SUB");
    MV->FUNCIONES[SUB].func = sub;
    strcpy(MV->FUNCIONES[MUL].nombre, "MUL");
    MV->FUNCIONES[MUL].func = mul;
    strcpy(MV->FUNCIONES[DIV_].nombre, "DIV");
    MV->FUNCIONES[DIV_].func = div_;
    strcpy(MV->FUNCIONES[CMP].nombre, "CMP");
    MV->FUNCIONES[CMP].func = cmp;
    strcpy(MV->FUNCIONES[SHL].nombre, "SHL");
    MV->FUNCIONES[SHL].func = shl;
    strcpy(MV->FUNCIONES[SHR].nombre, "SHR");
    MV->FUNCIONES[SHR].func = shr;
    strcpy(MV->FUNCIONES[SAR].nombre, "SAR");
    MV->FUNCIONES[SAR].func = sar;
    strcpy(MV->FUNCIONES[AND].nombre, "AND");
    MV->FUNCIONES[AND].func = and;
    strcpy(MV->FUNCIONES[OR].nombre, "OR");
    MV->FUNCIONES[OR].func = or;
    strcpy(MV->FUNCIONES[XOR].nombre, "XOR");
    MV->FUNCIONES[XOR].func = xor;
    strcpy(MV->FUNCIONES[SWAP].nombre, "SWAP");
    MV->FUNCIONES[SWAP].func = swap;
    strcpy(MV->FUNCIONES[LDL].nombre, "LDL");
    MV->FUNCIONES[LDL].func = ldl;
    strcpy(MV->FUNCIONES[LDH].nombre, "LDH");
    MV->FUNCIONES[LDH].func = ldh;
    strcpy(MV->FUNCIONES[RND].nombre, "RND");
    MV->FUNCIONES[RND].func = rnd;

    strcpy(MV->FUNCIONES[PUSH].nombre, "PUSH");
    MV->FUNCIONES[RND].func = push;
    strcpy(MV->FUNCIONES[POP].nombre, "POP");
    MV->FUNCIONES[RND].func = pop;
    strcpy(MV->FUNCIONES[CALL_].nombre, "CALL");
    MV->FUNCIONES[RND].func = call_;
    strcpy(MV->FUNCIONES[RET].nombre, "RET");
    MV->FUNCIONES[RND].func = ret;
}

void init_regs(tMV *MV)
{
    int i;
    for (i = 0; i < 32; i++)
        MV->REGS[i].dato = 0;

    strcpy(MV->REGS[LAR].nombre, "LAR");
    strcpy(MV->REGS[MAR].nombre, "MAR");
    strcpy(MV->REGS[MBR].nombre, "MBR");

    strcpy(MV->REGS[IP].nombre, "IP");
    strcpy(MV->REGS[OPC].nombre, "OPC");
    strcpy(MV->REGS[OP1].nombre, "OP1");
    strcpy(MV->REGS[OP2].nombre, "OP2");

    // PILA
    strcpy(MV->REGS[SP].nombre, "SP");
    strcpy(MV->REGS[BP].nombre, "BP");

    // Bloque de registros generales
    strcpy(MV->REGS[EAX].nombre, "EAX");
    strcpy(MV->REGS[EBX].nombre, "EBX");
    strcpy(MV->REGS[ECX].nombre, "ECX");
    strcpy(MV->REGS[EDX].nombre, "EDX");
    strcpy(MV->REGS[EEX].nombre, "EEX");
    strcpy(MV->REGS[EFX].nombre, "EFX");

    // Acumulador y código de condición
    strcpy(MV->REGS[AC].nombre, "AC");
    strcpy(MV->REGS[CC].nombre, "CC");

    // Segmentos
    strcpy(MV->REGS[CS].nombre, "CS");
    strcpy(MV->REGS[DS].nombre, "DS");
}


void setCodeSegment(FILE *arch, tMV *MV)
{
    // vuelca el codigo al code segment
    int i, aux, posCS;
    posCS = buscarCS();
    for (i = 0; i < posCS; i++)
    {
        fread(&aux, 1, 1, arch);
        /*
        printf("%3x",aux);
        if (i>0 && i%15 == 0)       //escribe lo que lee del .vmx
            printf("\n");
        */
        MV->MEMORIA[i] = aux;
    }
}

void addsegmento(tMV *MV, int inicio, int tamano, int pos) {
    /*
    Recibe todos los parametros, lo unico que hace es insertar los segmentos en la tabla.
    */

    MV->SEGMENTTABLE[pos] = ( inicio<<16 || tamano);

}

int getdireccionfisica(tMV *MV, int Puntero) {
    int basePuntero = (Puntero>>16 && 0xFFFF);
    int offsetPuntero = (Puntero&&0xFFFF);
    int pos;

    pos = MV->SEGMENTTABLE[basePuntero]>>16 && 0xFFFF; // Obtengo el inicio
    pos += offsetPuntero;

    return pos;
}



void setSegmentTable(tMV *MV, FILE *arch)
{
    /*
    Esta funcion servirá para ambas versiones, depende la version que esté en la MV->version, actuará de una u otra forma.
    Objetivo: armar tabla de segmentos e ir inicializando punteros simultaneamente
    */
    int offset;
    int tamano;
    int aux;
    int inicio = 0;
    int pos;
    int indice;

    int ordenlectura[5] = {CS, DS, ES, SS, KS};
    int ordensegmentos[5] = {KS, CS, DS, ES, SS};

    if (MV->PARAM == -1)
        pos = 0;
    else
        pos = 1;

    if (MV->VERSION == 1)
    {
        fread(&aux, 1, 1, arch);
        tamano = aux;
        fread(&aux, 1, 1, arch);
        tamano += aux;

        addsegmento(MV, inicio, tamano, pos);
        inicio += tamano;
        pos++;
        addsegmento(MV, inicio, MV->MEM - inicio, pos);
    }

    else
    {
        // ESTOS FREAD LEERAN EL HEADER DEL VMX, Y EN CADA REGISTRO GUARDARÁ TEMPORALMENTE EL TAMAÑO DE CADA SEGMENTO

        for (int i = 0; i < 5; i++)
        {
            indice = ordenlectura[i];
            fread(&aux, 1, 1, arch);
            MV->REGS[indice].dato = aux;
            fread(&aux, 1, 1, arch);
            MV->REGS[indice].dato += aux;
        }

        for (int i = 0; i < 5; i++)
        {
            indice = ordensegmentos[i];
            if (MV->REGS[indice].dato <= 0)
                MV->REGS[indice].dato = -1;
            else
            {
                tamano = MV->REGS[indice].dato;
                MV->REGS[indice].dato = (pos << 16);
                addsegmento(MV, inicio, tamano, pos);
                inicio += tamano;
                pos++;
            }
        }

        // Aprovecho el archivo que esta abierto para leer el entry point
        fread(&aux,1,1,arch); 
        offset = aux;
        fread(&aux,1,1,arch);
        offset+=aux;

        MV->REGS[IP].dato = MV->REGS[CS].dato+offset;

    }
}

void setParamSegment(tMV *MV, int argsc, char *args[], int *sizePS, int *PSpointer)
{
    int j, w, pos, i = 0;
    int aux = argsc;
    int offsets[100];
    while (i < argsc && args[i] != "-p")
    {
        i++;
        aux--;
    }
    if (args[i] != "-p")
    {
        i++;
        pos = 0;
        for (w = 0; w < aux; w++)
        {
            offsets[w] = pos;
            j = 0;
            while (j != '\0')
            {
                MV->MEMORIA[pos] = args[i][j]; // accedo a la palabra i, y a la letra j
                pos++;
                j++;
            }
            MV->MEMORIA[pos] = '\0';
            pos++;
        }
        (*PSpointer) = pos;
        for (w = 0; w < aux; w++)
        {
            for (i = 0; i < 2; i++)
            {
                MV->MEMORIA[pos] = 0x00;
                pos++;
            }
            MV->MEMORIA[pos] = (offsets[w] && 0x0000FF00) >> 8;
            pos++;
            MV->MEMORIA[pos] = (offsets[w] && 0xFF);
            pos++;
        }
    }
    (*sizePS) = pos;
}

void init_MV(tMV *MV, int *OK, int CONTROL[], int argsc, char *args[]) //,char ANOMBREARCHIVO[]) {
{

    FILE *arch;
    int aux, i;
    int sizePS = 0;
    int PSpointer = 0;
    char *tipoParametro, *a;

    MV->MEM = RAMDEFAULT;
    MV->DISSASEMBLER = 0;
    MV->NOMBREIMAGEN = NULL;
    MV->PARAM = -1;

    for (i = 2; i < argsc; i++)
    { // analizo desde i=2 ya que el i=0 es la maquina, y el i=1 es el nombre del archivo vmx o vmi sin falta.
        a = args[i];
        tipoParametro = strrchr(a, '.');

        if (strcmp(tipoParametro, ".vmi") == 0)
            strcpy(MV->NOMBREIMAGEN, a);

        else if (strncmp(a, "m=", 2) == 0)
        {
            MV->MEM = atoi(a + 2);
        }

        else if (strcmp(a, "-d") == 0)
        {
            MV->DISSASEMBLER = 1;
        }

        else if (strcmp(a, "-p") == 0)
        {
            MV->PARAM = i + 1; // Indice dentro del argv donde comienzan los parametros
            break;             // Es el ultimo flag, no recorro mas
        }
    }

    arch = fopen(args[1], "rb");
    tipoParametro = strrchr(args[1], '.');

    if (arch)
    {
        if (tipoParametro == ".vmx")
        { // archivo que le pasamos tiene extension .vmx

            printf("\n");
            fread(&aux, 1, 1, arch); // CONTROLO LOS CARACTERES "VMX25"
            while (i < 5 && aux - CONTROL[i] == 0)
            {
                // printf("%3c",aux);
                i++;
                fread(&aux, 1, 1, arch);
            }
            if (i < 5)
            {
                printf("\n ERROR, ARCHIVO NO VALIDO!");
            }
            else
            { // CONTROLO VERSIONES
                // printf("\n VERSION: %d \n\n", aux);
                if (aux != 1 && aux != 2)
                    printf("\n VERSION NO SOPORTADA!");
                else
                { // VALIDO
                    MV->VERSION = aux;
                    (*OK) = 1;
                    setParamSegment(MV, argsc, args, &sizePS, &PSpointer);
                    setTabla(MV, arch, sizePS, PSpointer);

                    setCodeSegment(arch, MV);
                }
            }
        }
        else if (tipoParametro == ".vmi")
        {
            // carga a partir de una imagen.
        }

        fclose(arch);
    }
    else
        printf("EL ARCHIVO NO SE PUDO ABRIR!!");
}
