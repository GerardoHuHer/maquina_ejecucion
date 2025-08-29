// Usaremos short porque será de 16 bits

#ifndef MAQUINA_EJECUCION_H
#define MAQUINA_EJECUCION_H

#include <iostream>
#include <string>
#include <sstream>
#include <cstdint>
#include <fstream>
#include <algorithm>

// Constantes de la máquina para definir registros y memoria
const int INSTRUCCIONES_MAXIMO = 1024; 
const int DATOS_MAXIMO = 2048; 
const int NUMERO_REGISTROS = 8; 

const int PC_REGISTRO = 7; 

// Lexemas del lenguaje
enum TipoComando{ 
    HALT, IN, OUT, ADD, SUB, MUL, DIV, 
    LD, LDA, LDC, ST, 
    JLT, JLE, JGE, JGT, JEQ, JNE, 
};

// Estructura para representar cada línea
struct Instruccion{
    int no_linea;
    TipoComando comando; 
    int r, d, s, t;
};

extern int reg[NUMERO_REGISTROS]; // Registros de la máquina
extern int datos_Memoria[DATOS_MAXIMO];


void lexer(Instruccion* instrucciones, const std::string& path );

TipoComando stringToTipoComando(const std::string& command, int); 

void separar_variables( TipoComando instruccion, int& r, int& s, int& t, int& d, const std::string& operand_str, int);
void ejecutar_codigo(Instruccion* arr);

#endif
