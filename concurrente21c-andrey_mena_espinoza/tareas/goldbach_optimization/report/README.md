# Documento de reporte

## _Descripcion:_
En el presente reporte se informara el desarrollo de las optimizaciones llevadas a cabo sobre el programa `goldbach_pthread` de nuestra `tarea02/`, para las mismas se utilizara `El metodo sugerido para optimizar` el cual es detallado por el profesor Jeisson Hidalgo Cespedes, donde la forma de optimizar sigue el siguiente ciclo de pasos:

* 1-Medir el rendimiento del código antes de realizar las modificaciones.

* 2-Analizar el código para detectar las regiones críticas a optimizar (profiling).

* 3-Hacer las modificaciones que se cree incrementarán el rendimiento en las regiones críticas.

* 4-Asegurarse de que las modificaciones sean correctas (correr el programa de pruebas).

* 5-Medir el rendimiento del código después de realizar las modificaciones y comparar para determinar si hubo una ganancia.

* 6-Indiferentemente de si se incrementó o no el rendimiento, documentar las lecciones aprendidas, ya que servirán para otros desarrolladores que intenten optimizar la misma sección de código.

* 7-Si es realmente requerido incrementar aún más el rendimiento, repetir desde el paso 2.

![Diagrama del metodo sugerido para optimizar](http://jeisson.ecci.ucr.ac.cr/concurrente/2020c/ejemplos/pthreads/producer_consumer_unbounded_buffer_modularized/problem_solving_process.svg)

Para el desarrollo de las mediciones, optimizaciones y pruebas se utilizara una maquina virtual con 6 nucleos, esto debido a que se concluyo que era considerablemente mas rapido los presentes 6 nucleos de la maquina virtual actual que los 8 nucleos de arenal. Ademas cabe aclarar que se hará uso del caso de prueba `test/test-medium/input020.txt` para las mediciones de tiempo.

![Duracion de tiempo 8 hilos arenal vs 6 hilos maquina virtual](https://git.ucr.ac.cr/ANDREY.MENAESPINOZA/concurrente21c-andrey_mena_espinoza/-/raw/main/tareas/goldbach_optimization/report/Duracion%208%20hilos%20arenal%20vs%206%20hilos%20maquina%20virtual.PNG)

Nota: todas las mediciones a continuacion seran realizadas con 6 hilos con excepcion de la primera.

## _Iteracion/Optimizacion #1, Mapeo dinámico:_
#### Paso #1. Medicion de rendimiento:
`Execution time: 897.952951620` (segundos) (1 hilo goldbach serial)
![Execution time: 897.952951620](https://git.ucr.ac.cr/ANDREY.MENAESPINOZA/concurrente21c-andrey_mena_espinoza/-/raw/main/tareas/goldbach_optimization/report/DuracionGoldbachSerial.PNG)

`Execution time: 266.224848587` (segundos). (6 hilos)
![Execution time: 266.224848587 (segundos)](https://git.ucr.ac.cr/ANDREY.MENAESPINOZA/concurrente21c-andrey_mena_espinoza/-/raw/main/tareas/goldbach_optimization/report/MedicionGoldbachPthread.PNG)

#### Paso #2. Profiling:
No admite. Por enunciado del problema se utilizara un mapeo dinamico.

#### Paso #3. Implementacion:
Implementacion de mapeo dinamico en el codigo, se utiliza el patron de `conditionally safe`, mediante el incremento de un contador que al ser aumentado de forma protegida asigna la posicion al respectivo hilo. (mas detalles sobre diseño [aqui](https://git.ucr.ac.cr/ANDREY.MENAESPINOZA/concurrente21c-andrey_mena_espinoza/-/tree/main/tareas/goldbach_optimization/optim01/design))

#### Paso #4. Correctitud:
Se confirma correctitud del programa mediante casos de prueba.

#### Paso #5. Medicion de rendimiento y comparacion:
`Execution time: 213.182639254` (segundos).
![Execution time: 213.182639254](https://git.ucr.ac.cr/ANDREY.MENAESPINOZA/concurrente21c-andrey_mena_espinoza/-/raw/main/tareas/goldbach_optimization/report/DuracionPostOptim01.PNG)

Comparando el tiempo de ejecucion pre-optimizacion versus el tiempo de ejecucion post-optimizacion, podemos confirmar que hay un incremento de velocidad, mas especificamente tenemos 53.04 segundos de diferencia respecto a ambos.

#### Paso #6. Lecciones aprendidas:
Se ha aprendido que para problemas en los que no se sabe muy bien, cuanto va a ser la duracion de cada unidad de trabajo, puede ser una buena idea utilizar un mapeo dinamico, mas especificamente cuando pueden haber trabajos que tengan una duracion excesiva mientras otros una duracion minima, al haber mucha descompensacion entre cada unidad de trabajo, esta puede ser una buena opcion, ya que por ejemplo en este problema en especifico antes de optimizar, teniamos hilos que terminaban su trabajo muy rapidamente mientras otros seguian trabajando, luego de este mapeo cuando un hilo termina su trabajo se le asigna otro.

Cabe aclarar que a diferencia del mapeo por bloque realizado anteriormente, este mapeo dinamico si tiene control de concurrencia, para esto se intento disminuir al maximo la region critica e intentar aprovechar al maximo cada hilo asignandole mas trabajo.

#### Paso #7. ¿Es realmente necesario incrementar mas el desempeño?:
En este caso y para esta optimizacion en especifico no, ya que la implementacion es hacer un mapeo dinamico y ya esta realizada.


## _Iteracion/Optimizacion #2:_
#### Paso #1. Medicion de rendimiento:
`Execution time: 213.182639254` (segundos). Correspondiente a optimizacion #1.
![Execution time: 213.182639254](https://git.ucr.ac.cr/ANDREY.MENAESPINOZA/concurrente21c-andrey_mena_espinoza/-/raw/main/tareas/goldbach_optimization/report/DuracionPostOptim01.PNG)

#### Paso #2. Profiling:
Mediante el uso de la herramienta kcachegrind se hizo busqueda de las lineas de codigo mas problematicas del programa, como vemos a continuacion hay una alta demanda del metodo isPrime() para calcular las sumas de goldbach, derivado de las multiples llamadas provenientes de las conjeturas, especialmente de la conjetura debil, ya que esta tiene un ciclo extra haciendo llamados a nuestro metodo isPrime.
![Kcachegrind Conjetures](https://git.ucr.ac.cr/ANDREY.MENAESPINOZA/concurrente21c-andrey_mena_espinoza/-/raw/main/tareas/goldbach_optimization/report/KcachegrindConjetures.PNG)
![Kcachegrind isPrime](https://git.ucr.ac.cr/ANDREY.MENAESPINOZA/concurrente21c-andrey_mena_espinoza/-/raw/main/tareas/goldbach_optimization/report/KcachegrindIsprime.PNG)

#### Paso #3. Implementacion:
Como vemos en el [diseño](https://git.ucr.ac.cr/ANDREY.MENAESPINOZA/concurrente21c-andrey_mena_espinoza/-/tree/main/tareas/goldbach_optimization/optim01/design), se optimizaran las conjeturas, intercambiando condicionales para intentar hacer la menor cantidad de llamados a isPrime(), de la igual manera se optimizara tambien el metodo isPrime() de forma que tenga que llamar a sqrt() una unica vez al principio de la funcion y no multiples veces en el ciclo, ademas los incrementos seran de dos en dos y no de uno en uno.

#### Paso #4. Correctitud:
Se confirma correctitud del programa mediante casos de prueba.

#### Paso #5. Medicion de rendimiento y comparacion:
Optimizacion en calculo de conjeturas:

`Execution time: 24.173962643`
![Execution time: 24.173962643](https://git.ucr.ac.cr/ANDREY.MENAESPINOZA/concurrente21c-andrey_mena_espinoza/-/raw/main/tareas/goldbach_optimization/report/DuracionPostOptim02.PNG)

Optimizacion en calculo de conjeturas + optimizacion en isPrime():

`Execution time: 11.608791367`
![Execution time: 11.608791367](https://git.ucr.ac.cr/ANDREY.MENAESPINOZA/concurrente21c-andrey_mena_espinoza/-/raw/main/tareas/goldbach_optimization/report/DuracionPostOptim02b.PNG)

Como vemos hubo un aumento en el rendimiento bastante considerable, disminuimos un total de 202 segundos respecto a la optimizacion 01, tuvimos un speedup de 19.36 respecto a la optimizacion 01 y un speedup de 24.20 respecto a la version pthread con 6 hilos sin optimizar.

#### Paso #6. Lecciones aprendidas:
Se aprendio que sacar la funcion de sqrt() fuera del ciclo no hace un cambio realmente significativo, en cambio hacer incrementos de dos en dos dentro del ciclo isPrime() disminuye casi a la mitad la duracion del programa, pero lo que verdaderamente hace una disminucion de tiempo enorme dentro del programa, es hacer intercambios de condicionales de forma que hagan los mismo, solo que en un orden de menor duracion a mayor duracion de forma que al llegar a la operacion isPrime() ya hayan sido descartados muchos numeros, lo que nos permite eliminar ciclos. 

Para darse una idea, anteriormente habian casos en que se hacian los extensos ciclos para verificar si ciertos numeros son primos, para luego descartarlos puesto que ya habian sido almacenado pero en distinto orden.

Y es por esto que la mayor leccion aprendida es que siempre en la medida de lo posible, es mejor hacer los condicionales menos fuertes (en terminos de tiempo), antes que los condicionales u operaciones mas duraderas. De esta manera descartamos antes de tiempo ciertos numeros que podrian haber durado mucho posteriormente.

#### Paso #7. ¿Es realmente necesario incrementar mas el desempeño?:
Considerando que actualmente el caso de prueba mas duradero es de 17 segundos e inicialmente, estos duraban mas de 12 horas (nunca se logro terminar de correr uno), al punto de que no era posible hacer las mediciones e incluso se tuvo que optimizar un poco antes de tiempo, realmente se considera que es un tiemop realmente bueno, por lo que se considera innecesario optimizar aun mas.

## _Analisis final sobre graficacion y recoleccion de datos:_
#### Optimizacion:
![Optimizacion](https://git.ucr.ac.cr/ANDREY.MENAESPINOZA/concurrente21c-andrey_mena_espinoza/-/raw/main/tareas/goldbach_optimization/report/OptimizationGraph.PNG)

Vemos un grafico comparativo del `speedup` y la `duracion` en segundos sobre las distintas versiones que hemos utilizado, vemos primeramente que ha habido una clara disminucion del tiempo de ejecucion con forme hemos hido implementando una version nueva hasta llegar a las optimizaciones, con excepcion de la version goldbach_pthread con 1 hilo y la version goldbach_serial, que duran practicamente lo mismo, el resto han hido en un incremento del `speedup` y por consiguiente un decremento en la duracion del programa.

#### Concurrency level:
![Concurrency level](https://git.ucr.ac.cr/ANDREY.MENAESPINOZA/concurrente21c-andrey_mena_espinoza/-/raw/main/tareas/goldbach_optimization/report/ConcurrencyLevelGraph.PNG)

Respecto al nivel de concurrencia, haciendo esta comparacion sobre el `speedup` y la `eficiencia` vemos que conforme vamos aumentando la cantidad de hilos tambien va aumentando el speedup, pero llega a un punto donde el incremento ya es que casi imperceptible por no decir nulo, esto sucede cuando utilizamos mas hilos de los que posee nuestra maquina. Por otro lado respecto a la eficiencia, vemos que conforme utilizamos mas hilos esta tambien va disminuyendo.

##### Sobre: ¿Cual es la cantidad de hilos óptima para conseguir el mejor rendimiento? Considerando la eficiencia y la velocidad

Podemos observar que este punto se encuentra entre la mitad de hilos de la maquina y el total de hilos de la maquina, para este caso, entre 3 y 6 hilos. Esto debido a que en este rango donde se da el punto de intercepcion que ocasiona el balance entre una buena velocidad y buena eficiencia.
