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
    if(command == "HALT") return HALT;
    std::cerr << "Comando desconocido: " << command << " en la línea " << linea << std::endl;
    std::exit(EXIT_FAILURE);    
    return HALT;
}

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
            try{

            r = stoi(temp_str); 
            } catch(const std::invalid_argument& e){
                std::cerr << "Error: Operando inválido para la instrucción en línea " << linea << std::endl;
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
            try{

            r = stoi(temp_str);
            } catch(const std::invalid_argument& e){
                std::cerr << "Error: Operando inválido para la instrucción en línea " << linea << std::endl;
                std::exit(EXIT_FAILURE);
            }
            std::getline(ss, temp_str, ',');
            try{
                s = stoi(temp_str);
            } catch(const std::invalid_argument& e){
                std::cerr << "Error: Operando inválido para la instrucción en línea " << linea << std::endl;
                std::exit(EXIT_FAILURE);
            }
            std::getline(ss, temp_str); // Lee hasta el final de la línea
            try{
                t = stoi(temp_str);
            } catch(const std::invalid_argument& e){
                std::cerr << "Error: Operando inválido para la instrucción en línea " << linea << std::endl;
                std::exit(EXIT_FAILURE);
            }
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
            try{
                r = stoi(temp_str);
            } catch(const std::invalid_argument& e){
                std::cerr << "Error: Operando inválido para la instrucción en línea " << linea << std::endl;
                std::exit(EXIT_FAILURE);
            }

            std::getline(ss, temp_str, '('); // Lee hasta el paréntesis
            try {
                d = std::stoi((temp_str));
            } catch(const std::invalid_argument& e){
                std::cerr << "Error: Operando inválido para la instrucción en línea " << linea << std::endl;
                std::exit(EXIT_FAILURE);
            }
            std::getline(ss, temp_str, ')'); // Lee el valor de 's'
            try{
                s = std::stoi(temp_str);
            } catch(const std::invalid_argument& e){
                std::cerr << "Error: Operando inválido para la instrucción en línea " << linea << std::endl;
                std::exit(EXIT_FAILURE);
            }
            t = 0;
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


        // Validación con función lambda para líneas en blanco
        if(std::all_of(line.begin(), line.end(), [](unsigned char c) {
            return std::isspace(c);
        })) {
            continue;
        }

        // Encontrar la posición del primer carácter no-espacio.
        // Devuelve un npos si la línea está vacía o contiene solo espacios.
        unsigned int first_char_pos = line.find_first_not_of(" \t\n\r");



        if (first_char_pos == std::string::npos || line[first_char_pos] == '*' ) {
            continue;
        }

        char first_char = line[first_char_pos];

        if(std::isdigit(static_cast<unsigned char>(first_char))){
            
            std::stringstream line_stream(line);
            std::string num_str;
            // Transformamos la línea en stringstream
            std::getline(line_stream, num_str, ':'); // Leemos el número de línea
            int no_linea = std::stoi(num_str);
            //instrucciones[no_linea].no_linea = no_linea;
            if (no_linea >= 0 && no_linea < INSTRUCCIONES_MAXIMO) {
                instrucciones[no_linea].no_linea = no_linea;

                std::string command_str;
                line_stream >> command_str;
                instrucciones[no_linea].comando = stringToTipoComando(command_str, no_linea);


                std::string operand_str;
                line_stream >> operand_str;

                separar_variables(instrucciones[no_linea].comando, instrucciones[no_linea].r, instrucciones[no_linea].s, instrucciones[no_linea].t, instrucciones[no_linea].d, operand_str, no_linea);
                continue;
            } else {
                std::cerr << "Número de línea fuera de rango: " << no_linea << std::endl;
            }
        } else {
            //std::cout << "Error: la línea inicia con un caracter inválido: " << line << std::endl;
            std::cerr << "Error: la línea inicia con un caracter inválido: " << line << std::endl;
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
                reg[PC_REGISTRO]++;
                std::cout << "? ";
                std::cin >> reg[current_instruction.r];
                if (std::cin.fail()) {
                    std::cerr << "Error: el valor ingresado no es un número entero.\n";
                    std::exit(EXIT_FAILURE);
                }
                break;
            case OUT:
                reg[PC_REGISTRO]++;
                std::cout << "> " << reg[current_instruction.r] << std::endl;
                break;
            case ADD:
                reg[PC_REGISTRO]++;
                reg[current_instruction.r] = reg[current_instruction.s] + reg[current_instruction.t];
                break;
            case SUB:
                reg[PC_REGISTRO]++;
                reg[current_instruction.r] = reg[current_instruction.s] - reg[current_instruction.t];
                break;
            case MUL:
                reg[PC_REGISTRO]++;
                reg[current_instruction.r] = reg[current_instruction.s] * reg[current_instruction.t];
                break;
            case DIV:
                reg[PC_REGISTRO]++;
                if (reg[current_instruction.t] == 0) {
                    std::cerr << "Error: División por cero en la instrucción en línea " << current_instruction.no_linea << std::endl;
                    std::exit(EXIT_FAILURE);
                }
                reg[current_instruction.r] = reg[current_instruction.s] / reg[current_instruction.t];
                break;
            case LD:
                reg[PC_REGISTRO]++;
                a = current_instruction.d + reg[current_instruction.s];
                if (a < 0 || a >= DATOS_MAXIMO) {
                    std::cerr << "Error: Acceso a memoria fuera de límites en la línea " << current_instruction.no_linea << std::endl;
                    std::exit(EXIT_FAILURE);
                }
                reg[current_instruction.r] = datos_Memoria[a];
                break;
            case LDA:
                reg[PC_REGISTRO]++;
                reg[current_instruction.r] = current_instruction.d + reg[current_instruction.s];
                break;
            case LDC:
                reg[PC_REGISTRO]++;
                reg[current_instruction.r] = current_instruction.d;
                break;
            case ST:
                reg[PC_REGISTRO]++;
                a = current_instruction.d + reg[current_instruction.s];
                if (a < 0 || a >= DATOS_MAXIMO) {
                    std::cerr << "Error: Acceso a memoria fuera de límites en la línea " << current_instruction.no_linea << std::endl;
                    std::exit(EXIT_FAILURE);
                }
                datos_Memoria[a] = reg[current_instruction.r];
                break;
            case JLT:
                reg[PC_REGISTRO]++;
                if (reg[current_instruction.r] < 0) {
                    //reg[PC_REGISTRO] = current_instruction.d;
                    reg[PC_REGISTRO] = current_instruction.d + reg[current_instruction.s];
                    continue; // Continuamos el bucle sin incrementar el PC
                }                 
                break;
            case JLE:
                reg[PC_REGISTRO]++;
                if (reg[current_instruction.r] <= 0) {
                    //reg[PC_REGISTRO] = current_instruction.d;
                    reg[PC_REGISTRO] = current_instruction.d + reg[current_instruction.s];
                    continue;
                }
                break;
            case JGE:
                reg[PC_REGISTRO]++;
                if (reg[current_instruction.r] >= 0) {
                    //reg[PC_REGISTRO] = current_instruction.d;
                    reg[PC_REGISTRO] = current_instruction.d + reg[current_instruction.s];
                    continue;
                }
                break;
            case JGT:
                reg[PC_REGISTRO]++;
                if (reg[current_instruction.r] > 0) { 

                    reg[PC_REGISTRO] = current_instruction.d + reg[current_instruction.s];

                    continue;
                }
                break;
            case JEQ:
                reg[PC_REGISTRO]++;
                if (reg[current_instruction.r] == 0) {
                    reg[PC_REGISTRO] = current_instruction.d + reg[current_instruction.s];
                    continue;
                }
                break;
            case JNE:
                reg[PC_REGISTRO]++;
                if (reg[current_instruction.r] != 0) {
                    reg[PC_REGISTRO] = current_instruction.d + reg[current_instruction.s];

                    continue;
                }
                break;
        }

        // Si no se ejecutó un salto, incrementamos el PC para pasar a la siguiente instrucción.
    }
}


