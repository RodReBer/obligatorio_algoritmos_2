#include <cassert>
#include <string>
#include <iostream>
#include <limits>
#include "tads/Dijkstra.cpp"
using namespace std;

int main() {
    int numTareas, numCiudades, ciudadInicial;
    cin >> numTareas;
    Tarea* tareas[MAX_TAREAS] = {NULL};
    for (int i = 0; i < numTareas; i++) {
        int idTarea, idCiudad;
        string nombreTarea;
        cin >> idTarea >> nombreTarea >> idCiudad;
        tareas[idTarea - 1] = new Tarea(idTarea, nombreTarea, idCiudad);

        int dependencia;
        while (cin >> dependencia && dependencia != 0) {
            tareas[idTarea - 1]->dependencias.agregarFinal(dependencia - 1);
        }
    }
    cin >> numCiudades >> ciudadInicial;
    GrafoTareas grafo(numCiudades);
    for (int i = 0; i < numCiudades; i++) {
        int idCiudad;
        string nombreCiudad;
        cin >> idCiudad >> nombreCiudad;
        grafo.setNombreCiudad(idCiudad, nombreCiudad);
    }

    int numConexiones;
    cin >> numConexiones;
    for (int i = 0; i < numConexiones; i++) {
        int origen, destino, costo;
        cin >> origen >> destino >> costo;
        grafo.agregarConexion(origen, destino, costo);
    }

    ListaElementos orden;
    ordenTareas(tareas, numTareas, orden);

    Elemento* actualElemento = orden.cabeza;
    int ciudadActual = ciudadInicial;
    int tiempoTotal = 0;
    cout << "Ciudad inicial: " << grafo.nombreCiudad[ciudadActual - 1] << endl;

    // Recorremos las tareas
    while (actualElemento) {
        Tarea* tarea = tareas[actualElemento->valor];
        ListaElementos camino;
        int tiempo = grafo.realizarDijkstra(ciudadActual, tarea->idCiudad, camino);

        Elemento* nodoCamino = camino.cabeza;
        while (nodoCamino) {
            cout << grafo.nombreCiudad[nodoCamino->valor] << " -> ";
            nodoCamino = nodoCamino->siguiente;
        }

        cout << "Tarea: " << tarea->nombreTarea
             << " - " << grafo.nombreCiudad[tarea->idCiudad - 1]
             << " - Tiempo de viaje: " << tiempo << endl;

        ciudadActual = tarea->idCiudad;
        tiempoTotal += tiempo;
        actualElemento = actualElemento->siguiente;
    }

    cout << "Tareas completadas con exito." << endl;
    cout << "Tiempo total de viaje: " << tiempoTotal << endl;
    for (int i = 0; i < numTareas; i++) {
        delete tareas[i];
    }

    return 0;
}
