#include <iostream>
#include <cmath>
#include <cstring>
#include "funciones/ejercicio8.cpp"

using namespace std;

int main() {
    int n;
    cin >> n;

    // Usamos memoria dinámica para manejar grandes entradas porque sino nos tira segmentation fault
    Ciudad* ciudades = new Ciudad[n];

    for (int i = 0; i < n; i++) {
        cin >> ciudades[i].x >> ciudades[i].y >> ciudades[i].p;
    }

    // Ordenar las ciudades por coordenadas (x, y) usando quicksort
    quicksort(ciudades, 0, n - 1);

    // Buscar el par más cercano
    double mejorDistancia = 1e18;
    Ciudad mejor1, mejor2;

    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            // Si la diferencia en x ya es demasiado grande, breakkkkk
            double dx = ciudades[j].x - ciudades[i].x;
            if (dx * dx >= mejorDistancia) break;

            // Calcular la distancia efectiva
            double dist = distanciaEfectiva(
                ciudades[i].x, ciudades[i].y, ciudades[i].p,
                ciudades[j].x, ciudades[j].y, ciudades[j].p
            );

            // si encontramos una mejor distancia, actualizamoss
            if (dist < mejorDistancia) {
                mejorDistancia = dist;
                mejor1 = ciudades[i];
                mejor2 = ciudades[j];
            } else if (dist == mejorDistancia) {
                // si la distancia es igual, elegimos la que tenga mayor población
                if (ciudades[i].p + ciudades[j].p > mejor1.p + mejor2.p) {
                    mejor1 = ciudades[i];
                    mejor2 = ciudades[j];
                }
            }
        }
    }

    // Imprimir en el orden que se pide
    if (mejor1.x > mejor2.x || (mejor1.x == mejor2.x && mejor1.y > mejor2.y)) {
        Ciudad temp = mejor1;
        mejor1 = mejor2;
        mejor2 = temp;
    }

    cout << mejor1.x << " " << mejor1.y << " " << mejor1.p << endl;
    cout << mejor2.x << " " << mejor2.y << " " << mejor2.p << endl;

    // Liberar memoria pq sino pinch
    delete[] ciudades;

    return 0;
}
