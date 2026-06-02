# INFO145

## Descripcion
Este proyecto tiene como objetivo asdasdasdasdasdasdasd
El proyecto se divide en 3 partes:
1. 
2. 
3. Compresion atravez de Shannon-Fanno: Utilizando el algoritmo de codificacion de Shannon-Fanno sobre el arreglo gaps, se comprime a nivel de bits cada elemento, al buscar se decodifica el arreglo de bits lo que nos permite comparar si la reduccion de memoria utilizada compensa el costo extra anadido. 

## Compilacion y ejecucion

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

