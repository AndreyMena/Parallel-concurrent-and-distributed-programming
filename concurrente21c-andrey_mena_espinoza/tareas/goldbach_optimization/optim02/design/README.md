# Diseño del Tarea03, optimizacion #2

### _Explicacion general del diseño:_
Para el diseño de esta optimizacion se tomo como base de codigo la primer optimizacion de la tarea 3, se hicieron modifacion de orden, en las condicionales de las conjeturas, para asi reducir la probabilidad de llegar a los calculos mas duraderas. Tambien se modifico el metodo isPrime() para hacerlo mas rapido.

### _Explicacion de pseudocodigo:_
Como vemos en el pseudocodigo, anteriormente haciamos las verificaciones de primos antes de condicionales como el de ver si un numero ya ha sido valorado en distinto orden, esto ocasionaba ciclos de isPrime() que luego no valdrian de nada ya que seria numeros desechados, se colocaron las comprobaciones de isPrime() al final de la condicion, y la comprobacion de repeticion al principio, ademas la funcion de isPrime() aumenta de dos en dos, y hace el calculo de sqrt() una sola vez.

### _Creditos:_
Calculo de conjetura fuerte de goldbach (Adaptacion):
https://www.youtube.com/watch?v=ROEnh3ji-Oc

Adjuntacion de imagenes en Readme:
https://www.it-swarm-es.com/es/git/anade-imagenes-readme.md-en-github/1071767059/

Edicion de archivo .Markdown:
https://dillinger.io/

Trabajo diseñado y creado por:
Andrey Esteban Mena Espinoza, ECCI-UCR.
