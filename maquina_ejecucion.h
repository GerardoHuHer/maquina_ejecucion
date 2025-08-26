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
    int16_t no_linea;
    TipoComando comando; 
    int16_t r, d, s, t;
};

extern int16_t* reg[NUMERO_REGISTROS]; // Registros de la máquina
extern int16_t* datos_Memoria[DATOS_MAXIMO];


void lexer(Instruccion* instrucciones, const std::string& path );

TipoComando stringToTipoComando(const std::string& command); 
void separar_variables(int16_t& r, int16_t& s, int16_t& t, int16_t& d, const std::string& operand_str);

#endif
