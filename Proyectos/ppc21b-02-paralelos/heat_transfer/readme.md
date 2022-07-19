# heat_transfer
## Descripción del problema

Como parte de las aplicaciones que se tienen de la concurrencia en la ciencia, se lleva a cabo este proyecto para ayudar a los experimentadores a ejecutar procesos de simulación de transferencia de energía en placas de metal, considerando un modelo físico en el que se consideran variables como tiempo, dimensiones, materiales, difusividad y el cambio mínimo que se considera significativo y el cual determinará la detención del software. 

Este tipo de procesos llevados a cabo por los experimentadores requieren de alta eficiencia y buen uso de la concurrencia de tareas, ya que los datos son representados por láminas de gran tamaño y debido al modelo físico, no es un trabajo sencillo de procesar. Es por eso que en este proyecto se brinda una solución al problema, facilitando una herramienta para la simulación de la dispersión del calor en láminas.

Se desarrolla un programa para la simulación de transferencia de calor en láminas de material variable en sus versiones serial y para lograr la concurrencia, se aplicaron directivas de OpenMp para paralelizar las regiones de código consideradas críticas, logrando un mejor rendimiento del programa. El programa leerá las instrucciones de simulación de un archivo job###.txt, donde los investigadores establecen cuales son las listas de láminas y otros parámetros para llevar a cabo la simulación, además, se cargarán archivos .bin que contienen el estado inicial de las láminas, así como las temperaturas a las que son sometidas y su distribución.

## Diseño del programa
Puede consultar los archivos asociados al diseño en [este enlace](https://git.ucr.ac.cr/LUIS.BARRANTESROJAS/ppc21b-02-paralelos). 

## Manual de uso
### Instalar requerimientos
Ejecute:
```
sudo apt update
sudo apt upgrade
sudo apt install doxygen git build-essential python3-pip valgrind clang
```

Para instalar el analizador de código utilice:
`sudo pip3 install cpplint`

### Compilación
Para compilar el código, ubíquese en la carpeta raíz ` heat_transfer ` en su terminal y digite:
```
make
```
Con esta instrucción se compilará el código.

### Ejecución
Una vez compilado el código, ejecute el programa con la siguiente instrucción:
```
bin/heat_transfer job001.txt 16
```
Puede modificar la cantidad de hilos con el que se desea correr el programa, si se omite este número se utiliza la cantidad máxima de hilos del sistema.


## Integrantes
Andrés Fallas Mena

Andrey Mena Espinoza

Christopher Obando Salgado

### Créditos
https://people.sc.fsu.edu/~jburkardt/cpp_src/heated_plate/heated_plate.html
