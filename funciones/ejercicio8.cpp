
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
