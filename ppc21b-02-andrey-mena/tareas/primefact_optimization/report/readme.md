# Tarea01 primefact_pthread
### _Iteraciones realizadas:_
Se hicieron dos iteraciones.

    optim01_dynamic
Una optimizacion en la que se decidio implementar el mapeo dinamico. En el que una vez un hilo se desocupa y aun existen elementos por consumir este elije el siguiente elemento (en este caso el numero) y lo factoriza, luego lo guara en un arreglo de salidas, para cuando ya todos los hilos hayan terminado imprimirlas todas en orden.

    optim02_block
Y esta optimizacion fue la segunda utilizando un mapeo por bloque en el que cada hilo ya tiene asignado desde un principio el bloque de numeros que va a estar factorizando, en este mapeo si el hilo termina su trabajo no adquiere mas numeros.

### _Duracion_:
Comparacion de duracion para caso de prueba largo:
![Duracion](https://git.ucr.ac.cr/ANDREY.MENAESPINOZA/ppc21b-02-andrey-mena/-/raw/main/tareas/primefact_optimization/report/ComparacionLarge.png)

### _Incremento de velocidad_:
En ambos casos hubo un incremento de velocidad considerable, pero no tanto como se esperaba, se duplic la velocidad utilizando una maquina con 6 cpu's. Para la optimizacion1 se obtuvo un speedup de 2.14 en un caso de prueba largo. Y para la optimizacion2 un speedup de 2.09, por lo que no variaron tanto entre las dos optimizaciones

Comparacion de velocidad para caso de prueba largo:
![Incremento de velocidad](https://git.ucr.ac.cr/ANDREY.MENAESPINOZA/ppc21b-02-andrey-mena/-/raw/main/tareas/primefact_optimization/report/SpeedupLargeOptimization.png)

### _Eficiencia_:
En cuanto a la eficiencia disminuyeron bastante ambos casos, bajando hasta un 0.30 de uno. Ya que en el caso de la optimizacion1 bajo a un 0.36 y la optimizacion2 hasta un 0.35.
![Eficiencia](https://git.ucr.ac.cr/ANDREY.MENAESPINOZA/ppc21b-02-andrey-mena/-/raw/main/tareas/primefact_optimization/report/EficienciaLargeOptimization.png)

### _Discusion_:
Finalmente se puede concluir que las optimizaciones dieron sus frutos ya que efectivamente se logro aumentar la velocidad, no tanto como se esperaba pero en todos los casos en los que se hizo pruebas(exceptuando casos sumamente pequenos) se consiguio duplicar la velocidad en ambas optimizaciones respecto a la version serial.
