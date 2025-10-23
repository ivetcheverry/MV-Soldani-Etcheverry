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
    strcpy(MV->REGS[ES].nombre, "ES");
    strcpy(MV->REGS[SS].nombre, "SS");
    strcpy(MV->REGS[KS].nombre, "KS");
    strcpy(MV->REGS[PS].nombre, "PS");

}


void setCodeSegment(FILE *arch, tMV *MV)
{
    // vuelca el codigo al code segment
    int i, aux, liminf,limsup,base;

    base = (MV->REGS[CS].dato & 0xFFFF0000)>>16;
    liminf = (MV->SEGMENTTABLE[base]&0xFFFF0000) >> 16;
    limsup = (MV->SEGMENTTABLE[base]&0xFFFF0000) >> 16;
    limsup += MV->SEGMENTTABLE[base]&0xFFFF;

    for (i = liminf; i < limsup; i++)
    {
        fread(&aux, 1, 1, arch);
        
        /*printf("%3x",aux);
        if (i>0 && i%15 == 0)       //escribe lo que lee del .vmx
            printf("\n");*/
        
        MV->MEMORIA[i] = aux;
    }
}

void addsegmento(tMV *MV, int inicio, int tamano, int pos) {
    /*
    Recibe todos los parametros, lo unico que hace es insertar los segmentos en la tabla.
    Verifica si se puede ingresar el segmento o la memoria es insuficiente.
    */
    int posicion_memoria;
    posicion_memoria = inicio + tamano;
    if (posicion_memoria <= MV->MEM){
    MV->SEGMENTTABLE[pos] = (inicio<<16)&0xFFFF0000;
    MV->SEGMENTTABLE[pos] += tamano;
    }
    else
        memoria_insuficiente();

}

int getdireccionfisica(tMV *MV, int Puntero) {
    int basePuntero = (Puntero>>16 & 0xFFFF);
    int offsetPuntero = (Puntero&0xFFFF);
    int pos;

    pos = (MV->SEGMENTTABLE[basePuntero]>>16) & 0xFFFF; // Obtengo el inicio
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

    int ordenlectura[5];
    int ordensegmentos[5];

    if (MV->PARAM == -1)
        pos = 0;
    else
        pos = 1;

    if (MV->VERSION == 1)
    {
        aux=0;
        fread(&aux, 1, 1, arch);
        //printf("%x",aux);
        tamano = aux;
        fread(&aux, 1, 1, arch);
        //printf("%x",aux);
        tamano += aux;

        addsegmento(MV, inicio, tamano, pos);
        MV->REGS[IP].dato = MV->REGS[CS].dato = pos<<16;
        inicio += tamano;  pos++;
        addsegmento(MV, inicio, MV->MEM - inicio, pos);
        MV->REGS[DS].dato = pos<<16;
    }

    else
    {
          ordenlectura[0] = CS;   ordenlectura[1] = DS;   ordenlectura[2] = ES;   ordenlectura[3] = SS;   ordenlectura[4] = KS;
        ordensegmentos[0] = KS; ordensegmentos[1] = CS; ordensegmentos[2] = DS; ordensegmentos[3] = ES; ordensegmentos[4] = SS;
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
        MV->ENTRYPOINT = getdireccionfisica(MV,MV->REGS[IP].dato);
    }
}

void setParamSegment(tMV *MV, int argsc, char *args[])
{

    /*
     i: variable que lleva offset donde inicia cada palabra
     j: variable que recorre cada letra de cada palabra
     w: variable que recorre cada palabra
    */

    int j, w, i;
    int offsets[20], inicio=0, tamano=0;
    int puntero;

    int cant_palabras = argsc - MV->PARAM; //MV->PARAM tiene el indice dentro del vector args[] donde comienza la primer palabra
    i=0;

    MV->ARGC= cant_palabras;

    for (w = 0; w < cant_palabras; w++){
        offsets[w] = i;
        j = 0;
        while (j != '\0'){
            MV->MEMORIA[i] = args[i][j]; // guardo en memoria, la letra [j] de la palabra guardada en args[i]
            i++; j++;
        }
        MV->MEMORIA[i] = '\0';
        i++;
    }

    // Ya guardadas las palabras en memoria, puedo armar el puntero de PS al primer puntero dentro de memoria.
    MV->REGS[PS].dato = i;

    for (w=0; w<cant_palabras; w++) {
        puntero = offsets[w];
        MV->MEMORIA[i]= puntero&0xFF000000; i++;
        MV->MEMORIA[i]= puntero&0xFF0000;   i++;
        MV->MEMORIA[i]= puntero&0xFF00;     i++;
        MV->MEMORIA[i]= puntero&0xFF;       i++;
    }

    addsegmento(MV,0,i,0);
}

