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

// FIX: Verificar espacios en blanco en los registros para que no marque error al usar función stoi
// 6,0,0 funciona, pero 6, 0, 0 no
std::string remove_whitespace(const std::string& str) {
    std::string aux;
    unsigned int i = 0;
    while(str[i] != '\0'){
        if(str[i] != ' ' && str[i] != '\t'){
            aux += str[i];
        }
        i++;
    }
    return aux;
}


// Función que recibe las variables r, s, d, t, la cadena con sus valores y la instruccion para saber como dividirlos
void separar_variables( TipoComando instruccion, int16_t& r, int16_t& s, int16_t& t, int16_t& d, const std::string& operand_str) {
    // Para instrucción HALT donde inicializamos todos los valores en 0
    if(instruccion == HALT){
        r = 0; s = 0; t = 0; d = 0; 
    }    
    std::stringstream ss(operand_str);
    std::string temp_str;
    char comma, parentesis;

    switch (instruccion) {
        case HALT:
            r = 0; s = 0; t = 0; d = 0;
            break;

        case IN:
        case OUT:
            // Lee la cadena, ignora los espacios y convierte a int.
            ss >> temp_str;
            r = static_cast<int16_t>(std::stoi(remove_whitespace(temp_str)));
            s = 0; t = 0; d = 0;
            break;

        case ADD:
        case SUB:
        case MUL:
        case DIV:
            // Lee r, s y t, ignorando las comas.
            std::getline(ss, temp_str, ',');
            r = static_cast<int16_t>(std::stoi(remove_whitespace(temp_str)));
            std::getline(ss, temp_str, ',');
            s = static_cast<int16_t>(std::stoi(remove_whitespace(temp_str)));
            std::getline(ss, temp_str); // Lee hasta el final de la línea
            t = static_cast<int16_t>(std::stoi(remove_whitespace(temp_str)));
            d = 0;
            break;

        case LD:
        case LDA:
        case LDC:
        case ST:
        case JLT:
        case JLE:
        case JGE:
        case JGT:
        case JEQ:
        case JNE:
            // Lee r, d, y s.
            std::getline(ss, temp_str, ',');
            r = static_cast<int16_t>(std::stoi(remove_whitespace(temp_str)));
            std::getline(ss, temp_str, '('); // Lee hasta el paréntesis
            d = static_cast<int16_t>(std::stoi(remove_whitespace(temp_str)));
            std::getline(ss, temp_str, ')'); // Lee el valor de 's'
            s = static_cast<int16_t>(std::stoi(remove_whitespace(temp_str)));
            t = 0;
            break;

        default:
            r = 0; s = 0; t = 0; d = 0;
            break;        
    }       
    
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
        if (first_char_pos == std::string::npos || line[first_char_pos] == '*' ) {
            continue;
        }

        // Transformamos la línea en stringstream 
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


            std::string operand_str;
            ss >> operand_str;

            separar_variables(instrucciones[no_linea].comando, instrucciones[no_linea].r, instrucciones[no_linea].s, instrucciones[no_linea].t, instrucciones[no_linea].d, operand_str);

            continue;
        }
         else {
            std::cerr << "Número de línea fuera de rango: " << no_linea << std::endl;
        
        }
    }

    inputFile.close();
}
