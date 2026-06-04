#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstring>
#include <chrono>
#include <numeric>
#include "caso1.hpp"
#include "caso2.hpp"
#include "caso3.hpp"

using namespace std;

// Helper para imprimir arreglos pequeños si es necesario en depuración
void imprimirArreglo(const vector<int>& arr, const string& nombre) {
    cout << nombre << ": [";
    for (size_t i = 0; i < arr.size(); ++i) {
        cout << arr[i] << (i < arr.size() - 1 ? ", " : "");
    }
    cout << "]" << endl;
}

// MODO BENCHMARK: Generación automática, medición y exportación a CSV
void benchmark() {
    ofstream archivo("benchmark.csv");
    if (!archivo.is_open()) {
        cerr << "Error al crear el archivo benchmark.csv" << endl;
        return;
    }
    
    // Encabezados solicitados por el hito
    archivo << "Caso, n, Tiempo Construccion (ms), Tiempo Busqueda (ms), Espacio Usado (bytes)\n";

    // Tamaños incrementales en potencias de 10
    const vector<size_t> tamanos = {1000, 10000, 100000, 1000000};
    const int epsilon = 5;
    const size_t b = 32; // Salto paramétrico para el Sample

    for (size_t n : tamanos) {
        cout << "Procesando benchmark para n = " << n << "..." << endl;

        // --- CASO 1 ---
        auto startBuildC1 = chrono::high_resolution_clock::now();
        vector<int> arr_lineal = Caso1::generarLineal(n, epsilon);
        auto endBuildC1 = chrono::high_resolution_clock::now();
        
        double buildTimeC1 = chrono::duration<double, milli>(endBuildC1 - startBuildC1).count();
        size_t espacioC1 = Caso1::calcularEspacio(arr_lineal);

        // Realizar múltiples búsquedas para promediar (exigido por el enunciado)
        int buscar_c1 = arr_lineal[n / 2]; 
        auto startSearchC1 = chrono::high_resolution_clock::now();
        for(int i = 0; i < 100; ++i) {
            Caso1::busquedaBinaria(arr_lineal, buscar_c1);
        }
        auto endSearchC1 = chrono::high_resolution_clock::now();
        double searchTimeC1 = chrono::duration<double, milli>(endSearchC1 - startSearchC1).count() / 100.0;

        archivo << "Caso 1, " << n << ", " << buildTimeC1 << ", " << searchTimeC1 << ", " << espacioC1 << "\n";

        // --- CASO 2 ---
        auto startBuildC2 = chrono::high_resolution_clock::now();
        Caso2::GC estructC2 = Caso2::construir(arr_lineal, b);
        auto endBuildC2 = chrono::high_resolution_clock::now();

        double buildTimeC2 = chrono::duration<double, milli>(endBuildC2 - startBuildC2).count();
        size_t espacioC2 = Caso2::calcularEspacio(estructC2);

        auto startSearchC2 = chrono::high_resolution_clock::now();
        for(int i = 0; i < 100; ++i) {
            Caso2::buscar(estructC2, buscar_c1);
        }
        auto endSearchC2 = chrono::high_resolution_clock::now();
        double searchTimeC2 = chrono::duration<double, milli>(endSearchC2 - startSearchC2).count() / 100.0;

        archivo << "Caso 2, " << n << ", " << buildTimeC2 << ", " << searchTimeC2 << ", " << espacioC2 << "\n";

        // --- CASO 3 ---
        // El Caso 3 internamente mide su propia construcción y búsqueda
        Caso3::resultados resC3 = Caso3::caso3(estructC2, buscar_c1);
        
        archivo << "Caso 3, " << n << ", " << resC3.buildTime << ", " << resC3.searchTime << ", " << resC3.totalBytes << "\n";
    }

    archivo.close();
    cout << "¡Benchmark completado con éxito! Resultados guardados en 'benchmark.csv'." << endl;
}

