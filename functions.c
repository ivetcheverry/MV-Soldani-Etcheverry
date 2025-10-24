#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "const.h"
#include "functions.h"
#include <time.h>

void mostrar(tMV *MV, int OP, int j)
{
    int tipo, r, off;

    tipo = OP >> 24;

    if (tipo != 0)
    {

        if (tipo == MEMO)
        {
            r = (OP >> 16) & 0x1F;
            off = OP & 0xFFFF;

            if (off & NMASK16)
                off = (off ^ NMASK16) - NMASK16;

            if (off != 0)
                printf(" [%3s%+3d]", MV->REGS[r].nombre, off);
            else
                printf(" [%3s]", MV->REGS[r].nombre);
        }

        else if (tipo == REG)
        {
            r = OP & 0X1F;
            printf(" %3s", MV->REGS[r].nombre);
        }
        else
        { // INMEDIATO
            r = OP & 0XFFFF;
            if (j)
                printf(" %+3X", r);
            else
            {
                if (r & NMASK16)
                    r = (r ^ NMASK16) - NMASK16;
                printf(" %3d", r);
            }
        }
    }
}

// SIN OPERANDOS ------------------------------------------------------------
void stop(tMV *MV)
{
    printf("\n\n STOP ejecutado. Deteniendo la máquina virtual.");

    MV->REGS[IP].dato = 0;
    MV->REGS[IP].dato |= 0XFFFF;
}

void ret (tMV *MV){

    pop(MV);
    //devuelto en EAX
    MV->REGS[IP].dato=MV->REGS[EAX].dato;
}




