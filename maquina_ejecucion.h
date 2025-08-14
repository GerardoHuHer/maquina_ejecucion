// Usaremos short int porque será de 16 bits

#ifdef MAQUINA_EJECUCION_H
#define MAQUINA_EJECUCION_H

const int INSTRUCCIONES_MAXIMO = 1024; 
const int DATOS_MAXIMO = 2048; 
const int NUMERO_REGISTROS = 8; 
const int PC_REGISTRO = 7; 

enum TipoComando{ 
    HALT, IN, OUT, ADD, SUB, MUL, DIV, 
    LD, LDA, LDC, ST, 
    JLT, JLE, JGE, JGT, JEQ, JNE, 
    NONE 
} 

struct Instruccion{
    TipoComando comando; 
    short r, d, s, t;
}

extern short* reg[NUMERO_REGISTROS]; // Registros de la máquina
extern short* datos_Memoria[DATOS_MAXIMO];


#endif