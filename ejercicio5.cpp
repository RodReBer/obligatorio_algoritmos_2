// Rodrigo Rey y Román Ferrero
// Estructura de datos y algoritmos 2
#include <cassert>
#include <string>
#include <iostream>
#include <limits>
#include "Lista/lista.cpp"
#include "grafo.cpp"

using namespace std;

struct ciudad
{
    int id;
    string nombre;

    ciudad(int id, string nombre) : id(id), nombre(nombre) {}
};

struct mision
{
    int id;
    string nombre;
    int idCiudadOrigen;
    Lista<int> listaIdMisiones;

    mision(int id, string nombre, int idCiudadOrigen, Lista<int> listaIdMisiones) : id(id), nombre(nombre), idCiudadOrigen(idCiudadOrigen), listaIdMisiones(listaIdMisiones) {}
};

int main()
{
    cin >> m; // misiones

    Grafo<mision> *grafoMisiones = new Grafo<mision>(m + 1);

    for (int i = 1; i <= m; i++)
    {
        int idMision;
        string nombreMision;
        int idCiudadOrigen;
        Lista<int> listaIdMisiones;

        cin >> idMision >> nombreMision >> idCiudadOrigen;

        int dependencia;
        cin >> dependencia;
        while (dependencia != 0)
        {
            listaIdMisiones.InsertarFin(dependencia);
            cin >> dependencia;
        }

    }

    cin >> c; // ciudades
    cin >> o; // ciudad origen

    Grafo<ciudad> *grafoCiudades = new Grafo<ciudad>(c + 1);

    for (int i = 1; i <= c; i++)
    {
        int idCiudad;
        string nombreCiudad;

        cin >> idCiudad;
        cin >> nombreCiudad;
    }

    cin >> e; // cantidad de lineas que representan conexionese entre ciudades

    for (int i = 0; i < e; i++)
    {
        int idCiudadOrigen;
        int idCiudadDestino;
        int tiempoDesplazamiento;

        cin >> idCiudadOrigen;
        cin >> idCiudadDestino;
        cin >> tiempoDesplazamiento;
    }
    return 0;
}