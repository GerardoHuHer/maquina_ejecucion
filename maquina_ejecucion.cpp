#include "maquina_ejecucion.h"

TipoComando stringToTipoComando(const std::string& command) {
    if (command == "HALT") return HALT;
    if (command == "IN") return IN;
    if (command == "OUT") return OUT;
    if (command == "ADD") return ADD;
    if (command == "SUB") return SUB;
    if (command == "MUL") return MUL;
    if (command == "DIV") return DIV;
    if (command == "LD") return LD;
    if (command == "LDA") return LDA;
    if (command == "LDC") return LDC;
    if (command == "ST") return ST;
    if (command == "JLT") return JLT;
    if (command == "JLE") return JLE;
    if (command == "JGE") return JGE;
    if (command == "JGT") return JGT;
    if (command == "JEQ") return JEQ;
    if (command == "JNE") return JNE;
    return NONE;
    
}

/*Función para leer el archivo y almacenar las instrucciones en el arreglo instrucciones*/
void lexer(Instruccion* instrucciones, const std::string& path) {
    std::ifstream inputFile(path);
    // Verificamos que el archivo se haya abierto correctamente
    if(!inputFile.is_open()) {
        std::cerr << "Error al abrir el archivo: " << path << std::endl;
        return;
    } 

    std::string line;
    while (std::getline(inputFile, line)) {
        // Encontrar la posición del primer carácter no-espacio.
        unsigned int first_char_pos = line.find_first_not_of(" \t\n\r");

        // FIX: Arreglar si detecta una línea en blanco saltar
        if (first_char_pos == std::string::npos || line[first_char_pos] == '*') {
            continue;
        }
        std::stringstream ss(line);
        std::string num_str;
        std::getline(ss, num_str, ':'); // Leemos el número de línea
        int16_t no_linea = std::stoi(num_str);
        instrucciones[no_linea].no_linea = no_linea;

         if (no_linea >= 0 && no_linea < INSTRUCCIONES_MAXIMO) {
            instrucciones[no_linea].no_linea = no_linea;

            std::string command_str;
            ss >> command_str;
            instrucciones[no_linea].comando = stringToTipoComando(command_str);

            // TODO: Lectura y almacenamiento de r, s, t y d
            continue;



        }
         else {
            std::cerr << "Número de línea fuera de rango: " << no_linea << std::endl;
        
        }
    }

    inputFile.close();
}
