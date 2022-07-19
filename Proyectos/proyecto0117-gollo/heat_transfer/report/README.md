## Comparaciones de rendimiento, gráficas, y discusión

### Proyecto 2.1:
Como vemos en el siguiente grafico hemos tenido un decremento en el desempeño dadas las implementacion de OpenMP, respecto a la version, serial, donde nuestra version serial ha sido la mas rapida y nuestras implementaciones concurrentes mediante los mapeos colocados en schedule (block, cyclic, blockcyclic, dynamic, guided), han tenido todos una duracion similar donde la de bloque cyclic ha sido la ha tenido un mejor rendimiento entre todas las concurrentes.
#### Grafico proyecto2_1
![Grafico proyecto2_1](https://git.ucr.ac.cr/ADRIAN.HUTT/proyecto0117-gollo/-/raw/main/heat_transfer/report/RendimientoProy2_1.PNG)

### Proyecto 2.2:
Como vemos en este grafico, al contrario del anterior, aquí si tuvimos un incremento en el rendimiento, respecto a la duracion, speedup y eficiencia, siendo este el mejor de todos. Cabe aclarar que la prueba se hizo con 3 computadoras de 8 nucleos (arenal) y en el caso de prueba job003.txt. Igualmente que la prueba de OpenMP y serial. Tiene sentido el incremento del desempeño debido a que los procesos se dividen las laminas.

#### Grafico proyecto2_1
![Grafico proyecto2_1](https://git.ucr.ac.cr/ADRIAN.HUTT/proyecto0117-gollo/-/raw/main/heat_transfer/report/RendimientoProy2_2.PNG)
