# INFO145: Diseño y Análisis de Algoritmos
## Proyecto Semestral: Técnicas de Representación y Compresión en Arreglos Ordenados

## Descripcion
Este proyecto tiene como objetivo evaluar empírica y teóricamente el compromiso (*trade-off*) entre el **espacio de almacenamiento** y el **tiempo de ejecución** en estructuras de datos avanzadas. A través de la implementación de tres estrategias distintas, el sistema mide y compara la eficiencia al almacenar y buscar elementos sobre arreglos ordenados de gran magnitud.

El proyecto se divide en 3 partes:
1. **Representación Explícita (Línea Base):** Almacenamiento estándar y directo de los elementos utilizando vectores nativos (`std::vector<int>`), donde la búsqueda se realiza mediante el algoritmo de Búsqueda Binaria.
2. **Gap Coding apoyado por Muestreo:** Estructura compacta que reduce la magnitud de los valores guardando la diferencia matemática (*gap*) entre elementos contiguos. Se apoya en un arreglo de muestras (*Sample*) a una distancia paramétrica $b$ para acotar los rangos de búsqueda.
3. **Compresión a través de Shannon-Fano:** Utilizando el algoritmo de codificación de Shannon-Fano sobre el arreglo de *gaps* del Caso 2, se comprime cada elemento a nivel de bits asignando códigos más cortos a las diferencias más frecuentes. Al buscar, el flujo de bits se decodifica dinámicamente en memoria, lo que permite evaluar si la sustancial reducción de espacio compensa el costo de cómputo extra añadido.

## Compilacion y ejecucion
Ejecutar make antes para compilar

### Modo Benchmark
Modo creado para ejecutar automaticamente los 3 casos implementados,
realizar las pruebas y exportar los datos a un archivo .csv.

- se debe ejecutar como **./main --benchmark**

### Modo archivo
Modo creado para hacer pruebas manuales con datos personalizados extraidos de un archivo .csv,
permitiendo al usuario elejir de manera interactiva que caso desea utilizar.

- Se debe ejucutar como **./main -i ruta/del/archivo.csv**
- El programa trabaja con **int** (32 bits)
- El rango valido para los valores es de **-2^31 y 2(^31)-1**

