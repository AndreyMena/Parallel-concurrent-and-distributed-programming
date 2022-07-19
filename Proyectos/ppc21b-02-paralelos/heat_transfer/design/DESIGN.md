# Documento de diseño
Para el desarrollo de este proyecto se implementaron dos clases, la principal o main, encargada de ejecutar el programa, y la clase simulation.

![alt text](heat_transfer/design/Diagrama_1.png)

La clase principal, estará inicializando los valores a procesar, como la plantilla de trabajo, tiempo, difusividad térmica, dimensiones de las celdas, el valor de épsilon y la ruta en la que se encuentran las plantillas u archivos .bin. Esta clase se encargará de iniciar la simulación.
Para la clase simulation se tienen diversos métodos, los cuales están encargados de realizar las siguientes tareas:

1)	Calcular los tiempos de ejecución de cada simulación con cpu_time( ) y timestamp( ).
2)	Cargar el archivo de trabajo ( load_job_file ) con los parámetros para realizar la simulación.
3)	Función para la lectura de archivos bin, extraer dimensiones y transformarlos el input en matriz.
4)	Subrutina para iniciar la simulación.
