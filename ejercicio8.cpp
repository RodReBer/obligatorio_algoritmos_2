#include <iostream>
#include <cmath>
#include <cstring>

using namespace std;

struct Ciudad {
    int x, y, p; // Coordenadas (x, y) y población (p)
};

// Función para calcular la distancia efectiva
double distanciaEfectiva(int x1, int y1, int p1, int x2, int y2, int p2) {
    double distanciaCuadrada = (x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1);
    double diferenciaPoblacion = abs(p2 - p1);
    double maxPoblacion = (p1 > p2) ? p1 : p2;
    return distanciaCuadrada + (diferenciaPoblacion / maxPoblacion);
}

// Implementación de quicksort
void quicksort(Ciudad* ciudades, int inicio, int fin) {
    if (inicio >= fin) return;

    Ciudad pivote = ciudades[inicio];
    int i = inicio + 1, j = fin;

    while (i <= j) {
        while (i <= j && (ciudades[i].x < pivote.x || (ciudades[i].x == pivote.x && ciudades[i].y < pivote.y))) {
            i++;
        }
        while (i <= j && (ciudades[j].x > pivote.x || (ciudades[j].x == pivote.x && ciudades[j].y > pivote.y))) {
            j--;
        }
        if (i <= j) {
            Ciudad temp = ciudades[i];
            ciudades[i] = ciudades[j];
            ciudades[j] = temp;
            i++;
            j--;
        }
    }
    ciudades[inicio] = ciudades[j];
    ciudades[j] = pivote;

    quicksort(ciudades, inicio, j - 1);
    quicksort(ciudades, j + 1, fin);
}

int main() {
    int n;
    cin >> n;

    // Usamos memoria dinámica para manejar grandes entradas
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
            // Si la diferencia en x ya es demasiado grande, romper
            double dx = ciudades[j].x - ciudades[i].x;
            if (dx * dx >= mejorDistancia) break;

            // Calcular la distancia efectiva
            double dist = distanciaEfectiva(
                ciudades[i].x, ciudades[i].y, ciudades[i].p,
                ciudades[j].x, ciudades[j].y, ciudades[j].p
            );

            // Actualizar si encontramos una mejor distancia
            if (dist < mejorDistancia) {
                mejorDistancia = dist;
                mejor1 = ciudades[i];
                mejor2 = ciudades[j];
            } else if (dist == mejorDistancia) {
                // Desempate: mayor suma de poblaciones
                if (ciudades[i].p + ciudades[j].p > mejor1.p + mejor2.p) {
                    mejor1 = ciudades[i];
                    mejor2 = ciudades[j];
                }
            }
        }
    }

    // Imprimir en el orden requerido
    if (mejor1.x > mejor2.x || (mejor1.x == mejor2.x && mejor1.y > mejor2.y)) {
        Ciudad temp = mejor1;
        mejor1 = mejor2;
        mejor2 = temp;
    }

    cout << mejor1.x << " " << mejor1.y << " " << mejor1.p << endl;
    cout << mejor2.x << " " << mejor2.y << " " << mejor2.p << endl;

    // Liberar memoria
    delete[] ciudades;

    return 0;
}
