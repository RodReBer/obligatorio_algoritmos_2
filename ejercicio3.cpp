//Rodrigo Rey y Román Ferrero
//Estructura de datos y algoritmos 2
#include <cassert>
#include <string>
#include <iostream>
#include <limits>
#include "tads/heap.cpp"

using namespace std;

int main() {
    int N, K;
    cin >> N;


    const int MAX_ID = 999999;  // límite máximo para los ids
    int precioPorId[MAX_ID];   
    for (int i = 0; i < MAX_ID; i++) {
        precioPorId[i] = -1; 
    }

    for (int i = 0; i < N; i++) {
        int id, precio;
        cin >> id >> precio;

        if (precioPorId[id] == -1 || precio < precioPorId[id]) {
            precioPorId[id] = precio;
        }
    }

    cin >> K;

    Heap<int> heap(N);

    for (int id = 0; id < MAX_ID; id++) {
        if (precioPorId[id] != -1) {  // los ids válidos
            heap.Insertar(precioPorId[id], id);
        }
    }

    // K objetos más baratos
    for (int i = 0; i < K; i++) {
        if (!heap.EsVacia()) {
            int id = heap.ObtenerMin();  
            cout << id << endl;          
            heap.BorrarMin();          
        }
    }

    return 0;
}