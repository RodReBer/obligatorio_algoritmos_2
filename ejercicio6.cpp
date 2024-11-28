// Rodrigo Rey y Román Ferrero
// Estructura de datos y algoritmos 2
#include <iostream>
#include <climits>
#include <cstring>
#include "tads/Lista/lista.cpp"
#define MAX_CIUDADES 300

using namespace std;

struct conexion
{
    int destino;
    int costo;
};

struct Grafo
{
    int matriz[MAX_CIUDADES][MAX_CIUDADES];
    string nombres[MAX_CIUDADES];
    int numCiudades;


    void inicializar(int ciudades)
    {
        numCiudades = ciudades;
        for (int i = 0; i < numCiudades; ++i)
        {
            for (int j = 0; j < numCiudades; ++j)
            {
                matriz[i][j] = (i == j) ? 0 : INT_MAX;
            }
        }
    }

    void agregarConexion(int origen, int destino, int costo)
    {
        matriz[origen][destino] = costo;
        matriz[destino][origen] = costo;
    }

    void asignarNombreCiudad(int indice, string nombre)
    {
        nombres[indice] = nombre;
    }

    void dijkstra(int inicio, int distancia[], int anterior[], Grafo *grafo)
    {
        bool visitado[MAX_CIUDADES];
        for (int i = 0; i < MAX_CIUDADES; i++) // Inicializar visitado en false
        {
            visitado[i] = false;
        }

        for (int i = 0; i < grafo->numCiudades; i++)
        {
            distancia[i] = INT_MAX;
            anterior[i] = -1;
        }
        distancia[inicio] = 0;

        for (int i = 0; i < grafo->numCiudades; i++)
        {
            int minDistancia = INT_MAX, u = -1;
            for (int j = 0; j < grafo->numCiudades; j++)
            {
                if (!visitado[j] && distancia[j] < minDistancia)
                {
                    minDistancia = distancia[j];
                    u = j;
                }
            }

            if (u == -1)
                break;
            visitado[u] = true;

            for (int v = 0; v < grafo->numCiudades; v++)
            {
                if (!visitado[v] && grafo->matriz[u][v] != INT_MAX && distancia[u] + grafo->matriz[u][v] < distancia[v])
                {
                    distancia[v] = distancia[u] + grafo->matriz[u][v];
                    anterior[v] = u;
                }
            }
        }
    }

    void imprimirCamino(int anterior[], int destino, Grafo *grafo)
    {
        if (anterior[destino] == -1)
        {
            cout << grafo->nombres[destino];
            return;
        }
        imprimirCamino(anterior, anterior[destino], grafo);
        cout << " -> " << grafo->nombres[destino];
    }

    // Duplicar costos solo en la primera opción
    void duplicarCostos(Grafo *grafo, int anterior[], int destino)
    {
        while (anterior[destino] != -1)
        {
            int origen = anterior[destino];
            grafo->matriz[origen][destino] *= 2;
            grafo->matriz[destino][origen] *= 2;
            destino = origen;
        }
    }

    // Restaurar los costos a su valor original
    void restaurarCostos(Grafo *grafo, int anterior[], int destino)
    {
        while (anterior[destino] != -1)
        {
            int origen = anterior[destino];
            grafo->matriz[origen][destino] /= 2;
            grafo->matriz[destino][origen] /= 2;
            destino = origen;
        }
    }
};

int main()
{
    Grafo *grafo = new Grafo();
    int numCiudades, start, entidad, equipo, extraccion, conexiones;
    cin >> numCiudades;
    grafo->inicializar(numCiudades);

    for (int i = 0; i < numCiudades; ++i)
    {
        int idCiudad;
        string nombreCiudad;
        cin >> idCiudad >> nombreCiudad;
        grafo->asignarNombreCiudad(idCiudad - 1, nombreCiudad);
    }

    cin >> start >> entidad >> equipo >> extraccion >> conexiones;

    for (int i = 0; i < conexiones; i++)
    {
        int origen, destino, costo;
        cin >> origen >> destino >> costo;
        grafo->agregarConexion(origen - 1, destino - 1, costo);
    }

    int dist[6][MAX_CIUDADES], ant[6][MAX_CIUDADES];

    grafo->dijkstra(start - 1, dist[0], ant[0], grafo);
    int costoA1 = dist[0][entidad - 1];
    grafo->duplicarCostos(grafo, ant[0], entidad - 1);

    grafo->dijkstra(entidad - 1, dist[1], ant[1], grafo);
    int costoA2 = costoA1 + dist[1][equipo - 1];
    grafo->duplicarCostos(grafo, ant[1], equipo - 1);

    grafo->dijkstra(equipo - 1, dist[2], ant[2], grafo);
    int costoA3 = costoA2 + dist[2][extraccion - 1];
    int costoTotalA = costoA3;

    // Restaurar costos
    grafo->restaurarCostos(grafo, ant[0], entidad - 1);
    grafo->restaurarCostos(grafo, ant[1], equipo - 1);

    // Calcular la segunda opción de ruta y costos totales
    grafo->dijkstra(start - 1, dist[3], ant[3], grafo);
    int costoB1 = dist[3][equipo - 1];
    grafo->duplicarCostos(grafo, ant[3], equipo - 1);

    grafo->dijkstra(equipo - 1, dist[4], ant[4], grafo);
    int costoB2 = costoB1 + dist[4][entidad - 1];
    grafo->duplicarCostos(grafo, ant[4], entidad - 1);

    grafo->dijkstra(entidad - 1, dist[5], ant[5], grafo);
    int costoB3 = costoB2 + dist[5][extraccion - 1];
    int costoTotalB = costoB3;

    // Restaurar costos

    // Comparar y decidir la mejor opción
    // Decisión sobre la mejor ruta
    (costoTotalA <= costoTotalB) ? (
        cout << "BE11, la mejor ruta es Desactivar la Entidad, buscar equipo y punto de extraccion con un costo de " << costoTotalA << endl,
        cout << "La otra opcion tiene un costo de " << costoTotalB << endl,
        cout << "Paso 1: ", grafo->imprimirCamino(ant[0], entidad - 1, grafo),
        cout << " -> Desactivar la Entidad" << endl,
        cout << "Paso 2: ", grafo->imprimirCamino(ant[1], equipo - 1, grafo),
        cout << " -> Buscar equipo" << endl,
        cout << "Paso 3: ", grafo->imprimirCamino(ant[2], extraccion - 1, grafo),
        cout << " -> Ir a Punto de extraccion" << endl)
                                 :
                                 // Si la segunda opción es mejor
        (cout << "BE11, la mejor ruta es Buscar equipo, Desactivar la Entidad y punto de extraccion con un costo de " << costoTotalB << endl,
         cout << "La otra opcion tiene un costo de " << costoTotalA << endl,
         cout << "Paso 1: ", grafo->imprimirCamino(ant[3], equipo - 1, grafo),
         cout << " -> Buscar equipo" << endl,
         cout << "Paso 2: ", grafo->imprimirCamino(ant[4], entidad - 1, grafo),
         cout << " -> Desactivar la Entidad" << endl,
         cout << "Paso 3: ", grafo->imprimirCamino(ant[5], extraccion - 1, grafo),
         cout << " -> Ir a Punto de extraccion" << endl);

    delete grafo; // Liberar memoria

    return 0;
}
