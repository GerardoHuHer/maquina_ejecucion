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
    return HALT;
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
            r = static_cast<int16_t>(std::stoi(temp_str));
            s = 0; t = 0; d = 0;
            break;

        case ADD:
        case SUB:
        case MUL:
        case DIV:
            // Lee r, s y t, ignorando las comas.
            std::getline(ss, temp_str, ',');
            r = static_cast<int16_t>(std::stoi(temp_str));
            std::getline(ss, temp_str, ',');
            s = static_cast<int16_t>(std::stoi(temp_str));
            std::getline(ss, temp_str); // Lee hasta el final de la línea
            t = static_cast<int16_t>(std::stoi(temp_str));
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
            r = static_cast<int16_t>(std::stoi((temp_str)));
            std::getline(ss, temp_str, '('); // Lee hasta el paréntesis
            d = static_cast<int16_t>(std::stoi((temp_str)));
            std::getline(ss, temp_str, ')'); // Lee el valor de 's'
            s = static_cast<int16_t>(std::stoi((temp_str)));
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
    unsigned int index = 0;
    while (std::getline(inputFile, line)) {


        if(std::all_of(line.begin(), line.end(), [](unsigned char c) {
            return std::isspace(c);
        })) {
            continue;
        }

        // Encontrar la posición del primer carácter no-espacio.
        unsigned int first_char_pos = line.find_first_not_of(" \t\n\r");


        if (first_char_pos == std::string::npos || line[first_char_pos] == '*' ) {
            continue;
        }

        char first_char = line[first_char_pos];

        if(std::isdigit(static_cast<unsigned char>(first_char))){
            
            std::stringstream ss(line);
            std::string num_str;
            // Transformamos la línea en stringstream
            std::getline(ss, num_str, ':'); // Leemos el número de línea
            int16_t no_linea = static_cast<int16_t>(std::stoi(num_str));
            instrucciones[index].no_linea = no_linea;
            if (no_linea >= 0 && no_linea < INSTRUCCIONES_MAXIMO) {
                instrucciones[index].no_linea = no_linea;

                std::string command_str;
                ss >> command_str;
                instrucciones[index].comando = stringToTipoComando(command_str);


                std::string operand_str;
                ss >> operand_str;

                separar_variables(instrucciones[index].comando, instrucciones[index].r, instrucciones[index].s, instrucciones[index].t, instrucciones[index].d, operand_str);
                index++;

                continue;
            } else {
                std::cerr << "Número de línea fuera de rango: " << no_linea << std::endl;
            }
        } else {
            std::cout << "error: la línea inicia con un caracter inválido: " << line << std::endl;
            std::exit(EXIT_FAILURE);
        }
    }
    inputFile.close();
}
