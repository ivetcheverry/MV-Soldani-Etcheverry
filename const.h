// CONSTANTES EN MAYUS

#define TABLA_N 8
#define NMASK 0x8000
#define RAM 16328

#define baseCS 0 
#define baseDS 1

#define REG 1
#define INM 2
#define MEMO 3


struct tMV;

typedef struct tfunc {
    char nombre[5];
    void (*func)(struct tMV *);//vector con punteros a funciones
}tfunc;

typedef struct treg{
    char nombre[5];
    int dato;
}treg;

typedef struct tMV {
    int CSsize;
    int SEGMENTTABLE[TABLA_N];

    int MEMORIA[RAM];
    tfunc FUNCIONES[32];
    treg REGS[32];
    int DISSASEMBLER;
}tMV;




#define RMASK 0xFF000000 //mascara para obtener tipo de los OP1 y OP2
#define AMASK 0x0FFF; //mascara para obtener valor de los OP1 y OP2


//INSTRUCCIONES
// Dos operandos
#define MOV  16
#define ADD  17
#define SUB  18
#define MUL  19
#define DIV_  20
#define CMP  21
#define SHL  22
#define SHR  23
#define SAR  24
#define AND  25
#define OR   26
#define XOR  27
#define SWAP 28
#define LDL  29
#define LDH  30
#define RND  31

// Un operando
#define SYS  0
#define JMP  1
#define JZ   2
#define JP   3
#define JN_  4
#define JNZ  5
#define JNP  6
#define JNN  7
#define NOT_ 8

// Sin operandos
#define STOP 15

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
