#include "maquina_ejecucion.h"

int main(int argc, char** argv) {
    // validamos si el usuario paso el path del archivo desde terminal.
    if (argc < 2) {
        std::cerr << "Por favor, proporciona el archivo de instrucciones." << std::endl;
        return 1;
    }

    Instruccion* instrucciones= new Instruccion[INSTRUCCIONES_MAXIMO];
    for(unsigned int i = 0; i < INSTRUCCIONES_MAXIMO; i++){
        instrucciones[i].comando = HALT;
        instrucciones[i].no_linea = i;
        instrucciones[i].r = 0;
        instrucciones[i].d = 0;
        instrucciones[i].s = 0;
        instrucciones[i].t = 0;
    }

    lexer(instrucciones, argv[1]);

    for(unsigned int i = 0; i < 10; i++){
        // Imprimir solo las instrucciones válidas
        if (instrucciones[i].comando != NONE) {
            std::cout << "Instrucción en línea " << i << ": " << instrucciones[i].comando << std::endl;
        }
    }

    delete[] instrucciones;

    return 0;
}