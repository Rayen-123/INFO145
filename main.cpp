#include <iostream>
#include <vector>
#include <string>
#include "caso1.hpp"

using namespace std;

// Función auxiliar para imprimir los arreglos en la consola
void imprimirArreglo(const vector<int>& arr, const string& nombre) {
    cout << nombre << ": [";
    for (size_t i = 0; i < arr.size(); ++i) {
        cout << arr[i] << (i < arr.size() - 1 ? ", " : "");
    }
    cout << "]" << endl;
}

int main() {
    cout << "probar distribución lineal" << endl;
    vector<int> arr_lineal = Caso1::generarLineal(15, 5);
    imprimirArreglo(arr_lineal, "Arreglo Lineal");

    cout << "probar distribución normal" << endl;
    vector<int> arr_normal = Caso1::generarNormal(15, 50, 10);
    imprimirArreglo(arr_normal, "Arreglo Normal");

    cout << "probar Búsqueda Binaria (Caso de éxito)" << endl;
    // Extraemos un valor que sabemos que existe (ej. el que quedó en la mitad)
    int objetivo_existente = arr_lineal[arr_lineal.size() / 2];
    cout << "Buscando el numero " << objetivo_existente << " en el Arreglo Lineal..." << endl;
    
    int pos_encontrado = Caso1::busquedaBinaria(arr_lineal, objetivo_existente);
    if (pos_encontrado != -1) {
        cout << " -> EXITO: Numero encontrado en el indice [" << pos_encontrado << "]" << endl;
    } else {
        cout << " -> ERROR: El numero no fue encontrado." << endl;
    }

    cout << "probar Búsqueda Binaria (Caso de fallo)" << endl;
    // Buscamos un número que seguramente no existe (uno negativo)
    int objetivo_falso = -100;
    cout << "\nBuscando el numero " << objetivo_falso << " en el Arreglo Lineal..." << endl;
    
    int pos_falso = Caso1::busquedaBinaria(arr_lineal, objetivo_falso);
    if (pos_falso != -1) {
        cout << " -> ERROR: Encontro un numero que no deberia existir." << endl;
    } else {
        cout << " -> EXITO: Comportamiento correcto, retorno -1 (No encontrado)." << endl;
    }

    cout << "Bytes ocupados por el Arreglo Lineal: " << Caso1::calcularEspacio(arr_lineal) << " bytes" << endl;

    return 0;
}