#include <stdio.h>
#include <stdlib.h>

void ejecucion(uint8_t *Mem, uint8_t tamcs, uint8_t *Ip){
    uint8_t top1, top2, ultimos5, bits_altos, reg=1, inmediato=2, memoria=3, i,j,N,M;
    uint32_t opc, op1, op2, aux;

    i=0;
    while(i<tamcs){

        bits_altos = (Mem[i] >> 4) & 0x0F; // desplazar 4 bits y enmascarar
        top2=(bits_altos>>2)&& 0x03;
        top1=bits_altos && 0x03;                    //obtengo top1 y top2

        if(top1=0){
            top1=top2;
            top2=0;
        }

        switch (top2){
            case reg:                       //analizo de que tipo son
                N=1;
                break;
            case inmediato:
                N=2;
                break;
            case memoria:
                N=3;
                break;
            default:
                N=0;
                break;
        }

        op2 = ((uint32_t)N) << 24;

        switch (top1){
            case reg:
                M=1;
                break;
            case inmediato:
                M=2;
                break;
            case memoria:
                M=3;
                break;
            default:
                M=0;
                break;
        }
        op1 = ((uint32_t)M) << 24;

        ultimos5 = Mem[i] & 0x1F;                   //obtengo tipo de operacion y mando a operar

        operaciones[ultimos5](top1,top2);

                                            //set operandos
        opc=ultimos5;                       //me lo lleva directamente a 4 bytes en hexa


        int32_t aux = 0x0;

        for (int j = 0; j < N; j++) {
            i++;  //avanzo mem

            if (j == 0) {
            // primer byte leído: propagar signo solo si es negativo
                if ((int8_t)Mem[i] < 0) {
                    aux = ((int32_t)(int8_t)Mem[i]) << (8 * (N - 1 - j));
                }
                else {
                    aux = ((uint32_t)Mem[i]) << (8 * (N - 1 - j));
                }
            }
            else {
            // resto de bytes: siempre sin signo
            aux |= ((uint32_t)Mem[i]) << (8 * (N - 1 - j));
            }
        }

        op2 |= aux;



        int32_t aux = 0x0;
        for (int j = 0; j < M; j++) {
            i++;  //avanzo mem

            if (j == 0) {
            // primer byte leído: propagar signo solo si es negativo
                if ((int8_t)Mem[i] < 0) {
                    aux = ((int32_t)(int8_t)Mem[i]) << (8 * (M - 1 - j));
                }
                else {
                    aux = ((uint32_t)Mem[i]) << (8 * (M - 1 - j));
                }
            }
            else {
            // resto de bytes: siempre sin signo
            aux |= ((uint32_t)Mem[i]) << (8 * (M - 1 - j));
            }
        }

        op1 |= aux;




        Mem[Ip]+=((op1 & 0xFF000000)>>24)+((op2 & 0xFF000000)>>24)+0x00000001;    //actualizo Ip

        if (top1==3 || top2==3){
            acceso_mem();
        }


        i++;        //avanzo a sig op
    }

}

//actualizado-2-check

int main()
{
    printf("\n");
    return 0;
}
