#include <cassert>
#include <string>
#include <iostream>
#include <limits>
using namespace std;

const int LIMITE_CIUDAD = 50;
const int LIMITE_MISION = 200;
const int INFINITO = numeric_limits<int>::max();

struct Enlace {
    int destino;
    int costo;
    Enlace* siguiente;

    Enlace(int d, int c, Enlace* sig = NULL) : destino(d), costo(c), siguiente(sig) {}
};

struct Red {
    Enlace* cabeza;

    Red() : cabeza(NULL) {}

    void agregarConexion(int destino, int costo) {
        cabeza = new Enlace(destino, costo, cabeza);
    }

    ~Red() {
        while (cabeza != NULL) {
            Enlace* temp = cabeza;
            cabeza = cabeza->siguiente;
            delete temp;
        }
    }
};

struct Elemento {
    int valor;
    Elemento* siguiente;

    Elemento(int v, Elemento* sig = NULL) : valor(v), siguiente(sig) {}
};

struct ListaElementos {
    Elemento* cabeza;

    ListaElementos() : cabeza(NULL) {}
    
    void agregarInicio(int valor) {
        Elemento* nuevo = new Elemento(valor); 
        nuevo->siguiente = cabeza;   
        cabeza = nuevo;     
    }

    void agregarFinal(int valor) {
        if (cabeza == NULL) {
            cabeza = new Elemento(valor);
        } else {
            Elemento* actual = cabeza;
            while (actual->siguiente != NULL) {
                actual = actual->siguiente;
            }
            actual->siguiente = new Elemento(valor);
        }
    }

    int extraerInicio() {
        if (cabeza == NULL) return -1;
        int valor = cabeza->valor;
        Elemento* temp = cabeza;
        cabeza = cabeza->siguiente;
        delete temp;
        return valor;
    }

    bool estaVacia() {
        return cabeza == NULL;
    }

    ~ListaElementos() {
        while (cabeza != NULL) {
            Elemento* temp = cabeza;
            cabeza = cabeza->siguiente;
            delete temp;
        }
    }
};

struct Tarea {
    int idTarea;
    string nombreTarea;
    int idCiudad;
    ListaElementos dependencias;

    Tarea(int id, string nombre, int ciudad) 
        : idTarea(id), nombreTarea(nombre), idCiudad(ciudad) {}
};

struct GrafoTareas {
    Red listaAdy[LIMITE_CIUDAD];
    string nombreCiudad[LIMITE_CIUDAD];
    int tope;

    GrafoTareas(int tope) : tope(tope) {}

    void setNombreCiudad(int id, string nombre) {
        nombreCiudad[id - 1] = nombre;
    }

    void agregarConexion(int origen, int destino, int costo) {
        listaAdy[origen - 1].agregarConexion(destino - 1, costo);
        listaAdy[destino - 1].agregarConexion(origen - 1, costo);
    }

    int encontrarMasCercanoNoVisitado(int* dist, bool* visitado, int tope) {
        int posMin = -1;
        int minDist = INFINITO;
        for (int i = 0; i < tope; i++) {
            if (!visitado[i] && dist[i] < minDist) {
                minDist = dist[i];
                posMin = i;
            }
        }
        return posMin;
    }

    int realizarDijkstra(int origen, int destino, ListaElementos& ruta) {
        int dist[LIMITE_CIUDAD];
        bool visitado[LIMITE_CIUDAD] = {false};
        int anterior[LIMITE_CIUDAD];

        for (int i = 0; i < LIMITE_CIUDAD; i++) {
            dist[i] = INFINITO;
            anterior[i] = -1;
        }
        
        dist[origen - 1] = 0;

        for (int i = 0; i < tope; i++) {
            int candidato = encontrarMasCercanoNoVisitado(dist, visitado, tope);
            if (candidato == -1) break;
            visitado[candidato] = true;
            for (Enlace* enlace = listaAdy[candidato].cabeza; enlace; enlace = enlace->siguiente) {
                if (dist[candidato] + enlace->costo < dist[enlace->destino]) {
                    dist[enlace->destino] = dist[candidato] + enlace->costo;
                    anterior[enlace->destino] = candidato;
                }
            }
        }
        int actual = destino - 1;
        ruta.agregarFinal(actual);

        while (anterior[actual] != -1) {
            actual = anterior[actual];
            ruta.agregarInicio(actual);
        }
        return dist[destino - 1];
    }
};

void ordenTareas(Tarea* tareas[], int numTareas, ListaElementos& orden) {
    int dependencias[LIMITE_MISION] = {0};
    ListaElementos sinDependencias;

    for (int i = 0; i < numTareas; i++) {
        for (Elemento* elem = tareas[i]->dependencias.cabeza; elem; elem = elem->siguiente) {
            dependencias[elem->valor]++;
        }
    }

    for (int i = 0; i < numTareas; i++) {
        if (dependencias[i] == 0) {
            sinDependencias.agregarFinal(i);
        }
    }

    while (!sinDependencias.estaVacia()) {
        Elemento* actualElem = sinDependencias.cabeza;
        Elemento* tareaSeleccionada = actualElem;

        while (actualElem) {
            if (tareas[actualElem->valor]->idTarea < tareas[tareaSeleccionada->valor]->idTarea) {
                tareaSeleccionada = actualElem;
            }
            actualElem = actualElem->siguiente;
        }

        int actual = tareaSeleccionada->valor;
        
        if (sinDependencias.cabeza == tareaSeleccionada) {
            sinDependencias.cabeza = tareaSeleccionada->siguiente;
        } else {
            Elemento* prev = sinDependencias.cabeza;
            while (prev->siguiente != tareaSeleccionada) {
                prev = prev->siguiente;
            }
            prev->siguiente = tareaSeleccionada->siguiente;
        }

        orden.agregarFinal(actual);

        for (Elemento* elem = tareas[actual]->dependencias.cabeza; elem; elem = elem->siguiente) {
            dependencias[elem->valor]--;
            if (dependencias[elem->valor] == 0) {
                sinDependencias.agregarFinal(elem->valor);
            }
        }
    }
};
