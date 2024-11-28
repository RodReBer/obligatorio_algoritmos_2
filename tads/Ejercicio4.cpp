#include <iostream>
#include <string>
#include <limits>

using namespace std;
//el input mas grande que le puede venir
const int MAX_PEDIDOS = 100000;

struct Pedido {
    int id;
    int prioridad;
    bool paraLlevar;
    string items;
};

class TablaPedidos {
private:
    struct Nodo {
        string clave; 
        int valor;
        Nodo* siguiente;
    };

    Nodo* tabla[MAX_PEDIDOS];
    int capacidad;

    //la vimos en clase
    int hash2(string clave) {
        int h = 0;
        for (int i = 0; i < clave.length(); i++)
            h = h + clave[i] * (i + 1);
        return h;
    }

public:
    TablaPedidos(int tam) {
        capacidad = tam;
        for (int i = 0; i < capacidad; i++) {
            tabla[i] = NULL;
        }
    }

    void insertar(string clave, int valor) {  
        int indice = hash2(clave) % capacidad;  
        Nodo* nuevo = new Nodo;
        nuevo->clave = clave;
        nuevo->valor = valor;
        nuevo->siguiente = tabla[indice];
        tabla[indice] = nuevo;
    }

    void eliminar(string clave) {  // Modificado a string
        int indice = hash2(clave) % capacidad;  // Usando hash2
        Nodo* actual = tabla[indice];
        Nodo* anterior = NULL;

        while (actual != NULL && actual->clave != clave) {
            anterior = actual;
            actual = actual->siguiente;
        }

        if (actual == NULL) return;

        if (anterior == NULL) {
            tabla[indice] = actual->siguiente;
        } else {
            anterior->siguiente = actual->siguiente;
        }

        delete actual;
    }

    int obtenerPosicion(string clave) {  // Modificado a string
        int indice = hash2(clave) % capacidad;  // Usando hash2
        Nodo* actual = tabla[indice];

        while (actual != NULL) {
            if (actual->clave == clave) {
                return actual->valor;
            }
            actual = actual->siguiente;
        }

        return -1;
    }
};

class ColaPrioridad {
private:
    Pedido* pedidos;
    int capacidad;
    int cantidad;
    TablaPedidos* tabla;

    void intercambiar(int i, int j) {
        Pedido temp = pedidos[i];
        pedidos[i] = pedidos[j];
        pedidos[j] = temp;
        tabla->insertar(to_string(pedidos[i].id), i);  // Usando string como clave
        tabla->insertar(to_string(pedidos[j].id), j);  // Usando string como clave
    }

    bool compararPedidos(Pedido& a, Pedido& b) {
        if (a.prioridad != b.prioridad) {
            return a.prioridad < b.prioridad;
        }
        if (a.paraLlevar != b.paraLlevar) {
            return a.paraLlevar;
        }
        return a.id < b.id;
    }

    void flotar(int indice) {
        while (indice > 0) {
            int padre = (indice - 1) / 2;
            if (compararPedidos(pedidos[indice], pedidos[padre])) {
                intercambiar(indice, padre);
                indice = padre;
            } else {
                break;
            }
        }
    }

    void hundir(int indice) {
        while (true) {
            int izquierda = 2 * indice + 1;
            int derecha = 2 * indice + 2;
            int menor = indice;

            if (izquierda < cantidad && compararPedidos(pedidos[izquierda], pedidos[menor])) {
                menor = izquierda;
            }
            if (derecha < cantidad && compararPedidos(pedidos[derecha], pedidos[menor])) {
                menor = derecha;
            }

            if (menor == indice) {
                break;
            }

            intercambiar(indice, menor);
            indice = menor;
        }
    }

public:
    ColaPrioridad(int cap) {
        capacidad = cap;
        cantidad = 0;
        pedidos = new Pedido[capacidad];
        tabla = new TablaPedidos(capacidad);
    }

    ~ColaPrioridad() {
        delete[] pedidos;
        delete tabla;
    }

    void agregarPedido(int id, int prioridad, bool paraLlevar, string items) {
        if (cantidad >= capacidad) {
            cout << "Cola llena" << endl;
            return;
        }
        Pedido nuevoPedido = {id, prioridad, paraLlevar, items};
        pedidos[cantidad] = nuevoPedido;
        tabla->insertar(to_string(id), cantidad);  // Usamos string para el id
        flotar(cantidad);
        cantidad++;
    }

    void quitarPedido(int posicion) {
        if (cantidad == 0) {
            cout << "Cola vacía" << endl;
            return;
        }
        intercambiar(posicion, cantidad - 1);
        cantidad--;
        if (posicion < cantidad) {
            hundir(posicion);
        }
    }

    void modificarParaLlevar(int posicion) {
        pedidos[posicion].paraLlevar = true;
        flotar(posicion);
    }

    void mostrarPedidos() {
        Pedido* temp = new Pedido[cantidad];
        for (int i = 0; i < cantidad; i++) {
            temp[i] = pedidos[i];
        }

        for (int i = 0; i < cantidad - 1; i++) {
            for (int j = 0; j < cantidad - i - 1; j++) {
                if (!compararPedidos(temp[j], temp[j + 1])) {
                    Pedido tempPedido = temp[j];
                    temp[j] = temp[j + 1];
                    temp[j + 1] = tempPedido;
                }
            }
        }

        for (int i = 0; i < cantidad; i++) {
            cout << temp[i].id << " " << temp[i].prioridad << " "
                 << (temp[i].paraLlevar ? "true" : "false") << " "
                 << temp[i].items << endl;
        }

        delete[] temp;
    }

    TablaPedidos* obtenerTabla() {
        return tabla;
    }
};