// 1 OPERANDO ---------------------------------------------------------------
void sys(tMV *MV)
{
    int funcion, valor, aux, i,j,opanterior;
    char binario[33], imprimir[8];
    int B, H, O, D, C;
    char entrada;


    valor = 0;

    acceso_mem(MV, MV->REGS[OP2].dato);

    funcion = get(MV, MV->REGS[OP2].dato);
    D = MV->REGS[EAX].dato & 0b1;
    C = MV->REGS[EAX].dato & 0b10;
    O = MV->REGS[EAX].dato & 0b100;
    H = MV->REGS[EAX].dato & 0b1000;
    B = MV->REGS[EAX].dato & 0b10000;

    int cantidadceldas = (MV->REGS[ECX].dato & 0XFFFF);

    switch (funcion){

        case 1:
        {
            for (i = 0; i < cantidadceldas; i++){
                printf("\n");

                printf("[%04X]  ", MV->REGS[MAR].dato & 0XFFFF);
                if (D)
                    scanf("%d", &valor);
                if (C)
                    scanf("%c", &valor);
                if (O)
                    scanf("%o", &valor);
                if (H)
                    scanf("%x", &valor);
                if (B)
                {
                    scanf("%s", binario);
                    for (j = strlen(binario) - 1; j >= 0; j--)
                    {
                        valor = valor << 1;
                        valor |= binario[j] - '0';
                    }
                }

                setsys(MV, valor);
                MV->REGS[MAR].dato += ((MV->REGS[ECX].dato >> 16) & 0xffff);
            }
            break;
        }
        case 2:
        {
            for (i = 0; i < cantidadceldas; i++){
                printf("\n");

                valor = getsys(MV);
                printf("[%04X]  ", MV->REGS[MAR].dato & 0XFFFF);

                 if (B)
                {
                    unsigned int tmp = (unsigned int)valor;
                    for (j = 31; j >= 0; j--)
                    {
                        binario[31 - j] = (tmp & (1U << j)) ? '1' : '0';
                    }
                    binario[32] = '\0';
                    printf("0b%s\t", binario);
                }

                if (H)
                    printf("0x%8x \t", (unsigned int)valor);

                if (O)
                    printf("0o%o \t", (unsigned int)valor);
                

                
                int tmp_val = valor; // guardo el valor original
                if (C)
                {

                    for (j = 0; j < ((MV->REGS[ECX].dato & 0xFFFF0000) >> 16); j++)
                    {
                        imprimir[j] = (char)tmp_val & 0xff;
                        tmp_val = tmp_val >> 8;
                    }
                    for (j = ((MV->REGS[ECX].dato & 0xFFFF0000) >> 16) - 1; j >= 0; j--)
                        if (imprimir[j] >= 32 && imprimir[j] < 128)
                            printf("%c", (char)imprimir[j]);
                        else
                            printf(".");
                    printf(" \t");
                }

                if (D)
                    printf("%d\t", valor);

                MV->REGS[MAR].dato += ((MV->REGS[ECX].dato >> 16) & 0xffff);
                //printf("\n%08x",MV->REGS[MAR].dato);
            }

            break;
        }
        case 3:     //string read
        {
            printf("\n");
            printf("[%04X]  ", MV->REGS[MAR].dato & 0XFFFF);

            char buffer[512]; // limite razonable
            int len = 0;

            fflush(stdout);
            if (fgets(buffer, sizeof(buffer), stdin) == NULL)
                return;

            // Reemplazar '\n' por '\0'
            buffer[strcspn(buffer, "\n")] = '\0';

            // Máximo de caracteres a leer
            int max_len = MV->REGS[ECX].dato;

            if (max_len == -1)
                max_len = strlen(buffer);
            else if (max_len > (int)strlen(buffer))
                max_len = strlen(buffer);

            // Obtener dirección lógica desde EDX
            int seg_idx = (MV->REGS[EDX].dato >> 16) & 0xFFFF;
            int offset = MV->REGS[EDX].dato & 0xFFFF;

            // Validar segmento
            if (seg_idx < 0 || seg_idx > (MV->REGS[SS].dato >> 16))
                segmentationfault();

            // Dirección física real
            int base_fisica = ((MV->SEGMENTTABLE[seg_idx] >> 16) & 0xFFFF) + offset;

            // Chequeo de overflow del segmento
            int seg_tam = MV->SEGMENTTABLE[seg_idx] & 0xFFFF;
            if ((offset + max_len + 1) > seg_tam)
                segmentationfault();

            // Escribir en memoria byte a byte
            fgets(buffer, max_len, stdin);

            setsys_buffer(MV, buffer, strlen(buffer));

            break;
        }
        case 4:         //string write
        {

            // Obtener dirección lógica desde EDX
            int seg_idx = (MV->REGS[EDX].dato >> 16) & 0xFFFF;
            int offset  = MV->REGS[EDX].dato & 0xFFFF;

            // Validar segmento
            if (seg_idx < 0 || seg_idx > (MV->REGS[SS].dato >> 16))
                segmentationfault();

            // Dirección física base
            int base_fisica = ((MV->SEGMENTTABLE[seg_idx] >> 16) & 0xFFFF) + offset;

            // Tamaño del segmento
            int seg_tam = MV->SEGMENTTABLE[seg_idx] & 0xFFFF;

            // Obtener el string desde memoria (hasta '\0' o límite de segmento)
            char buffer[512];
            getsys_buffer(MV, buffer, base_fisica, seg_tam - offset);

            // Imprimir
            printf("\n");
            printf("%s", buffer);

            // Actualizar MBR con último carácter leído
            if (strlen(buffer) > 0)
                MV->REGS[MBR].dato = (unsigned char) buffer[strlen(buffer) - 1];
            else
                MV->REGS[MBR].dato = 0;

            break;
        }
        case 7:
        {
            printf("\n");
            printf("\033[2J\033[H");  // Limpia pantalla y mueve el cursor al inicio
            fflush(stdout);
            break;
        }
        case 0xF:
        {
            entrada = getchar();
            do {
                if (entrada == 'g')
                    generarimagen(MV);
                else if (entrada == '\n') {
                    opanterior = MV->REGS[OP2].dato;
                    generarimagen(MV);
                    MV->UNPASO=1;
                    ejecucion(MV);
                    MV->REGS[OP2].dato = opanterior;
                    MV->FUNCIONES[SYS].func(MV);
                }
                else if (entrada == 'q'){
                    generarimagen(MV);
                    MV->FUNCIONES[STOP].func(MV);
                }
                else
                    printf("CARACTER INVALIDO");

            } while (entrada != 'g' && entrada != 'q' && entrada != '\n');

            break;
        }
    }

}

