#include <iostream>
#include <string>
#define INF 1000000000

using namespace std;

const int MAX_N = 10;
string jardin[MAX_N][MAX_N];
int maxFlores = -INF;

// Es valido colocar una flor en la posición (x, y) con color 'color'?
bool esValido(int x, int y, int N, const string &color)
{
    // Posiciones adyacentes, incluyendo diagonales (8 posiciones)
    for (int dx = -1; dx <= 1; dx++)
    {
        for (int dy = -1; dy <= 1; dy++)
        {
            if (dx == 0 && dy == 0)
                continue; // Ignoramos la posición en la que estamos parados inicialmente
            int nuevaFila = x + dx, nuevaColumna = y + dy;
            // Verificar si la posición adyacente es válida y tiene el mismo color
            if (nuevaFila >= 0 && nuevaFila < N && nuevaColumna >= 0 && nuevaColumna < N && jardin[nuevaFila][nuevaColumna] == color)
            {
                return false;
            }
        }
    }
    return true;
}

// Función de backtracking
void plantarFlores(int x, int y, int N, int F, string colores[], int restricciones[], int floresPlantadas)
{
    if (x == N)
    { // final del jardin 🌹
        if (floresPlantadas > maxFlores)
        {
            maxFlores = floresPlantadas; // se actualiza el max
        }
        return;
    }

    // Poda: Si en algún momento el número de flores restantes por colocar no puede superar el máximo actual, no tiene sentido seguir buscando

    // Si floresPlantadas + posiblesRestantes ≤ maxFlores, podemos cortar la recursión.
    if (floresPlantadas + (N * N - x * N - y) <= maxFlores)
    {
        return;
    }

    // pos sig
    // Si estamos en la última columna, avanzamos a la siguiente fila
    // Si no, avanzamos a la siguiente columna
    // joyita
    int nuevaFila = (y == N - 1) ? x + 1 : x;
    int nuevaColumna = (y == N - 1) ? 0 : y + 1;

    // Intentar dejar esta celda vacía
    plantarFlores(nuevaFila, nuevaColumna, N, F, colores, restricciones, floresPlantadas);

    // Intentar colocar cada color
    for (int i = 0; i < F; i++)
    {
        if (restricciones[i] == -1 || restricciones[i] == x)
        { // verificamos restricciones
            if (esValido(x, y, N, colores[i]))
            {                              // chequeamos
                jardin[x][y] = colores[i]; // la colocamos
                plantarFlores(nuevaFila, nuevaColumna, N, F, colores, restricciones, floresPlantadas + 1);
                jardin[x][y] = ""; // retrocedemos
            }
        }
    }
}

// Función principal
int main()
{
    int N, F;
    cin >> N >> F;

    string colores[F];
    int restricciones[F];

    // leemos los colores y sus resetricciones
    for (int i = 0; i < F; i++)
    {
        cin >> colores[i] >> restricciones[i];
    }

    // JARDIN PELADO
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            jardin[i][j] = "";
        }
    }

    // BT
    plantarFlores(0, 0, N, F, colores, restricciones, 0);

    // resultado
    cout << maxFlores << endl;

    return 0;
}
