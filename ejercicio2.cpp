//Rodrigo Rey y Román Ferrero
//Estructura de datos y algoritmos 2
#include <iostream>
#include <string>
#include <cmath>
#include <fstream>
#include "tads/HashEj2/hashEj2.cpp"
#include "funciones/ejercicio2.cpp"

using namespace std;

int main()
{
    int tamañoTabla = 1000003; // Tamaño de la tabla hash
    Hash<int, Libro *> *tabla = new Hash<int, Libro *>(tamañoTabla);

    int n;
    cin >> n;
    cin.ignore(); // Ignorar el salto de línea

    for (int i = 0; i < n; i++)
    {
        string oracion;
        getline(cin, oracion);

        char opcion = oracion[0];

        switch (opcion)
        {
        case 'T':
        {
            int id = extraerID(oracion, 7);
            if (tabla->existe(id))
            {
                Libro *libro = tabla->recuperar(id);
                libro->habilitado = !libro->habilitado;

                if (libro->habilitado)
                {
                    tabla->incrementarHabilitados();
                    tabla->decrementarDeshabilitados();
                }
                else
                {
                    tabla->incrementarDeshabilitados();
                    tabla->decrementarHabilitados();
                }
            }
            else
            {
                cout << "libro_no_encontrado" << endl;
            }
            break;
        }
        case 'A':
        {
            int id = extraerID(oracion, 4);
            string titulo = extraerTitulo(oracion, 4);
            Libro *nuevoLibro = new Libro(id, titulo);
            if (tabla->existe(id))
            {
               tabla->eliminar(id);
            }
            tabla->insertar(id, nuevoLibro);
            break;
        }
        case 'F':
        {
            int id = extraerID(oracion, 5);
            if (tabla->existe(id))
            {
                Libro *libro = tabla->recuperar(id);
                cout << libro->titulo << " " << (libro->habilitado ? "H" : "D") << endl;
            }
            else
            {
                cout << "libro_no_encontrado" << endl;
            }
            break;
        }
        case 'C':
        {
            cout << tabla->getHabilitados() + tabla->getDeshabilitados() << " "
                 << tabla->getHabilitados() << " "
                 << tabla->getDeshabilitados() << endl;
            break;
        }
        default:
            cout << "OPCION INVALIDA | 1891" << endl;
            break;
        }
    }

    delete tabla; // Liberar la memoria de la tabla hash
    return 0;
}
