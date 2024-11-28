#include <cassert>
#include <string>
#include <iostream>
#include <limits>
using namespace std;

template <class K>
class Hash
{
private:
    struct NodoHash
    {
        K clave;
        int posicionHeap;
        bool eliminado;

        NodoHash(K clave, int posicion) : clave(clave), posicionHeap(posicion), eliminado(false) {}
    };

    NodoHash **tabla;
    int cantidad;
    int tamañoTabla;

    // Función auxiliar para verificar si un número es primo
    bool esPrimo(int num)
    {
        if (num <= 1)
        {
            return false;
        }
        for (int i = 2; i <= sqrt(num); i++)
        {
            if (num % i == 0)
            {
                return false;
            }
        }
        return true;
    }

    // Encuentra el siguiente número primo mayor a `num`
    int primoSup(int num)
    {
        while (!esPrimo(++num))
            ;
        return num;
    }

    // Genera una posición usando hash doble
    int posicionHash(K clave, int tryCount)
    {
        int hashPrimario = abs(hash3(to_string(clave))) % tamañoTabla;
        int hashSecundario = abs(hash2(to_string(clave))) % tamañoTabla;
        return (hashPrimario + tryCount * hashSecundario) % tamañoTabla;
    }

public:
    // Constructor
    Hash(int maxCantidad) : cantidad(0)
    {
        tamañoTabla = primoSup(maxCantidad * 2);
        tabla = new NodoHash *[tamañoTabla]();
    }

    // Destructor
    ~Hash()
    {
        for (int i = 0; i < tamañoTabla; i++)
        {
            if (tabla[i] && !tabla[i]->eliminado)
            {
                delete tabla[i];
            }
        }
        delete[] tabla;
    }

    // Inserta una clave con su posición en el heap
    void insertar(K clave, V valor)
    {
        int tryCount = 0;
        int posHash;

        do
        {
            posHash = posicionHash(clave, tryCount);
            tryCount++;
        } while (tabla[posHash] && !tabla[posHash]->eliminado && tabla[posHash]->clave != clave);

        if (tabla[posHash] == nullptr || tabla[posHash]->eliminado)
        {
            if (tabla[posHash] == nullptr)
            {
                cantidad++;
            }

            tabla[posHash] = new NodoHash<K, V>(clave, valor);
            if (valor->habilitado)
            {
                habilitados++;
            }
            else
            {
                deshabilitados++;
            }
        }
        else
        {
            // Si la clave ya existe, actualiza el valor
            if (tabla[posHash]->valor->habilitado != valor->habilitado)
            {
                if (valor->habilitado)
                {
                    habilitados++;
                    deshabilitados--;
                }
                else
                {
                    habilitados--;
                    deshabilitados++;
                }
            }
            tabla[posHash]->valor = valor;
        }
    }

    int obtenerPosEnHeap(K clave)
    {
        int tryCount = 0;
        int posHash;
        do
        {
            posHash = posicionHash(clave, tryCount);
            tryCount++;
        } while (tabla[posHash] && tabla[posHash]->clave != clave);

        if (tabla[posHash] && !tabla[posHash]->eliminado)
        {
            return tabla[posHash]->valor; // Retorna la posición almacenada en la tabla
        }
        return -1; // Si no se encuentra la clave
    }

    // Actualiza la posición de un pedido en el heap
    void actualizarPosicion(K clave, V nuevaPos)
    {
        int tryCount = 0;
        int posHash;

        do
        {
            posHash = posicionHash(clave, tryCount);
            tryCount++;
        } while (tabla[posHash] && tabla[posHash]->clave != clave);

        if (tabla[posHash] && !tabla[posHash]->eliminado)
        {
            tabla[posHash]->valor = nuevaPos; // Actualizar la posición
        }
    }

    // Recupera la posición de un pedido en el heap
    int recuperarPosicion(K clave)
    {
        int tryCount = 0;
        int posHash;

        do
        {
            posHash = posicionHash(clave, tryCount);
            tryCount++;
        } while (tabla[posHash] && tabla[posHash]->clave != clave);

        if (tabla[posHash] && !tabla[posHash]->eliminado)
        {
            return tabla[posHash]->posicionHeap;
        }
        return -1; // Devuelve -1 si no se encuentra
    }

    // Elimina un elemento marcándolo como eliminado
    void eliminar(K clave)
    {
        int tryCount = 0;
        int posHash;

        do
        {
            posHash = posicionHash(clave, tryCount);
            tryCount++;
        } while (tabla[posHash] && tabla[posHash]->clave != clave);

        if (tabla[posHash] && !tabla[posHash]->eliminado)
        {
            if (tabla[posHash]->valor->habilitado)
            {
                habilitados--;
            }
            else
            {
                deshabilitados--;
            }

            tabla[posHash]->eliminado = true;
            cantidad--;
        }
    }

    // Verifica si una clave existe en la tabla
    bool existe(K clave)
    {
        int tryCount = 0;
        int posHash;

        do
        {
            posHash = posicionHash(clave, tryCount);
            tryCount++;
        } while (tabla[posHash] && tabla[posHash]->clave != clave);

        return tabla[posHash] && !tabla[posHash]->eliminado;
    }
};
