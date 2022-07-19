# Diseño

Para el diseño de la solución del proyecto se desarrolló el siguiente diagrama, a partir del cual se describen las diferentes partes.

![alt text](concurrent_server/design/P1.2-1.2.png)

### Sección 1
Un usuario al utilizar el navegador puede realizar una solicitud, en este caso, ingresa números de los cuales le interesa conocer su factorización prima. Inicialmente, el procesamiento de estas solicitudes se realiza de forma serial, lo que es ineficiente y no representa el comportamiento real de un servidor, el cual puede recibir múltiples solicitudes de una gran cantidad de usuarios.

### Sección 2
Las solicitudes (números en este caso) se empaquetan en sockets, los cuales son el medio por el cual los servicios se pueden comunicar (por ejemplo, el servidor web y la aplicación web). El socket del usuario contiene información de la solicitud realizada y de la respuesta. El servidor web por su parte, será el encargado de colocar en una cola los sockets.

### Sección 3
Los objetos HttpConnectionHandler para este avance, se harán cargo de recibir la cola de sockets que envía el servidor web e identificar cuál es la aplicación web que corresponde. Para este avance, se utiliza FactWebApp. Los objetos HttpConnectionHandler no atienden solicitudes.

### Sección 4
Arreglo del servidor web que contiene todas las aplicaciones a las cuales sirve.

### Sección 5
Se tienen objetos (nodos) los cuales tienen los datos que se generan a partir de las solicitudes de los clientes, como por ejemplo request, response, la cantidad de números por procesa, un arreglo de números y un arreglo de impresión (conditionally safe). También contiene el estado de la condición de parada.

### Sección 6
Cola a enviar a factorizar, cada elemento en la cola contiene un número y la referencia al nodo al que le pertenece ese número. Esta cola la envía la aplicación FactWebApp a FactCalculator.

### Sección 7
FactCalculator son hilos de tipo ensamblador, es decir, reciben una cola de números y referencias a nodos (sección 5), factorizan los números y los agregan a un arreglo conditionally safe que se encuentra en el nodo. Agregan a una cola la referencia a los nodos mediante una cola y los envía.

### Sección 8
Cola a enviar al dispacher, que contiene la referencia a los nodos.

### Sección 9
El dispacher recibe la cola de nodos, como se indicó que la sección 5, hay un arreglo que es conditionally safe, el cual es actualizado por el FactCalculator, y un contador, el cual, cada vez que se procesa un número va incrementando. El dispacher verificará el cumplimiento de la condición numbers_count == numbers_amount, si se cumple, envía al cliente la factorización completa de los elementos que ingresó, caso contrario, seguirá consumiendo de la cola.
Por otro lado, también verificará si existe una condición de parada, para hacer que el servidor finalice correctamente.

## Diagrama UML
Se cuenta con diferentes clases, sin embargo, las reelvantes para esta parte del proyecto son HttpServer, FactCalculator y HttpConnectionHandler.
La primer clase, HttpServer, se encarga de correr y detener el servicio, escuchar siempre las solicitudes de los clientes y analizar los argumentos que se pueden llegar a colocar, además se encarga de recibir las solicitudes de los usuarios (sockets) y agregarlo a una cola, la cual la clase HttpConnectionHandler estará recibiendo, y agregando a la cola que le corresponde procesar a una aplicación determinada.

![alt text](concurrent_server/design/UmlProy1.2.PNG)

