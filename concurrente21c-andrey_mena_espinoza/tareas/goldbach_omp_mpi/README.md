# Tarea04: goldbach_omp_mpi
### _Descripcion del problema:_
Paraleliza y distribuir el programa goldbach de la tarea 03 con las optimizacion implementadas. El primer paso sera paraleliza el programa mediante la concurrencia declarativa (OpenMP) y hacer una comparación de Pthreads respecto a OpenMP. El segundo paso sera implementarle a la version de OpenMP Distribución, mediante MPICH O MPI, luego, de igual manera debera hacer la comparación de OpenMP respecto a la deMPI. Todo esto debera seguir el lineamiento de las tareas, anteriores, pasar casos de prueba, linter etc.

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
Una vez descargamos el repositorio, debemos posicionarnos en la carpeta `goldbach_omp_mpi/`, luego elegiremos la version que queremos probar ya sea con OpenMP sola o con OpenMP+MPI, si queremos la segunda, abriremos una terminal en `goldbach_omp_mpi/goldbach_omp_mpi` y escribiremos el siguiente comando: `make clean; make`, esto limpiara y compilara el programa, ademas creara un ejecutable en la carpeta `bin/`. Para ejecutar el programa haciendo uso de mpi y especificando la cantidad de procesos, podemos excribir un comando como el siguiente:

`mpiexec -np 3 bin/goldbach_omp_mpi < test/test-small/input003.txt`

Esto correro el programa con la prueba `input003.txt`, con la cantidad de procesos `3`, numero que es especificado luego del `-np`. Para verificar que el programa este produciendo la salida correcta podemos utilizar el siguiente comando:

`mpiexec -np 3 bin/goldbach_omp_mpi < test/test-small/input003.txt | diff - test/test-small/output003.txt `

Tambien podemos ejecutar el programa con `bin/goldbach_omp_mpi`, e introducir los numeros nosotros mismos, cuando no queramos introducir mas podemos precionamos `CTRL + D` (END OF FILE), esto terminara la lectura y empezara a procesarlos. 

### _Reporte:_
En la carpeta `goldbach_omp_mpi/`, se encuentra una carpeta `report/` donde se encuentra una hoja de calculo, y un readme donde se encuentra la documentacion de los pasos llevados a cabo para las 2 optimizaciones de la tarea anterior, mas los resultados y comparaciones de esta tarea. Reporte [aqui](https://git.ucr.ac.cr/ANDREY.MENAESPINOZA/concurrente21c-andrey_mena_espinoza/-/tree/main/tareas/goldbach_omp_mpi/report)

### _Creditos:_
Calculo de conjetura fuerte de goldbach (Adaptacion):
https://www.youtube.com/watch?v=ROEnh3ji-Oc

Adjuntacion de imagenes en Readme:
https://www.it-swarm-es.com/es/git/anade-imagenes-readme.md-en-github/1071767059/

Edicion de archivo .Markdown:
https://dillinger.io/

Trabajo diseñado y creado por:

Andrey Esteban Mena Espinoza, ECCI-UCR.
