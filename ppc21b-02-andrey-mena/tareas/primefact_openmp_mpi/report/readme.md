# Tarea01 primefact_pthread
### _Iteraciones realizadas:_
Se hicieron dos iteraciones.

    primefact_openmp
Una version de primefact mediante la concurrencia declarativa de OpenMP utilizando el mapeo ciclico por bloque que viene ya por default en OpenMP

    primefact_openmp_dinamic
En esta version se utilizo concurrencia declarativa para hacer un mapeo dinamico, en este caso tambien con OpenMP.

### _Duracion_:
Comparacion de duracion para caso de prueba largo:
![Duracion](https://git.ucr.ac.cr/ANDREY.MENAESPINOZA/ppc21b-02-andrey-mena/-/raw/main/tareas/primefact_openmp_mpi/report/OpenMP_Large.png)

### _Incremento de velocidad_:
En ambos casos hubo un incremento de velocidad considerable, se podria decir que practicamente igualo nuestras dos optimizaciones anteriores hechas mediante pthreads, como era de esperar supero a la version serial considerablemente.

Comparacion de speedup para caso de prueba largo:
![Incremento de velocidad](https://git.ucr.ac.cr/ANDREY.MENAESPINOZA/ppc21b-02-andrey-mena/-/raw/main/tareas/primefact_openmp_mpi/report/SpeedupOpenMP_Large.png)

Comparacion de velocidad para caso de prueba medium:

### _Eficiencia_:
En cuanto a eficiencia se mantuvieron bastante parecidos respecto a las versiones con pthreads con un 2.10 para primefact_openmp y un 2,133936015 para primefact_openmp_dinamic. NUevamente al igual que nuestras versiones con pthreads es una eficiencia bastante baja.

Comparacion de eficiencia de OpenMP (con caso de preuba largo) respecto a versiones anteriores:
![Incremento de velocidad](https://git.ucr.ac.cr/ANDREY.MENAESPINOZA/ppc21b-02-andrey-mena/-/raw/main/tareas/primefact_openmp_mpi/report/EficienciaOpenMP_Large.png)

### _Discusion_:
Finalmente se puede concluir que OpenMP es una buena alternativa para como ya es sabido escribir menos codigo, pero tambien hemos visto que esto no significa que sea mejor o mas rapida que pthread como fue en este caso se mantuvieron bastante parecidos.
