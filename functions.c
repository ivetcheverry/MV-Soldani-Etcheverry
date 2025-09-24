#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "const.h"
#include "functions.h"
#include <time.h>


void mostrar(tMV *MV, int OP, int j){
    int tipo,r,off;

    tipo = OP>>24;

    if (tipo!=0) {

        if (tipo==MEMO){
            r = (OP>>16)&0x1F;
            off=OP&0xFFFF;

            if (off & NMASK16)
                off = (off ^ NMASK16) - NMASK16;

            if (off!=0)
                printf(" [%3s%+3d]",MV->REGS[r].nombre,off);
            else 
                printf(" [%3s]",MV->REGS[r].nombre);
        }

        else if (tipo==REG){
            r=OP&0X1F;
            printf(" %3s",MV->REGS[r].nombre);
        }
        else{ //INMEDIATO
            r = OP&0XFFFF;
            if (j)
                printf(" %+3X",r);
            else {
                if (r & NMASK16)
                r= (r ^ NMASK16) - NMASK16; 
                printf(" %3d",r);
            }
        }
    }
}


// SIN OPERANDOS ------------------------------------------------------------
void stop(tMV *MV) {
    printf("\n\n STOP ejecutado. Deteniendo la máquina virtual.");

    MV->REGS[IP].dato = baseCS<<16;
    MV->REGS[IP].dato |= 0XFFFF;
}


// 1 OPERANDO ---------------------------------------------------------------
void sys(tMV *MV){
    int funcion,valor, aux;
    char binario[33];
    int B,H,O,D,C;
    
    valor=0;

    acceso_mem(MV,MV->REGS[OP2].dato);

    funcion = get(MV,MV->REGS[OP2].dato);
    D =  MV->REGS[EAX].dato&0b1;
    C =  MV->REGS[EAX].dato&0b10;
    O =  MV->REGS[EAX].dato&0b100;
    H =  MV->REGS[EAX].dato&0b1000;
    B =  MV->REGS[EAX].dato&0b10000;
    
    if (funcion==1){ //READ   
        printf("\nINGRESAR VALOR: "); 
        if (D)
            scanf("%d",&valor);
        if (C)
            scanf("%c",&valor);
        if (O)
            scanf("%o",&valor);
        if (H)
            scanf("%x",&valor);
        if (B) {
            scanf("%s",binario);
            for (int i=strlen(binario)-1 ; i>=0 ; i--){
                valor=valor<<1;
                valor|=binario[i]-'0';
            }
        }

        setsys(MV,valor);

    }  

    
    else { //WRITE
        valor = getsys(MV);
        printf("\n\tRESULTADO: ");
        if (D)
            printf("%d\t",valor);
        if (C)
            printf("%c\t",(char)valor);
        if (O)
            printf("0o%o\t",valor);
        if (H)
            printf("0x%x\t",valor);
        if (B){ 
            int i=32;
            while (valor > 0 && i > 0) {
            i--;
            binario[i] = (valor & 1) ? '1' : '0';
            valor = valor >> 1;
        }
        printf("0b%s\t", &binario[i]);
        }

    }
    

}


void jmp(tMV *MV){
    MV->REGS[IP].dato=get(MV, MV->REGS[OP2].dato);
}

void jz (tMV *MV){
    int Z = (MV->REGS[CC].dato >> 30) & 0b1 ; 
    if (Z & 1)
       MV->REGS[IP].dato=get(MV, MV->REGS[OP2].dato);
}

void jnz (tMV *MV){
    int Z = ((MV->REGS[CC].dato >> 30) & 1); 

    if (!(Z & 1))
       MV->REGS[IP].dato=get(MV, MV->REGS[OP2].dato);
}

void jn_ (tMV *MV){
    int N = (MV->REGS[CC].dato >> 31) & 0b1 ; 
    if (N & 1)
       MV->REGS[IP].dato=get(MV, MV->REGS[OP2].dato);
}

void jnn (tMV *MV){
    int N = (MV->REGS[CC].dato >> 31) & 0b1 ; 
    if (!(N & 1))
       MV->REGS[IP].dato=get(MV, MV->REGS[OP2].dato);
}

void jp (tMV *MV){
    int N = (MV->REGS[CC].dato >> 31) & 0b1 ; 
    if (!(N & 1))
       MV->REGS[IP].dato=get(MV, MV->REGS[OP2].dato);
}

void jnp (tMV *MV){
    int N,Z;
    N = (MV->REGS[CC].dato >> 31) & 0b1 ;
    if (N & 1)
       MV->REGS[IP].dato=get(MV, MV->REGS[OP2].dato);
    else{
        Z = (MV->REGS[CC].dato >> 30) & 0b1 ;
        if (Z & 1)
            MV->REGS[IP].dato=get(MV, MV->REGS[OP2].dato);
    }
}

void not_(tMV *MV){     
    int aux;
    aux = get(MV, MV->REGS[OP2].dato);
    aux = ~aux;

    set(MV,MV->REGS[OP2].dato,aux);

    setCC(MV,get(MV,MV->REGS[OP2].dato));
}