void ejecutar_codigo_debug(Instruccion* instrucciones) {
    // Inicializamos el registro del Program Counter (registro 7) en 0.
    reg[PC_REGISTRO] = 0;
    int a;

    while (reg[PC_REGISTRO] < INSTRUCCIONES_MAXIMO && instrucciones[reg[PC_REGISTRO]].comando != HALT) {
        // Obtenemos la instrucción actual
        Instruccion current_instruction = instrucciones[reg[PC_REGISTRO]];

        // El switch ahora maneja la ejecución de la instrucción.
        switch (current_instruction.comando) {
            case IN:
                reg[PC_REGISTRO]++;
                imprimir_estado_maquina(current_instruction);
                std::cout << "? ";
                std::cin >> reg[current_instruction.r];
                if (std::cin.fail()) {
                    std::cerr << "Error: el valor ingresado no es un número entero.\n";
                    std::exit(EXIT_FAILURE);
                }
                std::cout << "Presiona Enter para continuar..." << std::endl;
                std::cin.get();
                break;
            case OUT:
                reg[PC_REGISTRO]++;
                imprimir_estado_maquina(current_instruction);
                std::cout << "> " << reg[current_instruction.r] << std::endl;
                std::cout << "Presiona Enter para continuar..." << std::endl;
                std::cin.get();
                break;
            case ADD:
                reg[PC_REGISTRO]++;
                imprimir_estado_maquina(current_instruction);
                reg[current_instruction.r] = reg[current_instruction.s] + reg[current_instruction.t];
                std::cout << "Presiona Enter para continuar..." << std::endl;
                std::cin.get();
                break;
            case SUB:
                reg[PC_REGISTRO]++;
                imprimir_estado_maquina(current_instruction);
                reg[current_instruction.r] = reg[current_instruction.s] - reg[current_instruction.t];
                std::cout << "Presiona Enter para continuar..." << std::endl;
                std::cin.get();
                break;
            case MUL:
                reg[PC_REGISTRO]++;
                imprimir_estado_maquina(current_instruction);
                reg[current_instruction.r] = reg[current_instruction.s] * reg[current_instruction.t];
                std::cout << "Presiona Enter para continuar..." << std::endl;
                std::cin.get();
                break;
            case DIV:
                reg[PC_REGISTRO]++;
                imprimir_estado_maquina(current_instruction);
                if (reg[current_instruction.t] == 0) {
                    std::cerr << "Error: División por cero en la instrucción en línea " << current_instruction.no_linea << std::endl;
                    std::exit(EXIT_FAILURE);
                }
                reg[current_instruction.r] = reg[current_instruction.s] / reg[current_instruction.t];
                std::cout << "Presiona Enter para continuar..." << std::endl;
                std::cin.get();
                break;
            case LD:
                reg[PC_REGISTRO]++;
                imprimir_estado_maquina(current_instruction);
                a = current_instruction.d + reg[current_instruction.s];
                if (a < 0 || a >= DATOS_MAXIMO) {
                    std::cerr << "Error: Acceso a memoria fuera de límites en la línea " << current_instruction.no_linea << std::endl;
                    std::exit(EXIT_FAILURE);
                }
                reg[current_instruction.r] = datos_Memoria[a];
                std::cout << "Presiona Enter para continuar..." << std::endl;
                std::cin.get();
                break;
            case LDA:
                reg[PC_REGISTRO]++;
                imprimir_estado_maquina(current_instruction);
                reg[current_instruction.r] = current_instruction.d + reg[current_instruction.s];
                std::cout << "Presiona Enter para continuar..." << std::endl;
                std::cin.get();
                break;
            case LDC:
                reg[PC_REGISTRO]++;
                imprimir_estado_maquina(current_instruction);
                reg[current_instruction.r] = current_instruction.d;
                std::cout << "Presiona Enter para continuar..." << std::endl;
                std::cin.get();
                break;
            case ST:
                reg[PC_REGISTRO]++;
                imprimir_estado_maquina(current_instruction);
                a = current_instruction.d + reg[current_instruction.s];
                if (a < 0 || a >= DATOS_MAXIMO) {
                    std::cerr << "Error: Acceso a memoria fuera de límites en la línea " << current_instruction.no_linea << std::endl;
                    std::exit(EXIT_FAILURE);
                }
                datos_Memoria[a] = reg[current_instruction.r];
                std::cout << "Presiona Enter para continuar..." << std::endl;
                std::cin.get();
                break;
            case JLT:
                reg[PC_REGISTRO]++;
                imprimir_estado_maquina(current_instruction);
                if (reg[current_instruction.r] < 0) {
                    //reg[PC_REGISTRO] = current_instruction.d;
                    reg[PC_REGISTRO] = current_instruction.d + reg[current_instruction.s];
                    continue; // Continuamos el bucle sin incrementar el PC
                }
                std::cout << "Presiona Enter para continuar..." << std::endl;
                std::cin.get();
                break;
            case JLE:
                reg[PC_REGISTRO]++;
                imprimir_estado_maquina(current_instruction);
                if (reg[current_instruction.r] <= 0) {
                    //reg[PC_REGISTRO] = current_instruction.d;
                    reg[PC_REGISTRO] = current_instruction.d + reg[current_instruction.s];
                    continue;
                }
                std::cout << "Presiona Enter para continuar..." << std::endl;
                std::cin.get();
                break;
            case JGE:
                reg[PC_REGISTRO]++;
                imprimir_estado_maquina(current_instruction);
                if (reg[current_instruction.r] >= 0) {
                    //reg[PC_REGISTRO] = current_instruction.d;
                    reg[PC_REGISTRO] = current_instruction.d + reg[current_instruction.s];
                    continue;
                }
                std::cout << "Presiona Enter para continuar..." << std::endl;
                std::cin.get();
                break;
            case JGT:
                reg[PC_REGISTRO]++;
                imprimir_estado_maquina(current_instruction);
                if (reg[current_instruction.r] > 0) { 


                    reg[PC_REGISTRO] = current_instruction.d + reg[current_instruction.s];

                    continue;
                }
                std::cout << "Presiona Enter para continuar..." << std::endl;
                std::cin.get();
                break;
            case JEQ:
                reg[PC_REGISTRO]++;
                imprimir_estado_maquina(current_instruction);
                if (reg[current_instruction.r] == 0) {
                    reg[PC_REGISTRO] = current_instruction.d + reg[current_instruction.s];
                    continue;
                }
                std::cout << "Presiona Enter para continuar..." << std::endl;
                std::cin.get();
                break;
            case JNE:
                reg[PC_REGISTRO]++;
                imprimir_estado_maquina(current_instruction);
                if (reg[current_instruction.r] != 0) {
                    reg[PC_REGISTRO] = current_instruction.d + reg[current_instruction.s];

                    continue;
                }
                std::cout << "Presiona Enter para continuar..." << std::endl;
                std::cin.get();
                break;
        }
        // Si no se ejecutó un salto, incrementamos el PC para pasar a la siguiente instrucción.
    }
}

