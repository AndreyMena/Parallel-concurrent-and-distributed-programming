# Diseño de Tarea04, omp + mpi

### _Explicacion general del diseño:_
Para el diseño de esta optimizacion se tomo como base de codigo la implementacion de openmp. Pare este diseno se hizo uso de las funciones de mpi, donde como vemos en el diseno el proceso 0 es el encargado de recibir los datos e imprimirlos, es el, el que envia los numeros dinamicamente al resto de procesos correspondientes, los cuales haran los calculos de los mismo, para ello, se utiliza un mecanismo en el que se asigna un trabajo a cada proceso, luego de esto el resto sera enviado dinamico. De forma que cada vez que un proceso termina su trabajo, avisa al proceso 0 para que le encargue mas, si aun queda, le envia, y si no, le enviara una condicion de parada.

### _Creditos:_
Calculo de conjetura fuerte de goldbach (Adaptacion):
https://www.youtube.com/watch?v=ROEnh3ji-Oc

Adjuntacion de imagenes en Readme:
https://www.it-swarm-es.com/es/git/anade-imagenes-readme.md-en-github/1071767059/

Edicion de archivo .Markdown:
https://dillinger.io/

Trabajo diseñado y creado por:

Andrey Esteban Mena Espinoza, ECCI-UCR.
