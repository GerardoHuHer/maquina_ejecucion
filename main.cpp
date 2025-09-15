#include "maquina_ejecucion.h"


int main(int argc, char** argv) {
    // validamos si el usuario paso el path del archivo desde terminal.
    if (argc < 2) {
        std::cerr << "Comando inválido usa " << argv[0] << " --help para más información"<< std::endl;
        return 1;
    }

    if(argc == 2 && std::string(argv[1]) == "--help") {
        std::cout << "Uso: " << argv[0] << " <archivo_de_instrucciones>" << std::endl;
        std::cout << "Modo debug: " << argv[0] << " --debug <archivo_de_instrucciones>" << std::endl;
        std::cout << "Ver código fuente en: " << argv[0] << " --code <archivo_de_instrucciones>" << std::endl;
        return 0;
    }

    Instruccion* instrucciones_memoria= new Instruccion[INSTRUCCIONES_MAXIMO];
    for(unsigned int i = 0; i < INSTRUCCIONES_MAXIMO; i++){
        instrucciones_memoria[i].comando = HALT;
        instrucciones_memoria[i].no_linea = i; 
        instrucciones_memoria[i].d = 0;
        instrucciones_memoria[i].s = 0;
        instrucciones_memoria[i].t = 0;
    }

   

    if(argc == 2 && std::string(argv[1]) != "--debug") {
        lexer(instrucciones_memoria, argv[1]);
        std::sort(instrucciones_memoria, instrucciones_memoria + INSTRUCCIONES_MAXIMO, [](const Instruccion& a, const Instruccion& b) {
            return a.no_linea < b.no_linea;
        });
        ejecutar_codigo(instrucciones_memoria);
        return 0;

    }


    if(argc == 3 && std::string(argv[1]) == "--code") {
        lexer(instrucciones_memoria, argv[2]);
        imprimir_instrucciones(instrucciones_memoria);
        delete[] instrucciones_memoria;
        return 0;
    }

    if(argc == 3 && std::string(argv[1]) == "--debug") {
        lexer(instrucciones_memoria, argv[2]);
    std::sort(instrucciones_memoria, instrucciones_memoria + INSTRUCCIONES_MAXIMO, [](const Instruccion& a, const Instruccion& b) {
        return a.no_linea < b.no_linea;
    });


        ejecutar_codigo_debug(instrucciones_memoria);
        return 0;
    } 
    
     

   std::cerr << "Argumentos inválidos. Usa --help para más información." << std::endl;
        delete[] instrucciones_memoria;
        return 1;

    delete[] instrucciones_memoria;

    return 0;
}
