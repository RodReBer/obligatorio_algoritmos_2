#include <cassert>
#include <string>
#include <iostream>
#include <limits>

using namespace std;

struct Ciudad {
    int x, y, p; 
};

double distanciaEfectiva(const Ciudad& c1, const Ciudad& c2) {
    double distanciaCuadrada = (c2.x - c1.x) * (c2.x - c1.x) + (c2.y - c1.y) * (c2.y - c1.y);
    double diferenciaPoblacion = abs(c2.p - c1.p);
    double maxPoblacion = max(c2.p, c1.p);
    return distanciaCuadrada + (diferenciaPoblacion / maxPoblacion);
}

void intercambiar(Ciudad& c1, Ciudad& c2) {
    Ciudad temp = c1;
    c1 = c2;
    c2 = temp;
}

//ordenar por coordenadas x (y luego por y en caso de empate).
void quicksort(Ciudad ciudades[], int inicio, int fin) {
    if (inicio >= fin) return;

    int pivote = inicio;
    int izq = inicio + 1;
    int der = fin;

    while (izq <= der) {
        while (izq <= fin && (ciudades[izq].x < ciudades[pivote].x || 
              (ciudades[izq].x == ciudades[pivote].x && ciudades[izq].y < ciudades[pivote].y))) {
            izq++;
        }
        while (der > inicio && (ciudades[der].x > ciudades[pivote].x || 
              (ciudades[der].x == ciudades[pivote].x && ciudades[der].y > ciudades[pivote].y))) {
            der--;
        }
        if (izq < der) {
            intercambiar(ciudades[izq], ciudades[der]);
        }
    }
    intercambiar(ciudades[pivote], ciudades[der]);

    quicksort(ciudades, inicio, der - 1);
    quicksort(ciudades, der + 1, fin);
}

pair<Ciudad, Ciudad> encontrarMejorPar(Ciudad ciudades[], int n) {
    pair<Ciudad, Ciudad> mejorPar;
    mejorPar.first = ciudades[0];
    mejorPar.second = ciudades[1];

    double mejorDistancia = distanciaEfectiva(ciudades[0], ciudades[1]);

    for (int i = 0; i < n; ++i) {
        for (int j = i + 1; j < n; ++j) {
            double dist = distanciaEfectiva(ciudades[i], ciudades[j]);
            if (dist < mejorDistancia) {
                mejorDistancia = dist;
                mejorPar.first = ciudades[i];
                mejorPar.second = ciudades[j];
            } else if (dist == mejorDistancia) {
                // Regla de desempate: mayor suma de poblaciones.
                int sumaPoblacionActual = ciudades[i].p + ciudades[j].p;
                int sumaPoblacionMejor = mejorPar.first.p + mejorPar.second.p;
                if (sumaPoblacionActual > sumaPoblacionMejor) {
                    mejorPar.first = ciudades[i];
                    mejorPar.second = ciudades[j];
                }
            }
        }
    }

    return mejorPar;
}

int main() {
    int n;
    cin >> n;

    Ciudad ciudades[n];

    for (int i = 0; i < n; ++i) {
        cin >> ciudades[i].x >> ciudades[i].y >> ciudades[i].p;
    }

    quicksort(ciudades, 0, n - 1);

    pair<Ciudad, Ciudad> mejorPar = encontrarMejorPar(ciudades, n);

    Ciudad c1 = mejorPar.first;
    Ciudad c2 = mejorPar.second;

    if (c1.x < c2.x || (c1.x == c2.x && c1.y < c2.y)) {
        cout << c1.x << " " << c1.y << " " << c1.p << endl;
        cout << c2.x << " " << c2.y << " " << c2.p << endl;
    } else {
        cout << c2.x << " " << c2.y << " " << c2.p << endl;
        cout << c1.x << " " << c1.y << " " << c1.p << endl;
    }

    return 0;
}