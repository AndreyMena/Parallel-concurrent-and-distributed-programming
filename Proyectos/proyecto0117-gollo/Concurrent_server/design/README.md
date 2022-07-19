### Diseño Proyecto 1.2:
![Diseño Proyecto 1.2:](https://git.ucr.ac.cr/ADRIAN.HUTT/proyecto0117-gollo/-/raw/main/Concurrent_server/design/DiagramaDeFlujoProy1_2.PNG)

#### Fase WebServer:
Como vemos en el diagrama una vez el cliente se conecta al servidor `HttpServer` e introduce los numeros, seran depositados en forma de Sockets a una cola mediante la cual posteriormente los hilos de ejecucion HttpConnectionHandler consumiran de la misma, de esta forma se podran conseguir multiples conexiones al mismo tiempo.

De igual manera los HttpConnectionHandler tambien produciran en una cola, teniendo una funcion de Assembler, donde producen un par, correspondiente a la solicitud, esta contiene el HttpRequest y el HttpResponse. Posteriormente estas solicitudes de respuesta seran recibidas por la `GoldBachWebApp` esto por medio de la cola que los une.

#### Fase WebAplication:
En esta fase nuestra `GoldBachWebApp`, como ya sabemos esta consumiendo solicitudes, solicitudes que se encarga de procesar y extraer sus numeros, estos numeros seran producidos por medio de un nodo privado que contiene un unico hilo, y un puntero otro nodo que compartiran cada hilo calculador. Estos `GoldBachWebApp`, que consumen estos nodos privado, calculan los numeros que se encuentran dentro de ellos y los depositan en la memoria compartida, una vez han calculado y depositado los datos de su respectivo numero, proceden a producir este nodo, en el `Empaquetador`.

El `Empaquetador` que recibe estos nodos privados, tiene que encargarse de observar cuando ya han sido procesados todas las solicitudes, esto lo hace internamente por medio de un mutex, en el que revisa si el nodo que ha recibido es el ultimo que completa la solicitud, si es asi. Produce el nodo compartido y final que sera recibido por el `Despachador`, el cual ensambla el mensaje y se lo despliega al usuario.

### _Diagramas UML de Proyecto1.2:_
![Diseño UML Proyecto 1.2:](https://git.ucr.ac.cr/ADRIAN.HUTT/proyecto0117-gollo/-/raw/main/Concurrent_server/design/DiagramaUMLProy1_2.PNG)

### _Pagina web donde el usuario ingresa sus numeros:_
![Diseño UML Proyecto 1.2:](https://git.ucr.ac.cr/ADRIAN.HUTT/proyecto0117-gollo/-/raw/main/Concurrent_server/design/CapturaServidor.PNG)

### _Resultado desplegado una vez son calculados los numeros:_
![Resultado](https://git.ucr.ac.cr/ADRIAN.HUTT/proyecto0117-gollo/-/raw/main/Concurrent_server/design/CapturaServidorRespuesta.PNG)


### _Creditos:_

Edicion de archivo .Markdown:
https://dillinger.io/
