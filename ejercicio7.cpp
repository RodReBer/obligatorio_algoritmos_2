//Rodrigo Rey y Román Ferrero
//Estructura de datos y algoritmos 2
#include <iostream>
#include <string>
#include <cassert>
#include <limits>
#include "funciones/ejercicio7.cpp"

using namespace std;


int main() {
    int N; // Número de canciones
    cin >> N;
    assert(N > 0 && N <= 1000000); // Validación de N

    int canciones[N];
    for (int i = 0; i < N; i++) {
        cin >> canciones[i];
        assert(canciones[i] >= 1 && canciones[i] <= 1000000000); // Validación de la duración de las canciones
    }

    int E; // Número de estudiantes
    cin >> E;
    assert(E > 0 && E <= 100000); // Validación de E

    // Búsqueda binaria para encontrar el tiempo máximo mínimo
    int low = 0, high = 0;
    for (int i = 0; i < N; i++) {
        if (canciones[i] > low) low = canciones[i]; // Canción más larga
        high += canciones[i]; // Suma total de todas las canciones
    }

    int resultado = high; // Tiempo máximo mínimo inicializado al peor caso
    while (low <= high) {
        int mid = (low + high) / 2;

        if (esValido(canciones, N, E, mid)) {
            resultado = mid; // Actualizamos el mejor resultado
            high = mid - 1;  // Buscamos un tiempo menor
        } else {
            low = mid + 1;   // Aumentamos el tiempo
        }
    }

    // Imprimir la distribución óptima
    int tiempoActual = 0, inicio = 1;
    int estudiantesUsados = 1;

    for (int i = 0; i < N; i++) {
        if (tiempoActual + canciones[i] > resultado) {
            cout << tiempoActual << " " << inicio << " " << i << endl;
            estudiantesUsados++;
            tiempoActual = canciones[i];
            inicio = i + 1;
        } else {
            tiempoActual += canciones[i];
        }
    }
    // Imprimir el último estudiante
    cout << tiempoActual << " " << inicio << " " << N << endl;
    cout << resultado << endl;

    return 0;
}