void imprimir_estado_maquina(Instruccion instruccion_actual) {
    std::cout << "Instrucción actual: \n" << instruccion_actual.no_linea << ": ";
    switch (instruccion_actual.comando) {
        case HALT: std::cout << "HALT"; break;
        case IN: std::cout << "IN"; break;
        case OUT: std::cout << "OUT"; break;
        case ADD: std::cout << "ADD"; break;
        case SUB: std::cout << "SUB"; break;
        case MUL: std::cout << "MUL"; break;
        case DIV: std::cout << "DIV"; break;
        case LD: std::cout << "LD"; break;
        case LDA: std::cout << "LDA"; break;
        case LDC: std::cout << "LDC"; break;
        case ST: std::cout << "ST"; break;
        case JLT: std::cout << "JLT"; break;
        case JLE: std::cout << "JLE"; break;
        case JGE: std::cout << "JGE"; break;
        case JGT: std::cout << "JGT"; break;
        case JEQ: std::cout << "JEQ"; break;
        case JNE: std::cout << "JNE"; break;
    }
    std::cout << "\nr: " << instruccion_actual.r 
              << "\ns: " << instruccion_actual.s 
              << "\nt: " << instruccion_actual.t 
              << "\nd: " << instruccion_actual.d << std::endl;

    std::cout << "================= \nRegistros: \n";
    for (int i = 0; i < NUMERO_REGISTROS; ++i) {
        std::cout << "R[" << i << "] = " << reg[i] << std::endl;
    }
    std::cout << std::endl;


    std::cout << std::endl;

}

