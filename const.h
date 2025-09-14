typedef struct tfunc_2op {
    char nombre[5];
    void (*func)(int memoria[],int a,int b, int atype, int btype);//vector con punteros a funciones void con 2 parámetros
}tfunc_2op ;

typedef struct  tfunc_1op  {
    char nombre[5];
    void (*func)(int memoria[],int a, int atype); //vector con punteros a funciones void con 1 parámetros
}tfunc_1op ;


// CONSTANTES EN MAYUS
#define VERSION 1
#define TABLA_N 2
#define TABLA_M 2
#define RAM 16328

#define REG 1
#define INM 2
#define MEM 3


//INSTRUCCIONES
// Instrucciones con dos operandos
#define MOV   0
#define ADD   1
#define SUB   2
#define MUL   3
#define DIV   4
#define CMP   5
#define SHL   6
#define SHR   7
#define SAR   8
#define AND   9
#define OR    10
#define XOR   11
#define SWAP  12
#define LDL   13
#define LDH   14
#define RND   15

// Instrucciones con un operando
#define SYS   0
#define JMP   1
#define JZ    2
#define JP    3
#define JN    4
#define JNZ   5
#define JNP   6
#define JNN   7
#define NOT   8

//REGISTROS
// Acceso a memoria
#define LAR   0
#define MAR   1
#define MBR   2

// Instrucción
#define IP    3
#define OPC   4
#define OP1   5
#define OP2   6

// Reservados
#define REG7  7
#define REG8  8
#define REG9  9

// Registros de propósito general
#define EAX   10
#define EBX   11
#define ECX   12
#define EDX   13
#define EEX   14
#define EFX   15

// Acumulador
#define AC    16

// Código de condición
#define CC    17

// Reservados
#define REG18 18
#define REG19 19
#define REG20 20
#define REG21 21
#define REG22 22
#define REG23 23
#define REG24 24
#define REG25 25

// Segmentos
#define CS    26
#define DS    27

// Reservados
#define REG28 28
#define REG29 29
#define REG30 30
#define REG31 31
