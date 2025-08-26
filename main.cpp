#include "maquina_ejecucion.h"

int main(int argc, char** argv) {
    // validamos si el usuario paso el path del archivo desde terminal.
    if (argc < 2) {
        std::cerr << "Por favor, proporciona el archivo de instrucciones." << std::endl;
        return 1;
    }

    Instruccion* instrucciones_memoria= new Instruccion[INSTRUCCIONES_MAXIMO];
    for(unsigned int i = 0; i < INSTRUCCIONES_MAXIMO; i++){
        instrucciones_memoria[i].comando = HALT;
        instrucciones_memoria[i].no_linea = i;
        instrucciones_memoria[i].r = 0;
        instrucciones_memoria[i].d = 0;
        instrucciones_memoria[i].s = 0;
        instrucciones_memoria[i].t = 0;
    }

    lexer(instrucciones_memoria, argv[1]);
    std::sort(instrucciones_memoria, instrucciones_memoria + INSTRUCCIONES_MAXIMO, [](const Instruccion& a, const Instruccion& b) {
        return a.no_linea < b.no_linea;
    });


    unsigned int i = 0;
    do{
        std::cout << "Instrucción en línea " << i << ": " << instrucciones_memoria[i].comando;
        std::cout  << " r: " << instrucciones_memoria[i].r
                   << " s: " << instrucciones_memoria[i].s
                   << " t: " << instrucciones_memoria[i].t
                   << " d: " << instrucciones_memoria[i].d
                   << std::endl;
        i++;
    }while (instrucciones_memoria[i - 1].comando != HALT);

    delete[] instrucciones_memoria;

    return 0;
}