// MODO ARCHIVO: Lee un CSV con enteros y permite búsquedas interactivas
void archivo(const string& rutaArchivo) {
    ifstream file(rutaArchivo);
    if (!file.is_open()) {
        cerr << "Error al abrir el archivo: " << rutaArchivo << endl;
        return;
    }

    vector<int> datosOriginales;
    string linea;
    
    // Extraer datos del CSV (asumiendo un entero por línea o separados por comas/saltos)
    while (getline(file, linea)) {
        if (!linea.empty()) {
            datosOriginales.push_back(stoll(linea)); // Usando stoll por seguridad de rango
        }
    }
    file.close();

    if (datosOriginales.empty()) {
        cerr << "El archivo no contiene datos válidos." << endl;
        return;
    }

    // Asegurar que el arreglo base esté ordenado para que funcionen las búsquedas
    sort(datosOriginales.begin(), datosOriginales.end());

    // Construir estructuras base
    size_t b = 32;
    Caso2::GC estructC2 = Caso2::construir(datosOriginales, b);

    int opcion = 0;
    int valorBuscado = 0;

    while (true) {
        cout << "\n========================================" << endl;
        cout << "Estructuras cargadas con " << datosOriginales.size() << " elementos." << endl;
        cout << "1. Buscar en Caso 1 (Búsqueda Binaria Estándar)" << endl;
        cout << "2. Buscar en Caso 2 (Gap Coding)" << endl;
        cout << "3. Buscar en Caso 3 (Shannon-Fano)" << endl;
        cout << "4. Salir" << endl;
        cout << "Seleccione una opción: ";
        cin >> opcion;

        if (opcion == 4) break;

        cout << "Ingrese el valor entero a buscar: ";
        cin >> valorBuscado;

        if (opcion == 1) {
            auto start = chrono::high_resolution_clock::now();
            int pos = Caso1::busquedaBinaria(datosOriginales, valorBuscado);
            auto end = chrono::high_resolution_clock::now();
            double elapsed = chrono::duration<double, milli>(end - start).count();

            if (pos != -1) cout << "-> Encontrado en Caso 1. Posición: " << pos;
            else cout << "-> Valor NO encontrado en Caso 1.";
            cout << " Tiempo de búsqueda: " << elapsed << " ms" << endl;
        } 
        else if (opcion == 2) {
            auto start = chrono::high_resolution_clock::now();
            int pos = Caso2::buscar(estructC2, valorBuscado);
            auto end = chrono::high_resolution_clock::now();
            double elapsed = chrono::duration<double, milli>(end - start).count();

            if (pos != -1) cout << "-> Encontrado en Caso 2. Posición: " << pos;
            else cout << "-> Valor NO encontrado en Caso 2.";
            cout << " Tiempo de búsqueda: " << elapsed << " ms" << endl;
        } 
        else if (opcion == 3) {
            // Se invoca la lógica de Shannon-Fano diseñada por tu grupo
            Caso3::resultados m = Caso3::caso3(estructC2, valorBuscado);

            if (m.pos != -1) cout << "-> Encontrado en Caso 3. Posición: " << m.pos;
            else cout << "-> Valor NO encontrado en Caso 3.";
            
            cout << "\n[Métricas Shannon-Fano]:" << endl;
            cout << "   Tiempo de construcción estructura: " << m.buildTime << " ms" << endl;
            cout << "   Tiempo de búsqueda específico: " << m.searchTime << " ms" << endl;
            cout << "   Espacio total estimado de la estructura: " << m.totalBytes << " bytes" << endl;
        } 
        else {
            cout << "Opción inválida." << endl;
        }
    }
}

int main(int argc, char* argv[]) {
    // Control estricto de los argumentos de ejecución por línea de comandos
    if (argc == 2 && strcmp(argv[1], "--benchmark") == 0) {
        benchmark();
        return 0;
    } 
    else if (argc == 3 && strcmp(argv[1], "-i") == 0) {
        archivo(argv[2]);
        return 0;
    } 
    else {
        // Mensaje de ayuda si no se ingresan parámetros válidos
        cout << "Uso del programa:" << endl;
        cout << "  Modo Benchmark: " << argv[0] << " --benchmark" << endl;
        cout << "  Modo Archivo:   " << argv[0] << " -i <ruta_archivo.csv>" << endl;
        return 1;
    }
}