#include <iostream>
#include <string>
#include <cmath>
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

    bool esPrimo(int num)
    {
        if (num <= 1)
        {
            return false;
        }
        else
        {
            for (int i = 2; i < num / 2; i++)
            {
                if (num % i == 0)
                {
                    return false;
                }
            }
            return true;
        }
    }

    int primoSup(int num)
    {
        while (!esPrimo(++num))
            ;
        return num;
    }

public:
    Hash(int maxCantidad) : cantidad(0)
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

        if (tabla[posHash])
        {
            delete tabla[posHash];
        }
        tabla[posHash] = new NodoHash<K, V>(clave, valor);
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
        } while (tabla[posHash]->clave != clave);

        tabla[posHash]->eliminado = true;
    }

    V recuperar(K clave)
    {
        int tryCount = 0; // Contador de intentos para el doble hashing
        int posHash;
        do
        {
            posHash = (abs(hash3(to_string(clave)) + tryCount * hash2(to_string(clave)))) % tamañoTabla;
            tryCount++;
        } while (tabla[posHash]->clave != clave);

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

    void count(int &habilitados, int &deshabilitados)
    {
        habilitados = 0;
        deshabilitados = 0;

        for (int i = 0; i < tamañoTabla; i++)
        {
            if (tabla[i] && !tabla[i]->eliminado)
            {
                if (tabla[i]->valor->habilitado)
                {
                    habilitados++;
                }
                else
                {
                    deshabilitados++;
                }
            }
        }
    }
};

struct Libro
{
    int id;
    string titulo;
    bool habilitado;

    Libro(int id, string titulo) : id(id), titulo(titulo), habilitado(true) {}
};

int extraerID(const string &oracion, int inicio)
{
    int pos = oracion.find(' ', inicio);
    string idStr = oracion.substr(inicio, pos - inicio);
    return stoi(idStr);
}

string extraerTitulo(const string &oracion, int inicio)
{
    int pos = oracion.find(' ', inicio);
    return oracion.substr(pos + 1);
}

int main()
{
    int tamañoTabla = 1000003; // Tamaño de la tabla hash
    Hash<int, Libro *> *tabla = new Hash<int, Libro *>(tamañoTabla);

    int habilitados = 0;
    int deshabilitados = 0;

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
            tabla->count(habilitados, deshabilitados);
            cout << habilitados + deshabilitados << " " << habilitados << " " << deshabilitados << endl;
            break;
        }
        default:
            cout << "OPCION INVALIDA" << endl;
            break;
        }
    }

    return 0;
}
