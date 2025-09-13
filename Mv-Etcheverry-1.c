// CONSTANTES EN MAYUS
#include <stdio.h>
#include <stdlib.h>
#define VERSION 1

void validar(int []);
void inicializar_memoria(int []);
void mostrar(FILE *);

int main()
{
    int memoria[16384];
    inicializar_memoria(memoria);
    validar(memoria);

    int total=0, n;
    printf("Ingrese un numero natural a sumar (0 para terminar)\n",n);
    scanf("%d",&n);
    while (n!=0)
        {
        total+=n;
        printf("Ingrese un numero natural a sumar \n",n);
        scanf("%d",&n);
        }
    printf("La suma resultante es: %d",total);
    return 0;
}

void inicializar_memoria(int memoria[]){
    for (int i=0;i<100;i++)
        memoria[i]=0;
}

void mostrar(FILE *a){
    int aux,i;

    while (fread(&aux,1,1,a)){
        printf("%3x",aux);
    }
}

void validar(int memoria[16328]) {
    FILE *arch = fopen("sample.vmx","rb");
    //mostrar(arch);
    int control[]= {'V','M','X','2','5'};
    int aux,i=0,sizecode=0;
    if (arch){
            fread(&aux,1,1,arch); //CONTROLO LOS CARACTERES "VMX25"
            while (i<5 && aux-control[i]==0 ) {
                printf("%3x",aux);
                i++;
                fread(&aux,1,1,arch);
            }
            if (i<5){
                printf("\n ERROR, ARCHIVO NO VALIDO!");
            }
            else{//CONTROLO VERSION
                printf("\n VERSION: %d \n", aux);
                if (aux!=VERSION)
                    printf("\n VERSION NO SOPORTADA!");
                else { //PASAR AL CODE SEGMENT
                    fread(&aux,1,1,arch);
                    sizecode+=aux;
                    fread(&aux,1,1,arch);
                    sizecode+=aux;
                    for (i=0;i<sizecode;i++){
                        fread(&aux,1,1,arch);
                        memoria[i]=aux;
                        printf("%5x",memoria[i]);
                    }
                    printf("\n CODE SEGMENT CARGADO CORRECTAMENTE! \n ");
                }  
            }
            fclose(arch);
        }
    else
        printf("EL ARCHIVO NO SE PUDO ABRIR!!");
}