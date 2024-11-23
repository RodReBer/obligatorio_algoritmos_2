#include <iostream>
#include <string>
#include <cmath>
#include <fstream>

using namespace std;

int hash3(string key)
{
    int h = 0;
    for (int i = 0; i < key.length(); i++)
        h = 31 * h + int(key[i]);
    return h;
}

int hash2(string key)
{
    int h = 0;
    for (int i = 0; i < key.length(); i++)
        h = h + key[i] * (i + 1);
    return h;
}


template <class K, class V>
class NodoHash
{
public:
    K clave;
    V valor;
    bool eliminado; // Marca si el nodo está marcado como eliminado

    NodoHash(K clave, V valor) : clave(clave), valor(valor), eliminado(false) {}
};

template <class K, class V>
class Hash
{
private:
    NodoHash<K, V> **tabla;
    int cantidad;
    int tamañoTabla;
    int habilitados;
    int deshabilitados;

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

    int primoSup(int num)
    {
        while (!esPrimo(++num))
            ;
        return num;
    }

public:
    Hash(int maxCantidad) : cantidad(0), habilitados(0), deshabilitados(0)
    {
        tamañoTabla = primoSup(maxCantidad * 2);
        tabla = new NodoHash<K, V> *[tamañoTabla]();
    }

    ~Hash()
    {
        for (int i = 0; i < tamañoTabla; i++)
        {
            if (tabla[i] && !tabla[i]->eliminado)
            {
                delete tabla[i]->valor; // Liberar el objeto Libro
                delete tabla[i];
            }
        }
        delete[] tabla;
    }

    void insertar(K clave, V valor)
    {
        int tryCount = 0; // Contador de intentos para el doble hashing
        int posHash;
        do
        {
            posHash = (abs(hash3(to_string(clave)) + tryCount * hash2(to_string(clave)))) % tamañoTabla;
            tryCount++;
        } while (tabla[posHash] && !tabla[posHash]->eliminado);

        tabla[posHash] = new NodoHash<K, V>(clave, valor);
        habilitados++;
        cantidad++;
    }

    void eliminar(K clave)
    {
        int tryCount = 0; // Contador de intentos para el doble hashing
        int posHash;
        do
        {
            posHash = (abs(hash3(to_string(clave)) + tryCount * hash2(to_string(clave)))) % tamañoTabla;
            tryCount++;
        } while (tabla[posHash] && tabla[posHash]->clave != clave);

        if (tabla[posHash])
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

    V recuperar(K clave)
    {
        int tryCount = 0; // Contador de intentos para el doble hashing
        int posHash;
        do
        {
            posHash = (abs(hash3(to_string(clave)) + tryCount * hash2(to_string(clave)))) % tamañoTabla;
            tryCount++;
        } while (tabla[posHash] && tabla[posHash]->clave != clave);

        return tabla[posHash]->valor;
    }

    bool existe(K clave)
    {
        int tryCount = 0; // Contador de intentos para el doble hashing
        int posHash;
        do
        {
            posHash = (abs(hash3(to_string(clave)) + tryCount * hash2(to_string(clave)))) % tamañoTabla;
            tryCount++;
        } while (tabla[posHash] && tabla[posHash]->clave != clave);

        return tabla[posHash] && !tabla[posHash]->eliminado;
    }

    void toggle(K clave)
    {
        if (existe(clave))
        {
            V libro = recuperar(clave);
            libro->habilitado = !libro->habilitado;

            if (libro->habilitado)
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
        else
        {
            cout << "libro_no_encontrado" << endl;
        }
    }

    int getHabilitados()
    {
        return habilitados;
    }

    int getDeshabilitados()
    {
        return deshabilitados;
    }

    int incrementarHabilitados()
    {
        return habilitados++;
    }

    int decrementarHabilitados()
    {
        return habilitados--;
    }

    int incrementarDeshabilitados()
    {
        return deshabilitados++;
    }

    int decrementarDeshabilitados()
    {
        return deshabilitados--;
    }
};
