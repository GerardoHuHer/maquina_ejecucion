# Máquina de ejecución para microlenguaje 

## Requerimientos

- g++ 13.3.0 o mayor

## Compilación
- Compilar los tres archivos fuente.

```bash
g++ main.cpp maquina_ejecucion.cpp maquina_ejecucion.h 
```

o

```bash
g++ main.cpp maquina_ejecucion.cpp maquina_ejecucion.h -o <nombre_ejecutable>.out

```

# Ejecución

```bash
./<nombre_ejecutable>.out <archivo_instrucciones>.cod
```

## Formas

### Ejecución completa
 
```bash
./<nombre_ejecutable>.out <archivo_instrucciones>.cod
```
### Ejecución modo debug
```bash
./<nombre_ejecutable>.out --debug <archivo_instrucciones>.cod
```
### Ejecución para ver código que se va a ejecutar
```bash
./<nombre_ejecutable>.out --code <archivo_instrucciones>.cod
```
### Ayuda
```bash
./<nombre_ejecutable>.out --help
```