// 2 OPERANDOS --------------------------------------------------------------

void mov(tMV *MV){
    set(MV,MV->REGS[OP1].dato ,get(MV,MV->REGS[OP2].dato));
}


void add(tMV *MV){
    int suma;
    suma = get(MV,MV->REGS[OP1].dato)+get(MV,MV->REGS[OP2].dato);

    set(MV,MV->REGS[OP1].dato,suma);

    setCC(MV,get(MV,MV->REGS[OP1].dato));
}

void sub (tMV *MV){
    int resta;

    resta = get(MV,MV->REGS[OP1].dato)-get(MV,MV->REGS[OP2].dato) ;
    //resta &= 0x0000FFFF;

    set(MV,MV->REGS[OP1].dato,resta);
    setCC(MV,get(MV,MV->REGS[OP1].dato));
}

void mul (tMV *MV){
    int v1, v2,mul;
    v1 = get(MV,MV->REGS[OP1].dato);
    v2 = get(MV,MV->REGS[OP2].dato);

    set(MV,MV->REGS[OP1].dato,v1*v2);

    setCC(MV,get(MV,MV->REGS[OP1].dato));
}

void div_(tMV *MV){
    int v1,v2;

    v1 = get(MV,MV->REGS[OP1].dato);
    v2 = get(MV,MV->REGS[OP2].dato);

    if(v2!=0){
        set(MV,MV->REGS[OP1].dato, v1 / v2);
        MV->REGS[AC].dato = v1 % v2; 

        setCC(MV,get(MV,MV->REGS[OP1].dato));
    }
    else
        divisionzero();
    
}



void cmp (tMV *MV){
    int v1, v2, result;

    v1 = get(MV,MV->REGS[OP1].dato);
    v2 = get(MV,MV->REGS[OP2].dato);

    setCC(MV,v1-v2);
    
}

void shl (tMV *MV){
    int v2;

    v2 = get(MV,MV->REGS[OP2].dato);

    set(MV,MV->REGS[OP1].dato, (get(MV,MV->REGS[OP1].dato)<<v2 ) );
}

void shr (tMV *MV){
    int v1,v2;
    unsigned int mask_aux;

    v2 = get(MV,MV->REGS[OP2].dato); 
    v1 = get(MV,MV->REGS[OP1].dato); 

    v1 = v1 >> v2;
    mask_aux = 0xFFFFFFFF >> v2;
    v1 &= mask_aux; 

    set(MV,MV->REGS[OP1].dato, v1);
} 

    
void sar(tMV *MV){
    int v1,v2;

    v2 = get(MV,MV->REGS[OP2].dato); 
    v1 = get(MV,MV->REGS[OP1].dato); 

    set(MV,MV->REGS[OP1].dato, v1>>v2 );
}


void and(tMV *MV) {
    int v1,v2;
    v1 = get(MV,MV->REGS[OP1].dato);
    v2 = get(MV,MV->REGS[OP2].dato);
    set(MV,MV->REGS[OP1].dato,v1&v2);

}

void or(tMV *MV){
    int v1,v2;
    v1 = get(MV,MV->REGS[OP1].dato);
    v2 = get(MV,MV->REGS[OP2].dato);
    set(MV,MV->REGS[OP1].dato,v1|v2);

}



void xor(tMV *MV){
    int v1,v2;
    v1 = get(MV,MV->REGS[OP1].dato);
    v2 = get(MV,MV->REGS[OP2].dato);
    set(MV,MV->REGS[OP1].dato,v1^v2);
}

void swap(tMV *MV) {
    int v1,v2,aux;

    v1 = get(MV,MV->REGS[OP1].dato);
    v2 = get(MV,MV->REGS[OP2].dato);
    aux = v1;
    set(MV,MV->REGS[OP1].dato,v2);
    set(MV,MV->REGS[OP2].dato,aux);
}

void ldl(tMV *MV){
    int v1,v2;
    v1 = get(MV,MV->REGS[OP1].dato);
    v2 = (get(MV,MV->REGS[OP2].dato))&0xFFFF;
    set(MV,MV->REGS[OP1].dato,v1|v2);

}

void ldh(tMV *MV) {
    int v1,v2;
    v1 = get(MV,MV->REGS[OP1].dato);
    v2 = (get(MV,MV->REGS[OP2].dato))&0xFFFF;
    v2 = v2<<16;
    set(MV,MV->REGS[OP1].dato,v1|v2);
}

void rnd(tMV *MV){
    int v2,aux;

    v2 = get(MV,MV->REGS[OP2].dato);
    aux = rand() % (v2 + 1);
    
    set(MV,MV->REGS[OP1].dato,aux);

}


// ERRORES ------------------------------------------

void segmentationfault() {
    printf("\nERROR! SEGMENTATION FAULT!");
    exit(1);
}

void invalidfunction() {
    printf("\nFUNCION INVALIDA!");
    exit(1);
}

void divisionzero() {
    printf("\nERROR! DIVISION POR CERO!");
    exit(1);
}