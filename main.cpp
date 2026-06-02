#include <iostream>
#include <vector>
#include <string>
#include "caso1.hpp"
#include "caso2.hpp"
#include "caso3.hpp"

using namespace std;

// Función auxiliar para imprimir los arreglos en la consola
void imprimirArreglo(const vector<int>& arr, const string& nombre) {
    cout << nombre << ": [";
    for (size_t i = 0; i < arr.size(); ++i) {
        cout << arr[i] << (i < arr.size() - 1 ? ", " : "");
    }
    cout << "]" << endl;
}

void benchmark(){
    // crear archivo csv


    const vector<int> t = {1000, 10000, 50000, 100000};
    for (int &n : t) {
        //Caso 1

        //Caso 2

        //Caso 3
        //Caso3::Resultados m = Caso3::caso3(struct caso2, valor a buscar);

    }
    return;
}

void archivo(const string& rutaArchivo){
    //leer los datos de un csv

    int e = 0, valor = 0;
    while(true){
        int e;
        cout << "1. Caso 1(Busqueda Binaria)" << endl;
        cout << "2. Caso 2(Gap Coding)" << endl;
        cout << "3. Caso 3(Shannon-Fano)" << endl;
        cout << "4. salir" << endl;
        cin >> e;
        
        if (e == 4){
            break;
        }

        cout << "Valor a buscar: ";
        cin >> valor;
        if (e == 1){
            //ejecutar caso 1 y mostrar resultados
        }
        if (e == 2){
            //ejecutar caso 2 y mostrar resultados
        }
        if (e == 3){
            //ejecutar caso 3 y mostrar resultados
        }
    }
}
int main(int argc, char* argv[]) {
    // hay que terminar las formas de ejecucion del programa
    // y pasar la logica de lo que esta en el main a estas :$
    if (argc == 2 && strcmp(argv[1], "--benchmark") == 0) {
        benchmark();
    } 
    else if (argc == 3 && strcmp(argv[1], "-i") == 0) {
        archivo(argv[2]);
    }

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
    cout << "Buscando el numero " << objetivo_falso << " en el Arreglo Lineal..." << endl;
    
    int pos_falso = Caso1::busquedaBinaria(arr_lineal, objetivo_falso);
    if (pos_falso != -1) {
        cout << " -> ERROR: Encontro un numero que no deberia existir." << endl;
    } else {
        cout << " -> EXITO: Comportamiento correcto, retorno -1 (No encontrado)." << endl;
    }

    cout << "Bytes ocupados por el Arreglo Lineal: " << Caso1::calcularEspacio(arr_lineal) << " bytes" << endl;

    //Caso 2

    cout << "\n===== CASO 2: GAP CODING =====" << endl;

   size_t b = 3;

   Caso2::GC Gap = Caso2::construir(arr_lineal, b);
   cout << "Gap Coding construido correctamente." << endl;
   imprimirArreglo(Gap.GC, "GC");
   imprimirArreglo(Gap.sample, "Sample");

   // ---------- BUSQUEDA EXITOSA ----------
   cout << "\nBusqueda Caso 2 (Exito)" << endl;
   int objetivo_gap = arr_lineal[arr_lineal.size()/2];
   cout << "Buscando " << objetivo_gap << endl;
   int pos_gap = Caso2::buscar(estructura, objetivo_gap);

   if(pos_gap != -1){
       cout << " -> EXITO: encontrado en indice "
            << pos_gap << endl;
   }
   else{
       cout << " -> ERROR: no encontrado" << endl;
   }

   // ---------- BUSQUEDA FALLIDA ----------
   cout << "\nBusqueda Caso 2 (Fallo)" << endl;

   int inexistente = -100;

   int pos_gap_falso =
       Caso2::buscar(Gap, inexistente);

   if(pos_gap_falso != -1){
       cout << " -> ERROR: encontro un valor inexistente"
            << endl;
   }
   else{
       cout << " -> EXITO: retorno -1 correctamente"
            << endl;
   }

   // ---------- ESPACIO ----------
   cout << "\nEspacio usado por Gap Coding: "
        << Caso2::calcularEspacio(Gap)
        << " bytes" << endl;
   
   



    //Caso 3
    Caso3::caso3(Gap, objetivo_existente);

    return 0;
}
