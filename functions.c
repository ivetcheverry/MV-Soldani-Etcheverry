#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "const.h"
#include "functions.h"
#include <time.h>

void mostrar(tMV *MV, int OPA, int OPB){
    int t1,t2,r,off;
    t1=t2=r=off=0;

    t1 = OPA>>24;
    t2 = OPB>>24;

    if (t1==MEM){
        r = (OPA>>16) & 0xFF;
        off=OPA&0xFFFF;
        printf("  [%s+%d], ",MV->REGS[r].nombre,off);
    }
    else if (t1==REG){
        r=OPA&0XFF;
        printf("  %s, ",MV->REGS[r].nombre);
    }
    else
        printf("  %d  ",OPA&0XFF);


    if (t2==MEM){
        r = (OPB>>16)&0x1F;
        off=OPB&0xFFFF;
        printf("  [%s+%d], ",MV->REGS[r].nombre,off);
    }
    else if (t2==REG){
        r=OPB&0X1F;
        printf("  %s ",MV->REGS[r].nombre);
    }
    else { //INMEDIATO
        r = OPB&0XFFFF;
        if (r & 0b1000000)
        r= (r ^ NMASK) - NMASK; 
        printf("  %d  ",r);
    }
        
    
}


// SIN OPERANDOS ---------------------------------------------------------------
void stop(tMV *MV) {
    printf("\n\n STOP ejecutado. Deteniendo la máquina virtual.");
    MV->REGS[IP].dato =-1;
}


// 1 OPERANDO ------------------------------------------------------------------
void sys(tMV *MV){
    int f,valor,formato;
    char binario[33];

    valor=0;
    f = get(MV,MV->REGS[OP2].dato);
    formato = MV->REGS[EAX].dato&0XFF;

    if (f==1){ //READ   
        printf("INGRESAR VALOR: "); 
        if (formato==1)
            scanf("%d",&valor);
        else if (formato==2)
            scanf("%c",&valor);
        else if (formato==4)
            scanf("%o",&valor);
        else if (formato==8)
            scanf("%x",&valor);
        else {
            scanf("%s",binario);
            for (int i=strlen(binario)-1 ; i>=0 ; i--){
                valor=valor<<1;
                valor|=binario[i]-'0';
            }
        }
        MV->MEMORIA[(MV->REGS[EDX].dato &0X1F) + MV->SEGMENTTABLE[1][0]]= valor;
    }  
    else { //WRITE
        valor= MV->MEMORIA[(MV->REGS[EDX].dato &0X1F) + MV->SEGMENTTABLE[1][0]];
        if (formato==1)
            printf("RESULTADO: %d",valor);
        else if (formato==2)
            printf("RESULTADO: %c",valor);
        else if (formato==4)
            printf("RESULTADO: %o",valor);
        else if (formato==8)
            printf("RESULTADO: %x",valor);
        else { //ESCRIBIR BINARIO
            int i=32;
            while (valor > 0 && i > 0) {
            i--;
            binario[i] = (valor & 1) ? '1' : '0';
            valor = valor >> 1;
        }
        printf("RESULTADO: 0b%s", &binario[i]);
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
    printf("\nCC: %x \n",MV->REGS[CC].dato);

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
    int N = (MV->REGS[CC].dato >> 31) & 0b1 ; 
    if (N & 1)
       MV->REGS[IP].dato=get(MV, MV->REGS[OP2].dato);
}

void not_(tMV *MV){
    set(MV, MV->REGS[OP2].dato, ~ MV->REGS[OP2].dato);

    setCC(MV,get(MV,MV->REGS[OP2].dato));
}


// 2 OPERANDOS -----------------------------------------------------------------

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
    resta = get(MV,MV->REGS[OP1].dato)-get(MV,MV->REGS[OP2].dato);
    set(MV,MV->REGS[OP1].dato,resta);

    setCC(MV,get(MV,MV->REGS[OP1].dato));
}

void mul (tMV *MV){
    int v1, v2;
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
        set(MV,MV->REGS[MBR].dato, v1 / v2);
        set(MV,MV->REGS[AC].dato, v1 % v2);   //resto en AC

        setCC(MV,get(MV,MV->REGS[MBR].dato));
    }
    else
        printf("Error, division por 0");
    
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

    v2 = get(MV,MV->REGS[OP2].dato); 
    v1 = get(MV,MV->REGS[OP1].dato); 

    set(MV,MV->REGS[OP1].dato, ((unsigned)v1>>v2 ));
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

    aux= v1 = get(MV,MV->REGS[OP1].dato);
    v2 = get(MV,MV->REGS[OP2].dato);

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
    srand(time(NULL));

    v2 = get(MV,MV->REGS[OP2].dato);
    aux = rand() % (v2 + 1);
    
    set(MV,MV->REGS[OP1].dato,aux);

}
