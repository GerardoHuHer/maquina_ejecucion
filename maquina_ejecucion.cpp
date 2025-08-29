#include "maquina_ejecucion.h"

int reg[NUMERO_REGISTROS];
int datos_Memoria[DATOS_MAXIMO];

TipoComando stringToTipoComando(const std::string& command, int linea) {
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
    std::cerr << "Comando desconocido: " << command << " en la línea " << linea << std::endl;
    std::exit(EXIT_FAILURE);    
    return HALT;
}

// Función que recibe las variables r, s, d, t, la cadena con sus valores y la instruccion para saber como dividirlos
void separar_variables( TipoComando instruccion, int& r, int& s, int& t, int& d, const std::string& operand_str, int linea) {
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
            try {
                r = std::stoi(temp_str);
            } catch (const std::invalid_argument& e) {
                std::cerr << "Error: valor inválido para r en la línea " << linea << std::endl;
                std::exit(EXIT_FAILURE);
            }
            s = 0; t = 0; d = 0;
            break;

        case ADD:
        case SUB:
        case MUL:
        case DIV:
            // Lee r, s y t, ignorando las comas.
            std::getline(ss, temp_str, ',');
            r = std::stoi(temp_str);
            std::getline(ss, temp_str, ',');
            s = std::stoi(temp_str);
            std::getline(ss, temp_str); // Lee hasta el final de la línea
            t = std::stoi(temp_str);
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
            r = std::stoi((temp_str));
            std::getline(ss, temp_str, '('); // Lee hasta el paréntesis
            d = std::stoi((temp_str));
            std::getline(ss, temp_str, ')'); // Lee el valor de 's'
            s = std::stoi((temp_str));
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
            int no_linea = std::stoi(num_str);
            instrucciones[index].no_linea = no_linea;
            if (no_linea >= 0 && no_linea < INSTRUCCIONES_MAXIMO) {
                instrucciones[index].no_linea = no_linea;

                std::string command_str;
                ss >> command_str;
                instrucciones[index].comando = stringToTipoComando(command_str, no_linea);


                std::string operand_str;
                ss >> operand_str;

                separar_variables(instrucciones[index].comando, instrucciones[index].r, instrucciones[index].s, instrucciones[index].t, instrucciones[index].d, operand_str, no_linea);
                index++;

                continue;
            } else {
                std::cerr << "Número de línea fuera de rango: " << no_linea << std::endl;
            }
        } else {
            std::cout << "Error: la línea inicia con un caracter inválido: " << line << std::endl;
            std::exit(EXIT_FAILURE);
        }
    }
    inputFile.close();
}

void ejecutar_codigo(Instruccion* instrucciones) {
    // Inicializamos el registro del Program Counter (registro 7) en 0.
    reg[PC_REGISTRO] = 0;
    int a;

    while (reg[PC_REGISTRO] < INSTRUCCIONES_MAXIMO && instrucciones[reg[PC_REGISTRO]].comando != HALT) {
        // Obtenemos la instrucción actual
        Instruccion current_instruction = instrucciones[reg[PC_REGISTRO]];

        // El switch ahora maneja la ejecución de la instrucción.
        switch (current_instruction.comando) {
            case IN:
                std::cout << "? ";
                std::cin >> reg[current_instruction.r];
                break;
            case OUT:
                std::cout << "> " << reg[current_instruction.r] << std::endl;



                break;
            case ADD:
                reg[current_instruction.r] = reg[current_instruction.s] + reg[current_instruction.t];
                break;
            case SUB:
                reg[current_instruction.r] = reg[current_instruction.s] - reg[current_instruction.t];
                break;
            case MUL:
                reg[current_instruction.r] = reg[current_instruction.s] * reg[current_instruction.t];
                break;
            case DIV:
                if (reg[current_instruction.t] == 0) {
                    std::cerr << "Error: División por cero en la instrucción en línea " << current_instruction.no_linea << std::endl;
                    return;
                }
                reg[current_instruction.r] = reg[current_instruction.s] / reg[current_instruction.t];
                break;
            case LD:
                a = current_instruction.d + reg[current_instruction.s];
                if (a < 0 || a >= DATOS_MAXIMO) {
                    std::cerr << "Error: Acceso a memoria fuera de límites en la línea " << current_instruction.no_linea << std::endl;
                    return;
                }
                reg[current_instruction.r] = datos_Memoria[a];
                break;
            case LDA:
                reg[current_instruction.r] = current_instruction.d + reg[current_instruction.s];
                break;
            case LDC:
                reg[current_instruction.r] = current_instruction.d;
                break;
            case ST:
                a = current_instruction.d + reg[current_instruction.s];
                if (a < 0 || a >= DATOS_MAXIMO) {
                    std::cerr << "Error: Acceso a memoria fuera de límites en la línea " << current_instruction.no_linea << std::endl;
                    return;
                }
                datos_Memoria[a] = reg[current_instruction.r];
                break;
            case JLT:
                if (reg[current_instruction.r] < 0) {
                    //reg[PC_REGISTRO] = current_instruction.d;
                    reg[PC_REGISTRO] = current_instruction.d + reg[current_instruction.s];
                    continue; // Continuamos el bucle sin incrementar el PC
                }
                break;
            case JLE:
                if (reg[current_instruction.r] <= 0) {
                    //reg[PC_REGISTRO] = current_instruction.d;
                    reg[PC_REGISTRO] = current_instruction.d + reg[current_instruction.s];
                    continue;
                }
                break;
            case JGE:
                if (reg[current_instruction.r] >= 0) {
                    //reg[PC_REGISTRO] = current_instruction.d;
                    reg[PC_REGISTRO] = current_instruction.d + reg[current_instruction.s];
                    continue;
                }
                break;
            case JGT:
                if (reg[current_instruction.r] > 0) { 


                    reg[PC_REGISTRO] = current_instruction.d + reg[current_instruction.s];
                    continue;
                }
                break;
            case JEQ:
                if (reg[current_instruction.r] == 0) {
                    //reg[PC_REGISTRO] = current_instruction.d + reg[instrucciones[]];

                    reg[PC_REGISTRO] = current_instruction.d + reg[current_instruction.s];
                    continue;
                }
                break;
            case JNE:
                if (reg[current_instruction.r] != 0) {
                    reg[PC_REGISTRO] = current_instruction.d + reg[current_instruction.s];
                    continue;
                }
                break;
        }

        // Si no se ejecutó un salto, incrementamos el PC para pasar a la siguiente instrucción.
        reg[PC_REGISTRO]++;
    }
}
