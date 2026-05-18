#include <iostream>
#include <vector>
#include <random>
#include <algorithm>

using namespace std;

namespace Caso1 {

    vector<int> generarLineal(size_t n, int epsilon) {;
        vector<int> A(n);
        
        random_device rd; // Semilla para el generador de números aleatorios
        mt19937 gen(rd()); // Generador de números aleatorios 
        uniform_int_distribution<int> dist_base(1, 100); // Rango arbitrario para A[0]
        uniform_int_distribution<int> dist_eps(1, epsilon); // Evitamos 0 para garantizar orden estricto

        A[0] = dist_base(gen); 
        for (int i = 1; i < n; ++i) {
            A[i] = A[i - 1] + dist_eps(gen); // Asegura que cada número sea mayor que el anterior, con una diferencia controlada por epsilon.
        }
        return A;
    }

    // generación de distribución normal (Gaussiana)
    vector<int> generarNormal(size_t n, double media, double desviacion_estandar) {
        vector<int> A(n);
        
        random_device rd;
        mt19937 gen(rd());
        normal_distribution<double> dist(media, desviacion_estandar);

        for (size_t i = 0; i < n; ++i) {
            A[i] = static_cast<int>(dist(gen));
        }
        
        // La distribución normal no genera datos ordenados, por lo que es obligatorio ordenar
        sort(A.begin(), A.end());
        return A;
    }

    int busquedaBinaria(const vector<int>& A, int x) {
        int n = A.size();
        int l,m, r;
        l = 0;
        r = n - 1;
        m = r/2;
        while (l <= r) {
            if (A[m] == x) 
                return m;
            else { 
                if (x < A[m]) 
                    r = m - 1;
                else 
                    l = m + 1;
            }
            m = (l + r) / 2;
        }
        return -1;
    }

    int calcularEspacio(const vector<int>& A) { //retorna tamaño de bytes que ocupa vector
        return A.capacity() * sizeof(int); // celdas ocupadas por bytes un int 
    }
}
