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
    struct Resultados {
        double buildTime;
        double searchTime;
        size_t totalBytes;
        int pos;
    };

    void set_bit(uint8_t& byte, int pos) {
        byte |= (1 << pos);
    }
    void clear_bit(uint8_t& byte, int pos) {
        byte &= ~(1 << pos);
    }

    //Construye un vector con las posiciones de los bits que corresponden a los samples
    vector<size_t> build_sample_bits(vector<uint8_t>& bitMap, size_t totalBits,unordered_map<string,int>& revTable, int b){

        vector<size_t> sampleBits;
        string cur;
        size_t gapCount = 0;

        for (size_t i = 0; i < totalBits; i++) {
            int bit = (bitMap[i / 8] >> (7 - i % 8)) & 1;
            cur += ('0' + bit);
            if (revTable.count(cur)) {
                gapCount++;
                cur.clear();
                if (gapCount % b == 0){
                    sampleBits.push_back(i + 1); 
                }
            }
        }
        return sampleBits;
    }

    int binary_search(vector<int>& sample, int value) {
        int l = 0, h = sample.size() - 1;
        while (l < h) {
            int mid = (l + h + 1) / 2;
            if (sample[mid] <= value){
                l = mid;
            }
            else{
                h = mid - 1; 
            } 
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

            int lsum = 0;
            int rsum = 0;

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
    int find_value(int value, size_t totalBits, vector<uint8_t>& bitMap, vector<size_t>& sampleBits, vector<int>& sample, unordered_map<string,int>& revTable, int l, int b){
        size_t startBit;
        int sum;
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
            if (revTable.count(temp)) {
                sum += revTable[temp];
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
    Resultados caso3(Caso2::GC& data, int value) {
        Resultados res;
        //Construcion de la estructura y los elemenetos necesarios para la busqueda
        auto startBuild = chrono::high_resolution_clock::now();

        unordered_map<int, string> Table;
        shannon_fano(data.GC, Table);
        unordered_map<string,int> revTable;
        for (auto& [key, val] : Table) revTable[val] = key;

        size_t totalBits = 0;
        vector<uint8_t> bitMap = encode(data.GC, Table, totalBits);
        vector<size_t> sampleBits = build_sample_bits(bitMap, totalBits, revTable, data.b);

        auto endBuild = chrono::high_resolution_clock::now();

        //Inicio de la busqueda
        auto startSearch = chrono::high_resolution_clock::now();

        int l = binary_search(data.sample, value);
        int pos = find_value(value, totalBits, bitMap, sampleBits, data.sample, revTable, l, data.b);
        
        auto endSearch = chrono::high_resolution_clock::now();

        //Resultados
        res.buildTime = chrono::duration<double, milli>(endBuild - startBuild).count();
        res.searchTime = chrono::duration<double, milli>(endSearch - startSearch).count();
        res.totalBytes = bitMap.size() + (sampleBits.size() * sizeof(size_t)) + (data.sample.size() * sizeof(int));
        res.pos = pos;

        return res;
    }
}