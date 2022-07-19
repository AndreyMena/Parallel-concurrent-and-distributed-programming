# Diseño del Tarea04.

### _Explicacion general del diseño:_
Para el diseño de esta optimizacion se tomo como base de codigo la segunda optimizacion de la tarea 3, se hicieron modifaciones a la hora del almacenamiento dentro de los calculos de goldbach, ya que al estos ser dinamicos, su almacenamiento tambien lo es, por lo que se tuvo que utilizar conditionaly safe, para que no hubieran condiciones de carrera. para ello se utilizo un calculo el cual divide el espacio en todos los factores posibles, en caso de conjetura debiles el maximo factor es un tercio del numero real ya que su suma se compone por 3 factores, y en caso de la conjetura fuerta, el maximo factor es la mitad del numero mismo. Teniendo en cuenta esto realizamos un calculo de espacio para tener el suficiente espacio que se ocupara.

Fuera del cambio en las conjeturas para que alamacenen dinamicamente sin chocar, tenemos el agregado de los pragmas omp parallel for, los cuales corresponden a la zona a paralelizar, con la utilizacion de openmp.

### _Creditos:_
Calculo de conjetura fuerte de goldbach (Adaptacion):
https://www.youtube.com/watch?v=ROEnh3ji-Oc

Adjuntacion de imagenes en Readme:
https://www.it-swarm-es.com/es/git/anade-imagenes-readme.md-en-github/1071767059/

Edicion de archivo .Markdown:
https://dillinger.io/

Trabajo diseñado y creado por:
Andrey Esteban Mena Espinoza, ECCI-UCR.
