# Diseño del Tarea01, goldbach_serial

### _Explicacion general del diseño:_
Para el diseño de esta tarea, se creó un diagrama UML para representar las "clases" y funciones a utilizar en el programa, un diagrama de la estructura de datos utilizada para representar el almacenamientos de los datos y un diseño en pseudocodigo para las funciones del programa.

### _Explicacion de diagrama UML:_
En este diagrama observamos 4 clases, inicialmente, el main, se encarga de redirigir el programa, y dentro del mismo se encuentran las funciones readStdin(), para la lectura de numeros, y printResult() para imprimir los resultados, dentro de la propia funcion del main, se crea un goldbachCalc y se llama a su funcion goldbachCalc run(), donde ya con los numeros leidos, se encarga de hacer el trabajo fuerte, ya sea llamando a goldbachCalc weakConject() o goldbachCalc strongConject(), que a su vez llama a goldbachCalc isPrime() para verificar numeros primos, los destructores se encargan de liberar la memoria una vez el programa va a terminar su ejecucion.

Tambien tenemos las clases array y arrayThreeField, y los campos de dataStructure, los cuales hablaremos mas detalladamente junto con el diagrama de la estructura de datos, por el momento, nuestros arreglos tienen funciones basicas de inicializacion, destruccion, aumento de espacio y agregado de elementos. Cabe aclarar que para la creación diagramas UML, se utiliza "+" para funciones o variables publicas, y "-" para privadas, dentro de los parentesis de cada funcion se encuentran los parametros que son pasados para su ejecucion, y despues de los dos puntos ":", el tipo de la variable o de retorno.
##### Diagrama UML:
![Diagrama de Tarea01](https://git.ucr.ac.cr/ANDREY.MENAESPINOZA/concurrente21c-andrey_mena_espinoza/-/raw/main/tareas/goldbach_serial/design/design-UML-goldbach_serial.PNG)

### _Explicacion de la estructura de datos utilizada:_
Para la estructura de datos de esta tarea utilizamos 3 "objetos", los tipos de cada uno se encuentra en el UML. El primero `inputsVector` es un arreglo donde se almacenan las entradas tal cual el usuario las introduce (a menos que no sea un entero de 64 bits), para el arreglo numOfSumsVector, se agrega la cantidad de sumas que tiene cada numero respecto a su indice de ingreso, este arreglo es almacenado conforme se calculan las conjeturas de goldbach, lo mismo con nuestro listArrThreeField, que este en cambio no es un arreglo comun, este objeto es una lista de arreglos, cada uno contiene 3 campos numericos por posicion a diferencia del arreglo comun que tiene solo uno, esto para poder almacenar los 3 o 2 factores de cada suma de goldbach por posicion. Como no sabemos cuantas sumas tendrá cada numero utilizamos arreglos dinamicos, lo mismo con inputsVector y numOfSumsVector.

##### Diagrama de la estructura de datos utilizada:
![Diagrama de factorizar](https://git.ucr.ac.cr/ANDREY.MENAESPINOZA/concurrente21c-andrey_mena_espinoza/-/raw/main/tareas/goldbach_serial/design/design-DataStructure-goldbach_serial.PNG)

##### Diagrama UML junto con representacion de estructura de datos:
![Diagrama de Base de datos](https://git.ucr.ac.cr/ANDREY.MENAESPINOZA/concurrente21c-andrey_mena_espinoza/-/raw/main/tareas/goldbach_serial/design/design-DataStructure-UML-goldbach_serial.PNG)

### _Creditos:_
Calculo de conjetura fuerte de goldbach (Adaptacion):
https://www.youtube.com/watch?v=ROEnh3ji-Oc

Adjuntacion de imagenes en Readme:
https://www.it-swarm-es.com/es/git/anade-imagenes-readme.md-en-github/1071767059/

Edicion de archivo .Markdown:
https://dillinger.io/

Trabajo diseñado y creado por:
Andrey Esteban Mena Espinoza, ECCI-UCR.
