// Rodrigo Rey y Román Ferrero
// Estructura de datos y algoritmos 2
#include <iostream>
using namespace std;

// Estructura para representar un objeto
struct Objeto {
    int id;
    int precio;
};

// Función para intercambiar dos objetos
void intercambiar(Objeto &a, Objeto &b) {
    Objeto temp = a;
    a = b;
    b = temp;
}

// Implementación de Quicksort
void quicksort(Objeto arr[], int low, int high) {
    if (low < high) {
        int pivot = arr[high].precio;  // Elegimos el último elemento como pivote
        int i = low - 1;
        for (int j = low; j < high; ++j) {
            // Comparar por precio y, en caso de empate, por id descendente
            if (arr[j].precio < pivot || 
                (arr[j].precio == pivot && arr[j].id > arr[high].id)) {
                ++i;
                intercambiar(arr[i], arr[j]);
            }
        }
        intercambiar(arr[i + 1], arr[high]);
        int pi = i + 1;

        quicksort(arr, low, pi - 1);
        quicksort(arr, pi + 1, high);
    }
}

int main() {
    int N;
    cin >> N;

    // Leer todos los objetos usando memoria dinámica
    Objeto* objetos = new Objeto[N];
    for (int i = 0; i < N; ++i) {
        cin >> objetos[i].id >> objetos[i].precio;
    }

    int K;
    cin >> K;

    // Encontrar el máximo id
    int max_id = 0;
    for (int i = 0; i < N; ++i) {
        if (objetos[i].id > max_id) {
            max_id = objetos[i].id;
        }
    }

    // Arreglo para almacenar el menor precio por id
    int* precios_min = new int[max_id + 1];
    // Inicializar manualmente con -1
    for (int i = 0; i <= max_id; ++i) {
        precios_min[i] = -1;
    }

    // Registrar el menor precio para cada id
    for (int i = 0; i < N; ++i) {
        int id = objetos[i].id;
        int precio = objetos[i].precio;

        if (precios_min[id] == -1 || precio < precios_min[id]) {
            precios_min[id] = precio;
        } else if (precio == precios_min[id] && objetos[i].id > id) {
            precios_min[id] = precio;  // Mantener id más grande en caso de empate
        }
    }

    // Crear un nuevo arreglo con objetos únicos
    Objeto* unicos = new Objeto[max_id]; // Max posibles objetos únicos
    int count = 0;
    for (int i = 1; i <= max_id; ++i) {
        if (precios_min[i] != -1) {
            unicos[count].id = i;
            unicos[count].precio = precios_min[i];
            ++count;
        }
    }

    // Liberar memoria no utilizada
    delete[] precios_min;
    delete[] objetos;

    // Ordenar los objetos únicos por precio ascendente y luego por id descendente
    quicksort(unicos, 0, count - 1);

    // Mostrar los K más baratos
    for (int i = 0; i < K; ++i) {
        cout << unicos[i].id << endl;
    }

    // Liberar memoria dinámica
    delete[] unicos;

    return 0;
}