void generarimagen(tMV *MV) {

    int i;
    FILE *arch = fopen(MV->NOMBREIMAGEN,"wb");
    int HEADER[6] = {'V','M','I','2','5','1'};

    if (arch) {
        //CARGA HEADER
        for (i=0;i<6;i++)
            fwrite(&HEADER[i],1,1,arch);

        //CARGA TABLA DE SEGMENTOS
        for (i=0;i<8;i++)
            fwrite(&MV->SEGMENTTABLE[i],1,1,arch);

        for (i=0; i<MV->MEM; i++) {
            fwrite(&MV->MEMORIA[i],1,1,arch);
        }

    fclose(arch);

    } else
        printf("NO SE PUDO ABRIR/GENERAR EL ARCHIVO");
}


void cargarimagen(tMV *MV, FILE *arch) {
    int aux=0,mem=0,i;
    int regsaux[32], tablaaux[8];


    fread(&aux,1,1,arch); mem = aux;
    fread(&aux,1,1,arch); mem += aux;

    MV->MEM=mem;

    i=0;
    fread(regsaux,4,32,arch);

    for (i=0;i<32;i++)
    MV->REGS[i].dato = regsaux[i];

    MV->ENTRYPOINT = getdireccionfisica(MV, MV->REGS[IP].dato);

    fread(tablaaux,4,8,arch);
    for (i=0;i<8;i++)
        MV->SEGMENTTABLE[i] = tablaaux[i];

    i=0;
    while (fread(&aux,1,1,arch)) {
        MV->MEMORIA[i] = aux;
        i++;
    }
}

void init_MV(tMV *MV, int *OK, int CONTROLVMX[],int CONTROLVMI[], int argsc, char *args[])
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
   // arch = fopen(args[1], "rb");
    arch = fopen(NOMBREARCHIVO, "rb");
   // tipoParametro = strrchr(args[1], '.');
    tipoParametro = ".vmx";

    if (arch)
    {
        if (tipoParametro == ".vmx")
        { // archivo que le pasamos tiene extension .vmx
            i=0;
            //printf("\n");
            fread(&aux, 1, 1, arch); // CONTROLO LOS CARACTERES "VMX25"

            while (i < 5 && aux - CONTROLVMX[i] == 0)
            {
                //printf("%3c",aux);
                i++;
                fread(&aux, 1, 1, arch);
            }
            if (i < 5)
            {
                printf("\n ERROR, ARCHIVO NO VALIDO!");
            }
            else
            { // CONTROLO VERSIONES
                 //printf("\n VERSION: %d \n\n", aux);
                if (aux != 1 && aux != 2)
                    printf("\n VERSION NO SOPORTADA!");
                else
                { // VALIDO
                    MV->VERSION = aux;
                    (*OK) = 1;
                    if (MV->PARAM!=-1)
                        setParamSegment(MV, argsc, args);
                    setSegmentTable(MV,arch);
                    setCodeSegment(arch, MV);
                }
            }
        }
        else if (tipoParametro == ".vmi")
        {
            i=0;
            //printf("\n");
            fread(&aux, 1, 1, arch); // CONTROLO LOS CARACTERES "VMI25"

            while (i < 5 && aux - CONTROLVMI[i] == 0)
            {
                //printf("%3c",aux);
                i++;
                fread(&aux, 1, 1, arch);
            }
            if (i < 5)
            {
                printf("\n ERROR, ARCHIVO NO VALIDO!");
            }
            else
            { // CONTROLO VERSION DE IMAGEN
                 //printf("\n VERSION: %d \n\n", aux);
                if (aux != 1)
                    printf("\n VERSION NO SOPORTADA!");
                else
                { // VALIDO
                    MV->VERSION = aux;
                    cargarimagen(MV,arch);
                }
            }

        }

        fclose(arch);
    }
    else
        printf("EL ARCHIVO NO SE PUDO ABRIR!!");
}
