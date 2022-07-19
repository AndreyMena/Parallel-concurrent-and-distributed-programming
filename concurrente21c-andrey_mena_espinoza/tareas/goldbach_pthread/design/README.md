# Diseño del Tarea02, goldbach_pthread

### _Explicacion general del diseño:_
Para el diseño de esta tarea, se creó un diagrama de memoria para resaltar que entradas le corresponden a cada hilo, ademas se encuentra un pseudocodigo, donde se ve mas detalladamente como se hace el calculo para la division de trabajo, asi como la creacion de hilos y su concurrencia.

### _Explicacion de diagrama de Memoria y pseudocodigo:_
Para este diagrama y pseudocodigo, vemos que lo distinto respecto a la Tarea01, es el agregado de hilos, por lo que la estructura general del programa se mantiene. Para esta creacion de hilos lo importante es entender la division de trabajo, esta se lleva a cabo mediante un mapeo por bloque. Se distribuye la cantidad de entradas entre la cantidad de hilos mediante una division entera. Donde si tenemos 10 entradas y 3 hilos, hacemos el calculo de 10/3 = 3 (division entera), con esto nuestra division de trabajo es de 3 unidades, ademas calculamos el modulo (para calcular el residuo) con 10 % 3 = 1, teniendo listos nuestra division de trabajo y el residuo, adaptamos la formula dada en clases impartidas por el profesor jeisson:

Con jobDivision := 3, remaining := 1, 3 hilos de ejecucion y 10 entradas numericas:

* El rango minimo del primer hilo es:

    (jobDivision * NumeroDeHilo) + min (numeroDeHilo, remaining) = *0*

    (jobDivision * NumeroDeHilo+1) + min (numeroDeHilo+1, remaining) = *4*
    ##### [0, 4[
* El rango minimo del segundo hilo es:

    (jobDivision * NumeroDeHilo) + min (numeroDeHilo, remaining) = *4*

    (jobDivision * NumeroDeHilo+1) + min (numeroDeHilo+1, remaining) = *7*
    ##### [4, 7[
* El rango minimo del segundo hilo es:

    (jobDivision * NumeroDeHilo) + min (numeroDeHilo, remaining) = *7*

    (jobDivision * NumeroDeHilo+1) + min (numeroDeHilo+1, remaining) = *10*
    ##### [7, 10[

Por lo que el primer hilo se encarga de la entrada 0, 1, 2, 3, el segundo hilo de 4, 5, 6, y el tecero de 7, 8, 9. Ademas de la division de trabajo y creacion de hilos, tenemos ahora una privateData con el valor "min" y "max" correspondientes a los indices minimos y maximos que recorren, por otro lado, nuestra antigua dataStructure de la Tarea01 se convirtio en nuestra sharedData, donde se guardaran los datos producido por nuestra calculadora de sumas de goldbach

##### Diagrama de Memoria:
![Diagrama de memoria Tarea02](https://git.ucr.ac.cr/ANDREY.MENAESPINOZA/concurrente21c-andrey_mena_espinoza/-/raw/main/tareas/goldbach_pthread/design/design-Diagrama-goldbach_pthread.PNG)

### _Explicacion de la estructura de datos utilizada:_
Para nuestra sharedData utilizamos 3 "objetos". El primero `inputsVector` es un arreglo donde se almacenan las entradas tal cual el usuario las introduce (a menos que no sea un entero de 64 bits), para el arreglo `numOfSumsVector`, se agrega la cantidad de sumas que tiene cada numero respecto a su indice de ingreso, este arreglo es almacenado conforme se calculan las conjeturas de goldbach, lo mismo con nuestro `listArrThreeField`, que este en cambio no es un arreglo comun, este objeto es una lista de arreglos, cada uno contiene 3 campos numericos por posicion a diferencia del arreglo comun que tiene solo uno, esto para poder almacenar los 3 o 2 factores de cada suma de goldbach por posicion. Como no sabemos cuantas sumas tendrá cada numero utilizamos arreglos dinamicos, lo mismo con inputsVector y numOfSumsVector. Para nuestra `privateData` que es nuestra estructura nueva, utilizamos los enteros min(indice minimo a procesar), max(indice maximo a procesar), threadNumber(numero de hilo) y un puntero a nuestra sharedData.

##### Diagrama de las estructuras:
![Diagrama de estructuras](https://git.ucr.ac.cr/ANDREY.MENAESPINOZA/concurrente21c-andrey_mena_espinoza/-/raw/main/tareas/goldbach_pthread/design/design-DiagramaDeEstructuras-goldbach_pthread.PNG)

### _Creditos:_
Calculo de conjetura fuerte de goldbach (Adaptacion):
https://www.youtube.com/watch?v=ROEnh3ji-Oc

Adjuntacion de imagenes en Readme:
https://www.it-swarm-es.com/es/git/anade-imagenes-readme.md-en-github/1071767059/

Edicion de archivo .Markdown:
https://dillinger.io/

Trabajo diseñado y creado por:
Andrey Esteban Mena Espinoza, ECCI-UCR.