void call_(tMV *MV) {
}

void jmp(tMV *MV)
{
    MV->REGS[IP].dato = get(MV, MV->REGS[OP2].dato);
}

void jz(tMV *MV)
{
    int Z = (MV->REGS[CC].dato >> 30) & 0b1;
    if (Z & 1)
        MV->REGS[IP].dato = get(MV, MV->REGS[OP2].dato);
}

void jnz(tMV *MV)
{
    int Z = ((MV->REGS[CC].dato >> 30) & 1);

    if (!(Z & 1))
        MV->REGS[IP].dato = get(MV, MV->REGS[OP2].dato);
}

void jn_(tMV *MV)
{
    int N = (MV->REGS[CC].dato >> 31) & 0b1;
    if (N & 1)
        MV->REGS[IP].dato = get(MV, MV->REGS[OP2].dato);
}

void jnn(tMV *MV)
{
    int N = (MV->REGS[CC].dato >> 31) & 0b1;
    if (!(N & 1))
        MV->REGS[IP].dato = get(MV, MV->REGS[OP2].dato);
}

void jp(tMV *MV)
{
    int N = (MV->REGS[CC].dato >> 31) & 0b1;
    int Z = ((MV->REGS[CC].dato >> 30) & 1);
    if (!(N & 1) && !(Z & 1))
        MV->REGS[IP].dato = get(MV, MV->REGS[OP2].dato);
}

void jnp(tMV *MV)
{
    int N, Z;
    N = (MV->REGS[CC].dato >> 31) & 0b1;
    if (N & 1)
        MV->REGS[IP].dato = get(MV, MV->REGS[OP2].dato);
    else
    {
        Z = (MV->REGS[CC].dato >> 30) & 0b1;
        if (Z & 1)
            MV->REGS[IP].dato = get(MV, MV->REGS[OP2].dato);
    }
}

void not_(tMV *MV)
{
    int aux;
    aux = get(MV, MV->REGS[OP2].dato);
    aux = ~aux;

    set(MV, MV->REGS[OP2].dato, aux);

    setCC(MV, get(MV, MV->REGS[OP2].dato));
}

void pop(tMV *MV) {
    int sp_val = MV->REGS[SP].dato & 0xFFFF;    // offset dentro del stack segment (en palabras)
    int ss_index = MV->REGS[SS].dato >> 16;     // índice del segmento de pila
    int ss_base  = (MV->SEGMENTTABLE[ss_index] >> 16) & 0xFFFF;
    int ss_limit = MV->SEGMENTTABLE[ss_index] & 0xFFFF;

    // Verificar underflow
    if (sp_val >= ss_limit) {
        stack_underflow();
    }

    // Leer el valor (entero de 4 bytes)
    int valor = MV->MEMORIA[ss_base + sp_val];

    // Obtengo el valor y se lo doy a EAX
    MV->REGS[EAX].dato= get(MV, MV->REGS[OP2].dato);

    // Incrementar SP en 1 palabra (4 bytes)
    sp_val += 1;
    MV->REGS[SP].dato = (MV->REGS[SP].dato & 0xFFFF0000) | sp_val;

}


