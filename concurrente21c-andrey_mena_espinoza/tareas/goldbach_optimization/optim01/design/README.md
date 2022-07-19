# Diseño del Tarea03, optimizacion #1

### _Explicacion general del diseño:_
Para el diseño de esta optimizacion se uitiliza la misma base de codigo de la tarea02, solo que la distribucion de trabajo es por medio de un mapeo dinamico, el cual mediante un contador protegido por un mutex es aumentado cada vez que un hilo entra en la region, este asigna el indice correpondiente al hilo entrante y el hilo se encarga de trabajo en su region de la memoria (un arreglo), mediante el patron conditionally safe.

### _Explicacion de pseudocodigo:_
Primeramente cada hilo de ejecucion se asigna el numero que se encuentra en el indice correspondiente a su propio numero de hilo, una vez cada hilo a terminado de ejecutar su trabajo asignado, se encuentra dentro de un ciclo el cual cada vez que termine de realizar su trabajo, se comprobara, si aun queda mas trabajo, esto mediante condicionales dentro de un mutex, si ya no existe mas trabajo disponible, este sale del ciclo y termina su ejecucion.


### _Creditos:_
Calculo de conjetura fuerte de goldbach (Adaptacion):
https://www.youtube.com/watch?v=ROEnh3ji-Oc

Adjuntacion de imagenes en Readme:
https://www.it-swarm-es.com/es/git/anade-imagenes-readme.md-en-github/1071767059/

Edicion de archivo .Markdown:
https://dillinger.io/

Trabajo diseñado y creado por:

Andrey Esteban Mena Espinoza, ECCI-UCR.
