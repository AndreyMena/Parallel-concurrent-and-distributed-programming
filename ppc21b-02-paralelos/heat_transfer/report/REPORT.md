# Reporte de análisis de optimización

## Optimización
## Versión OpenMp
### Medir el rendimiento del código antes de realizar las modificaciones.
El rendimiento medido del código de la versión serial es de 337.4367 segundos.

### Analizar el código para detectar las regiones críticas a optimizar (profiling).
A partir del uso de la herramienta callgrind, se determina que una de las regiones críticas del código se encuentra en el ciclo en el que se ejecuta la ecuación del modelo físico.

### Hacer las modificaciones que se cree incrementarán el rendimiento en las regiones críticas.
Se agregó el paralelismo con openMP en la región identificada como crítica.

### Asegurarse de que las modificaciones sean correctas.
El código se ejecuta de forma correcta según lo solicitado.

### Rendimiento del Código después de las modificaciones.
Se logra un aumento en el rendimiento. Ver documento adjunto.

### Lecciones aprendidas.
Se había intentado hacer el paralelismo en otras partes del código, donde realmente no tenían un impacto positivo, más bien iba en detrimento del rendimiento, por lo tanto se recomienda antes de intentar realizar una optimización hacer el mapeo de rendimiento con callgrind.


# Comparación de las optimizaciones

### Optimizations
#### CPU = 6

| Level | serial | guided | dynamic | 
| ------ | ------ | ------ | ------ |
| Duration  | 199.293 | 87.399| 34.755 |
| Speedup | 1.0 | 2.28 | 5.73 |
| Efficiency | 0.125 | 0.285 | 0.72 |

![Gráfico de optimizaciones](heat_transfer/report/rendimiento.png)
