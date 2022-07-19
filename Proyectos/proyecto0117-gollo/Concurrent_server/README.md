# Proyecto1.1: Servidor concurrente
### _Integrantes:_
Christopher Obando Salgado

Jose Alexis Valerio Ramirez.

Andrey Esteban Mena Espinoza

Adrian Hutt Orozco.

### _Descripcion del problema:_
Crear un `servidor concurrente` que consiga calcular sumas de goldbach correctamente. Sera brindado un codigo base, el cual deberá ser completado de forma que logre aceptar multiples conexiones al mismo tiempo, estas conexiones representa los visitantes, los cuales podrán proveer listas de numeros sobre los que desean calcular sus sumas de goldbach.

Para el calculo de las sumas, se basara en las conjeturas de goldbach, la conjetura fuerte, para numeros pares y la conjetura debil para numeros impares. En el caso de la conjetura fuerte, cada numero par, es representado mediante la suma de dos factores primos. Y para la suma de goldbach mediante conjetura debil, cada numero impar es representado mediante la  suma de 3 factores primos. Numeros en el rango de `0 <= numero <= 5`, imprimiran NA, ya que para estos no aplican las conjeturas. Para numeros negativos se interpretara que el usuario desea conocer sus sumas de goldbach especificamente ademas de la cantidad de sumas.

### _Manual de usuario:_
Una vez descargado el repositorio, para correr el servidor deberemos posicionarnos en la carpeta `concurrent_server/`, donde abriremos una terminal y  procedermos a compilar el programa mediante el comando `make clean; make`, enseguida se creara una carpeta `bin/` que contendra el ejecutable del servidor, donde mediante el comando `bin/concurrent_server`, se creara un servidor en el puerto 8080, si este puerto ya esta siendo ocupado podemos especificar otros puerto mediante el segundo argumento, por ejemplo `bin/concurrent_server 1234`. Ahora si, ya tenemos nuestro servidor en curso, podemos acceder a el, abriendo una venta en nuestro navegador web, y dirigendonos al url `localhost:8080`, donde veremos una ventana como la siguiente:

Para probar la concurrencia del servidor podemos hacer uso de pruebas de estres, como las que nos provee `httperf`, donde podemos simular multiples conexiones y ver como se comporta el programa con las mismas, para esto primero debemos correr nuestro servidor en una terminal y luego abrir otra y correr el siguiente comando: `httperf --server localhost --port 8080 --uri /goldbach?number=123 --num-conns 2000 --rate 1000 --num-call 3 --timeout 1`.

### _Creditos:_

Edicion de archivo .Markdown:
https://dillinger.io/