/* void push (tMV *MV) {
    int valor;

    MV->REGS[SP].dato -=4;

    if(MV->REGS[SP].dato < MV->SEGMENTTABLE[baseSS])         //?? base ss
        stackoverflow();

    valor = get(MV, MV->REGS[OP2].dato);

    if (valor & 0x8000)                     //extiendo signo
        valor |= 0xFFFF0000;

    int addr = MV->REGS[SP].dato;
    MV->MEMORIA[addr]     = (valor >> 24) & 0xFF;
    MV->MEMORIA[addr + 1] = (valor >> 16) & 0xFF;
    MV->MEMORIA[addr + 2] = (valor >> 8)  & 0xFF;
    MV->MEMORIA[addr + 3] = valor & 0xFF;



} */

void push(tMV *MV) {
    int valor;

    // Obtener info del stack segment
    int segIndex = (MV->REGS[SS].dato >> 16);
    int segDesc = MV->SEGMENTTABLE[segIndex];
    int baseSS = (segDesc >> 16) & 0xFFFF;
    int tamanioSS = segDesc & 0xFFFF;

    // Leer SP actual
    int offsetSP = MV->REGS[SP].dato & 0xFFFF;

    // Decrementar SP en 4
    offsetSP -= 4;

    // Verificar overflow
    if (offsetSP < 0) {
        stack_overflow();
    }

    // Obtener valor del operando
    valor = get(MV, MV->REGS[OP2].dato);

    // Extender signo a 32 bits si es 16 bits con signo
    if (valor & 0x8000)
        valor |= 0xFFFF0000;

    // Escribir en la pila (MSB → LSB)
   /* int addr = baseSS + offsetSP;
    MV->MEMORIA[addr]     = (valor >> 24) & 0xFF;
    MV->MEMORIA[addr + 1] = (valor >> 16) & 0xFF;
    MV->MEMORIA[addr + 2] = (valor >> 8) & 0xFF;
    MV->MEMORIA[addr + 3] = valor & 0xFF; */

    set(MV, MV->REGS[OP2].dato, valor);

    // Actualizar SP
    MV->REGS[SP].dato = (segIndex << 16) | offsetSP;
}


void call (tMV *MV) {
    /*
    Primero guardamos la direccion de la subrutina en una variable aux.
    En OP2 guardamos el IP que teniamos antes de ejecutar subrutina para asi reutilizar el push
    por ultimo seteamos IP con la direccion de la subrutina.
    */
    int aux = MV->REGS[OP2].dato;
    MV->REGS[OP2].dato = MV->REGS[IP].dato;
    push(MV);
    //MV->REGS[IP].dato = aux;
    jmp(MV);
}





// 2 OPERANDOS --------------------------------------------------------------

void mov(tMV *MV)
{
    set(MV, MV->REGS[OP1].dato, get(MV, MV->REGS[OP2].dato));
}

void add(tMV *MV)
{
    int suma;
    suma = get(MV, MV->REGS[OP1].dato) + get(MV, MV->REGS[OP2].dato);

    set(MV, MV->REGS[OP1].dato, suma);

    setCC(MV, get(MV, MV->REGS[OP1].dato));
}

void sub(tMV *MV)
{
    int resta;

    resta = get(MV, MV->REGS[OP1].dato) - get(MV, MV->REGS[OP2].dato);
    // resta &= 0x0000FFFF;

    set(MV, MV->REGS[OP1].dato, resta);
    setCC(MV, get(MV, MV->REGS[OP1].dato));
}

void mul(tMV *MV)
{
    int v1, v2, mul;
    v1 = get(MV, MV->REGS[OP1].dato);
    v2 = get(MV, MV->REGS[OP2].dato);

    set(MV, MV->REGS[OP1].dato, v1 * v2);

    setCC(MV, get(MV, MV->REGS[OP1].dato));
}

void div_(tMV *MV)
{
    int v1, v2;

    v1 = get(MV, MV->REGS[OP1].dato);
    v2 = get(MV, MV->REGS[OP2].dato);

    if (v2 != 0)
    {
        set(MV, MV->REGS[OP1].dato, v1 / v2);
        MV->REGS[AC].dato = v1 % v2;

        setCC(MV, get(MV, MV->REGS[OP1].dato));
    }
    else
        divisionzero();
}

