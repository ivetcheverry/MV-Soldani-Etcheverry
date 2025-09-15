#include <stdio.h>
#include <stdlib.h>
#include "const.h"
#include "functions.h"
#include <time.h>

void stop(int memoria[]) {
    printf("STOP ejecutado. Deteniendo la máquina virtual.\n");
    memoria[IP]=-1;
    break;
}
void jmp (int *memoria, int op1){
    memoria[IP]= get(memoria, op1 & 0x00FFFFFF ,(op1 >> 24) & 0xFF);
}

void jz (int *memoria, int op1){
    uint8_t aux;
    aux= (memoria[CC] >> 30) & 0b01;
    if (aux==1)
        memoria[IP]=get(memoria, op1 & 0x00FFFFFF,(op1 >> 24) & 0xFF);
}

void jnz (int *memoria, int op1){
    uint8_t aux;

    aux= (memoria[CC] >> 30) & 0b01;
    if (aux==0)
        memoria[IP]=get(memoria, op1 & 0x00FFFFFF ,(op1 >> 24) & 0xFF);
}

void jn (int *memoria, int op1){
    uint8_t aux;

    aux= (memoria[CC] >> 31) & 0b1;
    if (aux==1)
        memoria[IP]=get(memoria, op1 & 0x00FFFFFF ,(op1 >> 24) & 0xFF);
}

void jnn (int *memoria, int op1){
    uint8_t aux;
    aux= (memoria[CC] >> 31) & 0b1;
    if (aux==0)
        memoria[IP]=get(memoria, op1 & 0x00FFFFFF ,(op1 >> 24) & 0xFF);
}

void jp (int *memoria, int op1){
    uint8_t aux;

    aux= (memoria[CC] >> 31) & 0b1;
    if (aux==0)
        memoria[IP]=get(memoria, op1 & 0x00FFFFFF ,(op1 >> 24) & 0xFF);
}

void JNP (int *memoria, int op1){
    uint8_t aux;

    aux= (memoria[CC] >> 31) & 0b1;
    if (aux==1)
        memoria[IP]=get(memoria, op1 & 0x00FFFFFF ,(op1 >> 24) & 0xFF);
}

void not (int *memoria, int op1, int CC){
    memoria[op1 & 0x00FFFFFF]= ~get(memoria, op1 & 0x00FFFFFF ,(op1 >> 24) & 0xFF);
}

void mov(int memoria[],int a,int b, int atype, int btype){
    set(memoria,a,atype,get(memoria,b,btype));
}


//CAMBIAR 
void add(int memoria[],int a, int b, int atype, int btype){
    if (atype==REG)
        if (btype==INM) //tipo INSTANTANEO
            memoria[a]+=b;
        else if (btype==REG) //tipo REGISTRO
            memoria[a]+=memoria[b];
        else //tipo MEMORIA
            memoria[a]+=memoria[memoria[b]];

    else //es tipo Memoria
        if (btype==INM) //tipo INSTANTANEO
            memoria[memoria[a]]+=b;
        else if (btype==REG) //tipo REGISTRO
            memoria[memoria[a]]+=memoria[b];
        else //tipo MEMORIA
            memoria[memoria[a]]+=memoria[memoria[b]];
}

void sub (int memoria[],int a, int b, int atype, int btype){
    int v1, v2;

    v1=get(memoria, a & 0x00FFFFFF ,(a >> 24) & 0xFF);
    v2=get(memoria, b & 0x00FFFFFF ,(b >> 24) & 0xFF);
    set(memoria,a,atype, v1-v2);
}

void mul (int memoria[],int a, int b, int atype, int btype){
    int v1, v2;

    v1=get(memoria, a & 0x00FFFFFF ,(a >> 24) & 0xFF);
    v2=get(memoria, b & 0x00FFFFFF ,(b >> 24) & 0xFF);
    set(memoria,a,atype, v1*v2);
}

void div (int memoria[],int a, int b, int atype, int btype){
    int v1, v2, result;

    v1=get(memoria, a & 0x00FFFFFF ,(a >> 24) & 0xFF);
    v2=get(memoria, b & 0x00FFFFFF ,(b >> 24) & 0xFF);
    if(v2!=0){
        result=(int)(v1/v2);
        set(memoria,a & 0x00FFFFFF,atype, result);
        set(memoria,16,1,(double)(v1/v2)-result)                                  //resto en AC
    }
    else{
        printf("Error, division por 0");
        break;
    }
}

void cmp (int memoria[],int a, int b, int atype, int btype){
    int v1, v2, result;

    v1=get(memoria, a & 0x00FFFFFF ,(a >> 24) & 0xFF);
    v2=get(memoria, b & 0x00FFFFFF ,(b >> 24) & 0xFF);

    v1-=v2;
    if(v1==0){                                                  //actualizo CC
        memoria[17]|= 1u << 30;
    }
    else{
        memoria[17]&= ~(1u << 30);
    }
    if(v1<0){
        memoria[17] |= (1u << 31);
    }
    else{
        &= ~(1u << 31);
    }
}

void shl (int memoria[],int a, int b, int atype, int btype){
    int v2;

    v2=get(memoria, b & 0x00FFFFFF ,(b >> 24) & 0xFF);
    memoria[a & 0x00FFFFFF ]=memoria[a & 0x00FFFFFF ]<<v2;
}

void shr (int memoria[],int a, int b, int atype, int btype){
    int v2;

    v2=get(memoria, b & 0x00FFFFFF ,(b >> 24) & 0xFF);
    memoria[a & 0x00FFFFFF ]=memoria[a & 0x00FFFFFF ]>>v2;
}