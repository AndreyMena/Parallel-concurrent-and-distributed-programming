# Tarea02: goldbach_pthread
### _Descripcion del problema:_
Crear un programa en `c` de manera `concurrente`, mediante la implementacion de la tecnologia `pthread` que con el uso de hilos de ejecucion  al recibir una lista de numeros enteros por entrada estandar, haga el calculo de todas las sumas de goldbach de cada numero respectivamente. Para el calculo de las sumas, se basara en las conjeturas de goldbach, la conjetura fuerte, para numeros pares y la conjetura debil para numeros impares. En el caso de la conjetura fuerte, cada numero par, en la suma de goldbach, es representado mediante la suma de dos factores primos. Y para la suma de goldbach mediante conjetura debil, cada numero impar es representado mediante la  suma de 3 factores primos.

Cabe recalcar que cada número puede tener muchas sumas de goldbach, estas deberán ser informadas mediante la salida estandar, además en caso de introducir un numero negativo se interpretarú que el usuario quiere que ademas de expresar la cantidad de sumas del numero, las muestre en pantalla. El programa solo debera leer numeros de 64 bits, en caso de que un numero sobrepase este rango o se encuentre entre `0 <= numero <= 5` (ya que para este ultimo rango no aplica para las sumas de goldbach), se deberá imprimir `NA`. En caso de que la lectura no sea un numero se terminara la ejecucion y no se procesara mas numeros. A continuacion un ejemplo de una entrada y su salida esperada.

###### Entrada:
`1`  
`2`  
`6`  
`7`  
`-8`  
`-9`  
`10`  
`11`  
`12`  
`13`  
`-14`  
`-21`  
###### Salida:
`1: NA`  
`2: NA`  
`6: 1 sums`  
`7: 1 sums`  
`-8: 1 sums: 3 + 5`  
`-9: 2 sums: 2 + 2 + 5, 3 + 3 + 3`  
`10: 2 sums`  
`11: 2 sums`  
`12: 1 sums`  
`13: 2 sums`  
`-14: 2 sums: 3 + 11, 7 + 7`  
`-21: 5 sums: 2 + 2 + 17, 3 + 5 + 13, 3 + 7 + 11, 5 + 5 + 11, 7 + 7 + 7`  


### _Manual de usuario:_
Una vez descargamos el repositorio, debemos posicionarnos en la carpeta `goldbach_pthread`, aquí debemos abrir una terminal y escribir el siguiente comando: `make clean; make`, esto limpiara y compilara el programa, ademas creara un ejecutable en la carpeta `bin/`, ahora podemos ejecutar el programa con `bin/goldbach_pthread`, y es aqui donde podremos introducir los numeros, cuando no queramos introducir mas podemos precionar `CTRL + D` (END OF FILE), esto terminara la lectura y empezara a procesarlos. 

La otra manera de correr el programa es mediante los casos de prueba, los cuales se encuentra en la carpeta `test/`, un ejemplo de probar el programa con un caso de prueba es con el siguiente comando:`bin/goldbach_pthread < test/test-small/input002.txt`, de esta manera redirigirá el `.txt` a la entrada estandar de nuestro programa, leera sus numeros e imprimira su resultado. Con `bin/goldbach_pthread < test/test-small/input002.txt | diff - test/test-small/output002.txt`, analizaremos nuestras salidas respecto a los outputs.txt.

### _Diagramas y representaciones de nuestra tarea:_
##### Diagrama de memoria para la distribucion de trabajo:
![UML](https://git.ucr.ac.cr/ANDREY.MENAESPINOZA/concurrente21c-andrey_mena_espinoza/-/raw/main/tareas/goldbach_pthread/design/design-Diagrama-goldbach_pthread.PNG)

##### Diagrama de las estructura de datos utilizada:
![Diagrama de las estructuras de datos](https://git.ucr.ac.cr/ANDREY.MENAESPINOZA/concurrente21c-andrey_mena_espinoza/-/raw/main/tareas/goldbach_pthread/design/design-DiagramaDeEstructuras-goldbach_pthread.PNG)

### _Creditos:_
Calculo de conjetura fuerte de goldbach (Adaptacion):
https://www.youtube.com/watch?v=ROEnh3ji-Oc

Adjuntacion de imagenes en Readme:
https://www.it-swarm-es.com/es/git/anade-imagenes-readme.md-en-github/1071767059/

Edicion de archivo .Markdown:
https://dillinger.io/

Trabajo diseñado y creado por:
Andrey Esteban Mena Espinoza, ECCI-UCR.
