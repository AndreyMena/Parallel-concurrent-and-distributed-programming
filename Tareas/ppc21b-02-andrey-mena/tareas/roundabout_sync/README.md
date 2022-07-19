# Tarea05 roundabout_sync
### _Descripcion del problema:_
Crear un programa que simule una rotonda que contiene 4 direcciones y 4 semaforos para cada una, donde estas deberan tener una capacidad de autos por direccion (segmento), esto se debera implementar mediante hilos de ejecucion, creando de esta manera un programa, que muestre el recorrido de cada auto que pasa por la rotonda, donde debera pasar los casos de prueba, buenas practicas de concurrencia, correcto uso de memoria etc.

A continuacion se mostrara un ejemplo de como se mostraria la entrada, tome en cuenta que cada linea excepto la primera representa un hilo de ejecucion, ademas el primer numero representado en la lectura significa la capacidad de cada segmento de la rotonda. 
###### Entrada:
    2

    EO
    SE
    NN
    ON
###### Salida:
    1 EO: E N O
    2 SE: S E
    3 NN: N O S E N
    4 ON: O S E N

De esta manera deben ser la impresiones, como es de esperar nuestro programa debe leer estos datos atraves de la entrada estandar, para poder hacer la lectura de los archivos, ademas debera de poder mostrar una salida mas detallada en caso de que el usuario lo requiera, esto lo podra acceder mediante argumento en el tercer espacio con un "-v", masomenos de esta manera deberia ser pasado el argumento:

    bin/roundabout_sync 2 5 -v < test/tests/input014.txt 

Y de esta manera debe verse una salida detallada:

    1: E
    3: N
    2: S
    1: N
    1: O
    ...
    3 N

    1 EO: E N O
    2 SE: S E
    3 NN: N O S E N
    4 ON: O S E N

### _Representacion de la rotonda:_
![Imagen Rotonda](tareas/roundabout_sync/design/roundabout_initial.svg)

### _Manual de usuario:_
Una vez el usuario haya descargado o clonado el repositoria, procedera a compilarlo mediante el siguiente comando dentro de la carpeta roundabout_sync:

    make clean; make;

Posteriormente procedera a correrlo, puede utilizar cualquier caso de prueba, ademas puede especificar la duracion minima y maxima de cada auto por segmento mediante el argumento numero 2 y 3, a continuacion un ejemplo de como correr el programa

    bin/roundabout_sync 2 5 -v < test/tests/input014.txt 

### _Creditos:_
Adjuntacion de imagenes en Readme:
https://www.it-swarm-es.com/es/git/anade-imagenes-readme.md-en-github/1071767059/

Edicion de archivo .Markdown:
https://dillinger.io/

Randomizador con rango minimo y maximo:
https://www.includehelp.com/c-programs/generate-random-numbers-within-a-range.aspx
