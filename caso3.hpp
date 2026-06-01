#pragma once
#include <iostream>
#include <unordered_map>
#include <vector>
#include <stack>
#include <algorithm>
#include <chrono>
#include "caso2.hpp"

/*
**DESCRIPCION**
Shannon-Fano es un algoritmo de compresion de datos lossless(sin perdida),
que asigna a cada simbolo un codigo, con los simbolos mas frecuentes con codigos mas cortos.
Creado por Claude Shannon y Robert Fano en 1949, usa top-down para ir dividiendo los simbolos en grupos,
asignando 0 a un grupo y 1 al otro, hasta que cada simbolo tenga un codigo unico.

**ANALISIS TEORICO**
- Si cada simbolo tiene la misma frecuencia, ocupamos la entropia en el peor caso para largo fijo,
cada simbolo requeriria de log_2(n) bits, y el espacio requerido en este caso seria n*log_2(n) bits. N es el numero de simbolos distintos.

- Si cada simbolo tiene una frecuencia diferente, ocupamos la entropia de largo variable,
cada simbolo requeriria de log_2(1/freq(i)) bits siendo i cada simbolo,
el espacio requerido seria la entropia por la cantidad de simbolos, siendo la entropia la sumatoria de log_2(1/freq(i)) para cada simbolo.
*/

using namespace std;

namespace Caso3 {
    void set_bit(uint8_t& byte, int pos) {
        byte |= (1 << pos);
    }
    void clear_bit(uint8_t& byte, int pos) {
        byte &= ~(1 << pos);
    }

    //Construye un vector con las posiciones de los bits que corresponden a los samples
    vector<size_t> build_sample_bits(vector<uint8_t>& bitMap, size_t totalBits,unordered_map<int,string>& encodeTable, int b){
        unordered_map<string,int> decodeTable;
        for (auto& [key, val] : encodeTable) decodeTable[val] = key;

        vector<size_t> sampleBits;
        string cur;
        size_t gapCount = 0;

        for (size_t i = 0; i < totalBits; i++) {
            int bit = (bitMap[i / 8] >> (7 - i % 8)) & 1;
            cur += ('0' + bit);
            if (decodeTable.count(cur)) {
                gapCount++;
                cur.clear();
                if (gapCount % b == 0){
                    sampleBits.push_back(i + 1); 
                }
            }
        }
        return sampleBits;
    }

    int binary_search(vector<size_t>& sample, size_t value) {
        int l = 0, h = sample.size() - 1;
        while (l < h) {
            int mid = (l + h + 1) / 2;
            if (sample[mid] <= value) l = mid;
            else                      h = mid - 1;
        }
        return l;
    }


    //Construye una tabla con los gaps y sus codigos correspondientes
    void shannon_fano(vector<int>& gaps, unordered_map<int,string>& table){
        unordered_map<int, int> frequency;
        frequency.reserve(gaps.size()); 
        for (int gap : gaps) {
            frequency[gap]++;
        }

        vector<pair<int,int> > symbols(frequency.begin(), frequency.end());
        sort(symbols.begin(), symbols.end(), [](const auto& a, const auto& b) {
            return a.second > b.second;
        });


        stack<pair<size_t, size_t> > s;
        pair<size_t, size_t> lr = {0, symbols.size() - 1};
        s.push(lr);
        while (!s.empty()) {
            auto [l, r] = s.top();
            size_t origL = l;
            size_t origR = r;
            s.pop();

            unsigned int lsum = 0;
            unsigned int rsum = 0;

            while (l !=r-1) {
                if (lsum < rsum) {
                    lsum += symbols[l].second;
                    l++;
                } else {
                    rsum += symbols[r].second;
                    r--;
                }
            }

            for (size_t i = origL; i <= origR; i++) {
                if (i <= l) {
                    table[symbols[i].first] += "0";
                } else {
                    table[symbols[i].first] += "1";
                }
            }
            if (l - origL > 0) {
                s.push({origL, l});
            }
            if (origR - r > 0) {
                s.push({r, origR});
            }
        }

    }

    //Coloca los codigos de los gaps en un vector, ocupando el menor espacio posible
    vector<uint8_t> encode(vector<int>& gaps,  unordered_map<int,string>& table, size_t& totalBits){
        vector<uint8_t> bytes;
        totalBits = 0;

        for (int gap : gaps) {
            for (char c : table.at(gap)) {
                if (totalBits % 8 == 0) {
                    bytes.push_back(0);
                }
                int pos = 7 - totalBits % 8;
                if (c == '1'){
                    set_bit(bytes[totalBits / 8], pos);
                }else {
                    clear_bit(bytes[totalBits / 8], pos);
                }
                totalBits++;
            }
        }
        return bytes;
    }

    //Busca el valor en el vector de bits y devuelve la posicion, o -1 si no se encuentra
    int find_value(size_t value, size_t totalBits, vector<uint8_t>& bitMap, vector<size_t>& sampleBits, vector<size_t>& sample, unordered_map<int,string>& encodeTable, int l, int b){
        unordered_map<string,int> decodeTable;
        for (auto& [key, val] : encodeTable) decodeTable[val] = key;


        size_t startBit;
        size_t sum;
        if (l > 0) {
            startBit = sampleBits[l - 1];
            sum = sample[l - 1];
        }else {
            startBit = 0;
            sum = 0;
        }
        size_t pos = l * b;
        string temp = "";

        for (size_t i = startBit; i < totalBits; i++) {
            int bit = (bitMap[i / 8] >> (7 - i % 8)) & 1;
            temp += ('0' + bit);
            if (decodeTable.count(temp)) {
                sum += decodeTable[temp];
                temp.clear();

                if (sum == value){
                    return pos;
                }
                if (sum >  value){
                    return -1;
                }
                pos++;
            }

        }
        return -1;
    }


    //Recibe los datos de el resultado en el caso 2, para comparar utilizando shannon-Fanno
    void caso3(Caso2::GC& data, int value) {
        auto start = chrono::high_resolution_clock::now();

        unordered_map<int, string> encodeTable;
        shannon_fano(data.GC, encodeTable);

        size_t totalBits = 0;
        vector<uint8_t> bitMap = encode(data.GC, encodeTable, totalBits);
        vector<size_t> sample(data.sample.begin(), data.sample.end());
        vector<size_t> sampleBits = build_sample_bits(bitMap, totalBits, encodeTable, data.b);

        int l = binary_search(sample, value);
        int pos = find_value(value, totalBits, bitMap, sampleBits, sample, encodeTable, l, data.b);
        
        auto end = chrono::high_resolution_clock::now();
        double ms = chrono::duration<double, milli>(end - start).count();

        cout << "Valor: " << value  << endl;
        if (pos != -1) {
            cout << "Encontrado en posicion: [" << pos << "]" << endl;
        } else {
            cout << "Valor no encontrado." << endl;
        }
        cout << "Bits usados: " << totalBits << " ()" << bitMap.size() << " bytes" << endl;
        cout << "Tiempo: " << ms << " ms" << endl;
    }
}