void cmp(tMV *MV)
{
    int v1, v2, result;

    v1 = get(MV, MV->REGS[OP1].dato);
    v2 = get(MV, MV->REGS[OP2].dato);

    setCC(MV, v1 - v2);
}

void shl(tMV *MV)
{
    int v2;

    v2 = get(MV, MV->REGS[OP2].dato);

    set(MV, MV->REGS[OP1].dato, (get(MV, MV->REGS[OP1].dato) << v2));
}

void shr(tMV *MV)
{
    int v1, v2;
    unsigned int mask_aux;

    v2 = get(MV, MV->REGS[OP2].dato);
    v1 = get(MV, MV->REGS[OP1].dato);

    v1 = v1 >> v2;
    mask_aux = 0xFFFFFFFF >> v2;
    v1 &= mask_aux;

    set(MV, MV->REGS[OP1].dato, v1);
}

void sar(tMV *MV)
{
    int v1, v2;

    v2 = get(MV, MV->REGS[OP2].dato);
    v1 = get(MV, MV->REGS[OP1].dato);

    set(MV, MV->REGS[OP1].dato, v1 >> v2);
}

void and(tMV *MV)
{
    int v1, v2;
    v1 = get(MV, MV->REGS[OP1].dato);
    v2 = get(MV, MV->REGS[OP2].dato);
    set(MV, MV->REGS[OP1].dato, v1 & v2);
}

void or(tMV *MV)
{
    int v1, v2;
    v1 = get(MV, MV->REGS[OP1].dato);
    v2 = get(MV, MV->REGS[OP2].dato);
    set(MV, MV->REGS[OP1].dato, v1 | v2);
}

void xor(tMV *MV)
{
    int v1, v2;
    v1 = get(MV, MV->REGS[OP1].dato);
    v2 = get(MV, MV->REGS[OP2].dato);
    set(MV, MV->REGS[OP1].dato, v1 ^ v2);
}

void swap(tMV *MV)
{
    int v1, v2, aux;

    v1 = get(MV, MV->REGS[OP1].dato);
    v2 = get(MV, MV->REGS[OP2].dato);
    aux = v1;
    set(MV, MV->REGS[OP1].dato, v2);
    set(MV, MV->REGS[OP2].dato, aux);
}

void ldl(tMV *MV)
{
    int v1, v2;
    v1 = get(MV, MV->REGS[OP1].dato) & 0xFFFF0000;
    v2 = (get(MV, MV->REGS[OP2].dato)) & 0xFFFF;
    set(MV, MV->REGS[OP1].dato, v1 | v2);
}

void ldh(tMV *MV)
{
    int v1, v2;
    v1 = get(MV, MV->REGS[OP1].dato) & 0xFFFF;
    v2 = (get(MV, MV->REGS[OP2].dato)) & 0xFFFF;
    v2 = v2 << 16;
    set(MV, MV->REGS[OP1].dato, v1 | v2);
}

void rnd(tMV *MV)
{
    int v2, aux;

    v2 = get(MV, MV->REGS[OP2].dato);
    aux = rand() % (v2 + 1);

    set(MV, MV->REGS[OP1].dato, aux);
}

// ERRORES ------------------------------------------

void segmentationfault()
{
    printf("\nERROR! SEGMENTATION FAULT!");
    exit(1);
}

void invalidfunction()
{
    printf("\nFUNCION INVALIDA!");
    exit(1);
}

void divisionzero()
{
    printf("\nERROR! DIVISION POR CERO!");
    exit(1);
}

void memoria_insuficiente(){
    printf("\nERROR! MEMORIA INSUFICIENTE!");
    exit(1);
}

void stack_overflow()
{
    printf("\nERROR! STACK OVERFLOW!");
    exit(1);
}

void stack_underflow()
{
    printf("\nERROR! STACK UNDERFLOW!");
    exit(1);
}
