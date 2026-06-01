#pragma once
#include <vector>
#include <iostream>

using namespace std;

namespace Caso2 {

    struct GC {
        vector<int> GC;       // Arreglo de gaps
        vector<int> sample;   // Valores originales
        size_t b;                  // distancia entre muestras
        size_t n;                  // Tamaño del arreglo original
    };

    // 1. Construcción de la estructura
    GC construir(const vector<int>& A, size_t b) {
        GC esc;
        esc.b = b;
        esc.n = A.size();
        
        if (A.empty()) return esc;

        esc.GC.resize(esc.n);
        esc.GC[0] = A[0]; 
        for (size_t i = 1; i < esc.n; ++i) {
            esc.GC[i] = A[i] - A[i - 1];
        }

        for (size_t i = 0; i < esc.n; i += b) {
            esc.sample.push_back(A[i]);
        }

        return esc;
    }

    //Busqueda
    int buscar(const GC& esc, int x) {
        if (esc.n == 0) return -1;
        int l_sample = 0;
        int r_sample = esc.sample.size() - 1;
        int m_sample;
        int bloque_encontrado = -1;

        while (l_sample <= r_sample) {
            m_sample = (l_sample + r_sample) / 2;
            
            if (esc.sample[m_sample] == x) {
                return m_sample * esc.b;
            }
            
            if (x < esc.sample[m_sample]) {
                r_sample = m_sample - 1;
            } else {
                bloque_encontrado = m_sample;
                l_sample = m_sample + 1;
            }
        }
        if (bloque_encontrado == -1) return -1;
        size_t inicio_original = bloque_encontrado * esc.b; 
        size_t fin_original = min(inicio_original + esc.b, esc.n);

        int valor_acumulado = esc.sample[bloque_encontrado];

        if (valor_acumulado == x) return inicio_original;

        for (size_t i = inicio_original + 1; i < fin_original; ++i) {
            valor_acumulado += esc.GC[i]; 
            
            if (valor_acumulado == x) {
                return i; 
            }
            if (valor_acumulado > x) {
                return -1; 
            }
        }

        return -1;
    }


    size_t calcularEspacio(const GC& esc) {
        size_t espacio_GC = esc.GC.capacity() * sizeof(int);
        size_t espacio_sample = esc.sample.capacity() * sizeof(int);
        return espacio_GC + espacio_sample + sizeof(esc.b) + sizeof(esc.n);
    }

}