# Proy2: Simulación de calor distribuida
### _Integrantes:_
Christopher Obando Salgado.  
Jose Alexis Valerio Ramirez.  
Andrey Esteban Mena Espinoza  
Adrian Hutt Orozco.  

### _Descripcion del problema:_
Crear una `simulación de calor concurrente ` utilizando paralelismo de datos con concurrencia y distribución declarativa. El proyecto se divide en dos partes en la primera se deberá aplicar paralelismo mediante la utilizacion de OpenMP y en la segunda programacion distribuida mediante MPI.

Se deberá simular en forma de transferencia de calor, un programa que permita encontrar el momento de equilibrio termico de una lamina rectangular (puede ser de distintos tamaños), de forma que en los bordes se inyectara calor, mismo que conforme pasa el tiempo calentara sus alrededores. Las laminas son representadas por medio de una matriz, la cual contiene celdas en la que contiene un valor de calor predefinido en las laminas de lectura. El programa deberá leer trabajos, que dentro contiene laminas con sus respectivos valores los cuales son:

`1-Nombre del archivo que contiene la lámina en su estado inicial.`

`2-La duración de cada etapa Δt en segundos.`

`3-La difusividad térmica`

`4-Las dimensiones h de las celdas.`

`5-La sensitividad del punto de equilibrio ε`

Se deberá producir las laminas resultantes producto de la transferencia de calor, ademas de las salidas resultantes de todo el trabajo con su tiempo de duración simulado. Por otra parte se deberá hacer mediciones de tiempo, eficiencia y speedup y generar un archivo `report/` con graficos y los datos recolectados.

### _Construcción de la solución:_
Una vez descargado el repositorio, para correr la simulacion deberemos posicionarnos en la carpeta `heat_transfer/`, donde abriremos una terminal y  procedermos a compilar el programa mediante el comando `make clean; make`, enseguida se creara una carpeta `bin/` que contendra el ejecutable de la simulacion donde podremos correr el ejecutable.

### _Manual de usuario:_
Una de vez ya hemos compilado nuestra simulacion si queremos correr la primera version del proyecto (unicamente OpenMP sin MPI)procederemos a correr el programa, esto lo haremos mediante la siguiente estructura de comando

`bin/heat_transfer test/job002/job002.txt`
Donde el primer argumento es nuestro ejecutable, y segundo debe ser la direccion de carpetas donde se encuentra nuestro archivo de trabajo (.txt) a leer, y el tercer argumento es el numero hilos a utilizar. El formato para el segundo argumento siempre debe ser de esta manera, especificando el directorio desde la raiz.


Si queremos correr la segunda entrega del proyecto (MPI), deberemos tener instalada alguna de las versiones de MPI, ya sea MPICH o OpenMPI, una vez lo tenemos instalado correriamos nuestro programa con

`mpiexec -np 3 bin/heat_transfer test/job002/job002.txt`

Especificando luego de `-np` la cantidad de procesos a utilizar. Por otro lado en el anterior comando utilizamos la prueba de trabajo numero 2 en la carpeta numero 2. la cual tiene de entrada el siguiente texto:

`plate000.bin    1    50  1000  0.0001`  
`plate001.bin    1    50  1000  0.00000000000001`  
`plate002.bin    1    25  1000  0.00000000000001`  
`plate003.bin    1    50  1000  0.0001`  
`plate004.bin    1    50  1000  0.0001`  
`plate005.bin    1    50  1000  0.0005`  
`plate006.bin   60  33.8  1200  0.000000005`  
`plate007.bin  120  10.7  1500  0.00000000000005`  

El Cual debera tener la siguiente salida:

`plate000.bin	1	50	1000	0.0001	1	0000/00/00	00:00:01`  
`plate001.bin	1	50	1000	1e-14	129570	0000/00/01	11:59:30`  
`plate002.bin	1	25	1000	1e-14	250939	0000/00/02	21:42:19`  
`plate003.bin	1	50	1000	0.0001	16566	0000/00/00	04:36:06`  
`plate004.bin	1	50	1000	0.0001	20795	0000/00/00	05:46:35`  
`plate005.bin	1	50	1000	0.0005	52982	0000/00/00	14:43:02`  
`plate006.bin	60	33.8	1200	5e-09	5433	0000/00/03	18:33:00`  
`plate007.bin	120	10.7	1500	5e-14	22761	0000/01/01	14:42:00`  

Ademas el programa produce la salidas de cada lamina resultante dentro de los trabajos, producto de la transaferencia de calor. Estos se producen en la raiz de `heat_transfer` y pueden ser visto para comprobar la matriz resultante. Una vez hacemos `make clean` estas laminas serán borradas.

### _Creditos:_

Edicion de archivo .Markdown:
https://dillinger.io/

Trabajo diseñado y creado por:  
Christopher Obando Salgado.  
Jose Alexis Valerio Ramirez.  
Andrey Esteban Mena Espinoza.  
Adrian Hutt Orozco.  
