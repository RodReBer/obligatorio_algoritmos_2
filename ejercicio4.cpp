#include <cassert>
#include <string>
#include <iostream>
#include <limits>
#include "tads/Ejercicio4.cpp"

using namespace std;

int main()
{
    int cantPedidos, cantOp;
    cin >> cantPedidos;
    cin >> cantOp;

    ColaPrioridad *cola = new ColaPrioridad(cantPedidos);

    for (int i = 0; i < cantOp; i++)
    {
        string tarea;
        cin >> tarea;

        switch (tarea[0])
        {
        case 'I':
        {
            int id, prioridad;
            string paraLlevarStr, items;

            cin >> id;
            cin >> prioridad;
            cin >> paraLlevarStr;
            cin >> items;
            bool paraLlevar = (paraLlevarStr == "true");

            cola->agregarPedido(id, prioridad, paraLlevar, items);
            break;
        }
        case 'E':
        {
            int id;
            cin >> id;

            string idStr = to_string(id);
            int posEnCola = cola->obtenerTabla()->obtenerPosicion(idStr);

            if (posEnCola != -1)
            { // Verificar si la posición es válida
                cola->quitarPedido(posEnCola);
                cola->obtenerTabla()->eliminar(idStr);
            }
            break;
        }
        case 'C':
        {
            int id;
            cin >> id;

            string idStr = to_string(id);
            int posEnCola = cola->obtenerTabla()->obtenerPosicion(idStr);

            if (posEnCola != -1)
            { // Verificar si la posición es válida
                cola->modificarParaLlevar(posEnCola);
            }
            break;

        }
        default:
            cout << "Tarea desconocida: " << tarea << endl;
            break;
        }
    }

    cola->mostrarPedidos();

    // Eliminar el objeto creado con new
    delete cola;
    return 0;
}