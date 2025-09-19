#ifndef MAQUINA_EJECUCION_H
#define MAQUINA_EJECUCION_H

#include <iostream>
#include <string>
#include <sstream>
#include <cstdint>
#include <fstream>
#include <algorithm>

const int INSTRUCCIONES_MAXIMO = 1024; // Cantidad máxima de intrucciones que acepta la máquina virtual
const int DATOS_MAXIMO = 2048;  
const int NUMERO_REGISTROS = 8; // Cantidad de registros que tiene la máquina virtual
const int PC_REGISTRO = 7; // Registro del contador de programa

// Enum de tipos de comandos para representar los lexemas de las instrucciones
enum TipoComando{ 
    HALT, IN, OUT, ADD, SUB, MUL, DIV, 
    LD, LDA, LDC, ST, 
    JLT, JLE, JGE, JGT, JEQ, JNE, 
};

// Estructura para representar cada línea de instrucciones del archivo a leer
// Contiene los registros, r,d,s,t, no_linea y el tiupo de comando que se ejecutará
struct Instruccion{
    int no_linea;
    TipoComando comando; 
    int r, d, s, t;
};

extern int reg[NUMERO_REGISTROS]; // Registros de la máquina
extern int datos_Memoria[DATOS_MAXIMO]; // Arreglo de memoria de datos

/*
@brief Función que se encarga de analizar el archivo de instrucciones y llenar el arreglo de instrucciones.
@param instrucciones Puntero a la memoria de instrucciones.
@param path Ruta del archivo a analizar.
*/
void lexer(Instruccion* instrucciones, const std::string& path );

/*
@brief Función para convertir una cadena (string) a un tipo de comando.
@param command Comando en formato string.
@param line Número de línea del comando (En caso de error)
*/
TipoComando stringToTipoComando(const std::string& command, int line); 

/*
@brief Función para separar los operandos de una instrucción en sus componentes r, s, t, d.
@param instruccion Tipo de instrucción.
@param r Registro de destino.
@param s Registro fuente 1.
@param t Registro fuente 2.
@param d Valor inmediato.
@param operand_str Cadena con los operandos.
@param line Número de línea (En caso de error).
*/
void separar_variables( TipoComando instruccion, int& r, int& s, int& t, int& d, const std::string& operand_str, int);

/*
@brief Función para ejecutar el código de las instrucciones en memoria.
@param arr Puntero a la memoria de instrucciones.
*/
void ejecutar_codigo(Instruccion* arr);

void ejecutar_codigo_debug(Instruccion* arr);

void imprimir_estado_maquina(Instruccion instruccion_actual);

void imprimir_instrucciones(Instruccion* instrucciones);


#endif
