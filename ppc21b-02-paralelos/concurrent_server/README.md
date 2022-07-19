# Manual de usuario
## Descripción del problema
Se entrega como código base un servidor serial, en el cual se implementará la funcionalidad de calcular factores primos 
de números que el cliente ingresa mediante la página web. Sin embargo, un servidor serial no tiene un rendimiento óptimo, por lo tanto se construyo un servidor concurrente que le aceptar más de un cliente a la vez esto se hace logrará con el patrón productor-consumidor. (Ver diseño del programa para más información)


### Diseño del programa
El programa se desarrolla de la siguiente manera, el usuario digitará un número y este se guardará en una cola este proceso será nuestro productor,
después se consultará la cola, esta cola es un thread safe, esto básicamente evitará la doble lectura de números de diferentes consumidores; después de seleccionar un número, el encargado de consultar y seleccionar el número es el consumidor y este irá a la aplicación y se calcularán los primos del número deseado, es importante destacar que solo se puede digitar un número a la vez, si se intenta digitar más de un número se mostrará un error en pantalla.
Puede consultar el diseño en pseudocódigo en [este enlace](https://git.ucr.ac.cr/LUIS.BARRANTESROJAS/ppc21b-02-paralelos/-/blob/main/concurrent_server/design/DiagramaProyUml.PNG)

Puede consultar los archivos asociados al diseño en [este enlace](https://git.ucr.ac.cr/LUIS.BARRANTESROJAS/ppc21b-02-paralelos/-/blob/0c700adc10f2b9f46489df8dc9f9e953e28e6845/concurrent_server/design/documento_de_diseño.md). 

### Manual de uso
Para compilar el código, ubíquese en la carpeta raíz `concurrent_server` en su terminal y digite:
```
make
```
Con esta instrucción se compilará el código.

### Ejecución
Una vez compilado el código, ejecute el servidor con la siguiente instrucción:
```
bin/concurrent_server 8080
```

Puede modificar el número de puerto con el que se desea correr el servidor.

### Detención del servidor
Puede detener el server con la con `ctrl+c`, `command+c` o con el comando `kill`.


## Integrantes
Andrés Fallas Mena

Andrey Mena Espinoza

Christopher Obando Salgado

Luis Diego Barrantes Rojas
