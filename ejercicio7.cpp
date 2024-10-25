#include <iostream>
#include <string>
#include <cassert>
#include <limits>
#include <algorithm>

using namespace std;

// Función para verificar si es posible dividir las canciones con un tiempo máximo dado
bool esPosible(int canciones[], int N, int estudiantes, int tiempoMaximo) {
    int sumaActual = 0;
    int estudiantesUsados = 1;
    for (int i = 0; i < N; i++) {
        if (sumaActual + canciones[i] > tiempoMaximo) {
            estudiantesUsados++;
            sumaActual = canciones[i];
            if (estudiantesUsados > estudiantes) {
                return false;
            }
        } else {
            sumaActual += canciones[i];
        }
    }
    return true;
}

int main() {
    int N, E;
    cin >> N;
    int canciones[N];
    for (int i = 0; i < N; i++) {
        cin >> canciones[i];
    }
    cin >> E;

    // Búsqueda binaria sobre el tiempo máximo mínimo
    int izq = canciones[0], der = 0;
    for (int i = 0; i < N; i++) {
        izq = max(izq, canciones[i]);
        der += canciones[i];
    }
    
    int tiempoMinimoMax = der;

    while (izq <= der) {
        int mid = izq + (der - izq) / 2;
        if (esPosible(canciones, N, E, mid)) {
            tiempoMinimoMax = mid;
            der = mid - 1;
        } else {
            izq = mid + 1;
        }
    }

    // Generar la distribución óptima usando tiempoMinimoMax
    int sumaActual = 0;
    int estudiantesUsados = 1;
    int inicio = 0;

    for (int i = 0; i < N; i++) {
        if (sumaActual + canciones[i] > tiempoMinimoMax) {
            cout << sumaActual << " " << inicio + 1 << " " << i << endl;
            estudiantesUsados++;
            sumaActual = canciones[i];
            inicio = i;
        } else {
            sumaActual += canciones[i];
        }
    }

    cout << sumaActual << " " << inicio + 1 << " " << N << endl;
    cout << tiempoMinimoMax << endl;

    return 0;
}