void imprimir_instrucciones(Instruccion* instrucciones) {
    unsigned int i = 0;
    do{
        std::cout << instrucciones[i].no_linea << ": "; 
        switch (instrucciones[i].comando) {
            case HALT: std::cout << "HALT "
            << instrucciones[i].r << ","  << instrucciones[i].s << "," << instrucciones[i].t ;  
            i++;
            break;
            case IN: std::cout << "IN " 
            << instrucciones[i].r << ","  << instrucciones[i].s << "," << instrucciones[i].t ; 
            i++;
            break;
            case OUT: std::cout << "OUT "
            << instrucciones[i].r << ","  << instrucciones[i].s << "," << instrucciones[i].t ;
            i++;
                break;
            case ADD: std::cout << "ADD "
            << instrucciones[i].r << ","  << instrucciones[i].s << "," << instrucciones[i].t ;
            i++;
                break;
            case SUB: std::cout << "SUB "
            << instrucciones[i].r << ","  << instrucciones[i].s << "," << instrucciones[i].t ;
            i++;
                break;
            case MUL: std::cout << "MUL "
            << instrucciones[i].r << ","  << instrucciones[i].s << "," << instrucciones[i].t ;
            i++;
                break;
            case DIV: std::cout << "DIV "
            << instrucciones[i].r << ","  << instrucciones[i].s << "," << instrucciones[i].t ;
            i++;
                break;
            case LD: std::cout << "LD "
            << instrucciones[i].r << ","  << instrucciones[i].d << "(" << instrucciones[i].s << ")";
            i++;
                break;
            case LDA: std::cout << "LDA " 
            << instrucciones[i].r << ","  << instrucciones[i].d << "(" << instrucciones[i].s << ")";
            i++;
                break;
            case LDC: std::cout << "LDC "
            << instrucciones[i].r << ","  << instrucciones[i].d << "(" << instrucciones[i].s << ")";
            i++;
                break;
            case ST: std::cout << "ST "
            << instrucciones[i].r << ","  << instrucciones[i].d << "(" << instrucciones[i].s << ")";
            i++;
                break;
            case JLT: std::cout << "JLT "
            << instrucciones[i].r << ","  << instrucciones[i].d << "(" << instrucciones[i].s << ")";
            i++;
                break;
            case JLE: std::cout << "JLE "
            << instrucciones[i].r << ","  << instrucciones[i].d << "(" << instrucciones[i].s << ")";
            i++;
                break;
            case JGE: std::cout << "JGE "
            << instrucciones[i].r << ","  << instrucciones[i].d << "(" << instrucciones[i].s << ")";
            i++;
                break;
            case JGT: std::cout << "JGT "
            << instrucciones[i].r << ","  << instrucciones[i].d << "(" << instrucciones[i].s << ")";
            i++;
                break;
            case JEQ: std::cout << "JEQ "
            << instrucciones[i].r << ","  << instrucciones[i].d << "(" << instrucciones[i].s << ")";
            i++;
                break;
            case JNE: std::cout << "JNE "
            << instrucciones[i].r << ","  << instrucciones[i].d << "(" << instrucciones[i].s << ")";
            i++;
                break;
        }
        std::cout << std::endl;
    }while(instrucciones[i - 1].comando != HALT && i < INSTRUCCIONES_MAXIMO);

}