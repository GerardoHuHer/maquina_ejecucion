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

    

    unsigned int i = 0;
    do{
        std::cout << "Instrucción en línea " << i << ": " << instrucciones[i].comando;
        std::cout  << " r: " << instrucciones[i].r 
                   << " s: " << instrucciones[i].s 
                   << " t: " << instrucciones[i].t 
                   << " d: " << instrucciones[i].d 
                   << std::endl;
        i++;
    }while (instrucciones[i - 1].comando != HALT);
    
   

    delete[] instrucciones;

    return 0;
}