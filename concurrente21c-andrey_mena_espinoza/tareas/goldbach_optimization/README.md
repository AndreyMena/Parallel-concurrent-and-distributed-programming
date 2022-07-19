# Tarea03: goldbach_optimization
### _Descripcion del problema:_
Optimizar el programa goldbach_pthread de la tarea 02 mediante el [metodo sugerido para optimizar](http://jeisson.ecci.ucr.ac.cr/concurrente/2021b/ejemplos/#optimization) en el que de forma `concurrente` igual que la tarea 02, haga el calculo de las sumas de goldbach, para esto debera hacer 2 iteraciones del metodo sugerido para optimizar, donde en la primer optimizacion debera implementar un mapeo dinamico, y en la segunda podra hacer una optimizacion e escogencia. Cada paso de las optimizaciones debera ser documentado en una carpeta `report/` con un readme, en el que se hara conocer las mejoras, duraciones, lecciones aprendidad etc, que llevo a cabo para resolver el problema.


Respecto al calculo de sumas de goldbach:

Cada número puede tener muchas sumas de goldbach, estas deberán ser informadas mediante la salida estandar, además en caso de introducir un numero negativo se interpretara que el usuario quiere que ademas de expresar la cantidad de sumas del numero, las muestre en pantalla. El programa solo debera leer numeros de 64 bits, en caso de que un numero sobrepase este rango o se encuentre entre `0 <= numero <= 5` (ya que para este ultimo rango no aplica para las sumas de goldbach), se deberá imprimir `NA`. En caso de que la lectura no sea un numero se terminara la ejecucion y no se procesara mas numeros. A continuacion un ejemplo de una entrada y su salida esperada.

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
Una vez descargamos el repositorio, debemos posicionarnos en la carpeta `goldbach_optimization/`, luego elegiremos la optimizacion que queremos probar, si queremos la 1, abriremos una terminal en `goldbach_optimization/optim01` y escribiremos el siguiente comando: `make clean; make`, esto limpiara y compilara el programa, ademas creara un ejecutable en la carpeta `bin/`, ahora podemos ejecutar el programa con `bin/optim01`, y es aqui donde podremos introducir los numeros, cuando no queramos introducir mas podemos precionar `CTRL + D` (END OF FILE), esto terminara la lectura y empezara a procesarlos. 

La otra manera de correr el programa es mediante los casos de prueba, los cuales se encuentra en la carpeta `test/`, un ejemplo de probar el programa con un caso de prueba es con el siguiente comando:`bin/optim01 < test/test-small/input002.txt`, de esta manera redirigirá el `.txt` a la entrada estandar de nuestro programa, leera sus numeros e imprimira su resultado. Con `bin/optim01 < test/test-small/input002.txt | diff - test/test-small/output002.txt`, analizaremos nuestras salidas respecto a los outputs.txt.

### _Reporte:_
En la carpeta `goldbach_optimization/`, se encuentra una carpeta `report/` donde se encuentra una hoja de calculo, y un readme donde se encuentra la documentacion de los pasos llevados a cabo para las 2 optimizaciones. Reporte [aqui](https://git.ucr.ac.cr/ANDREY.MENAESPINOZA/concurrente21c-andrey_mena_espinoza/-/tree/main/tareas/goldbach_optimization/report)

### _Creditos:_
Calculo de conjetura fuerte de goldbach (Adaptacion):
https://www.youtube.com/watch?v=ROEnh3ji-Oc

Adjuntacion de imagenes en Readme:
https://www.it-swarm-es.com/es/git/anade-imagenes-readme.md-en-github/1071767059/

Edicion de archivo .Markdown:
https://dillinger.io/

Trabajo diseñado y creado por:

Andrey Esteban Mena Espinoza, ECCI-UCR.
