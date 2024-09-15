#include <iostream>
#include <string>
#include <cmath>
using namespace std;

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
    int (*funcionHash)(K);

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
    Hash(int maxCantidad, int (*fHash)(K)) : cantidad(0), funcionHash(fHash)
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
                // No hay necesidad de liberar memoria si V no es un puntero
                // delete tabla[i]->valor; // Solo si V es un puntero
                delete tabla[i];
            }
        }
        delete[] tabla;
    }

    void insertar(K clave, V valor)
    {
        int pos = abs(funcionHash(clave)) % tamañoTabla;
        int paso = 1 + (abs(funcionHash(clave)) % (tamañoTabla - 1));

        while (tabla[pos] && !tabla[pos]->eliminado && tabla[pos]->clave != clave)
        {
            pos = (pos + paso) % tamañoTabla;
        }

        if (tabla[pos])
        {
            // No hay necesidad de liberar memoria si V no es un puntero
            // delete tabla[pos]->valor; // Solo si V es un puntero
            delete tabla[pos];
        }
        tabla[pos] = new NodoHash<K, V>(clave, valor);
        cantidad++;
    }

    void Eliminar(K clave)
    {
        int posHash = abs(funcionHash(clave)) % tamañoTabla;
        while (tabla[posHash] && tabla[posHash]->clave != clave)
        {
            posHash = (posHash + 1) % tamañoTabla;
        }
        if (tabla[posHash])
        {
            tabla[posHash]->eliminado = true;
        }
    }

    V recuperar(K clave)
    {
        int posHash = abs(funcionHash(clave)) % tamañoTabla;
        while (tabla[posHash]->clave != clave)
        {
            posHash = (posHash + 1) % tamañoTabla;
        }
        return tabla[posHash]->valor;
    }

    bool existe(K clave)
    {
        int pos = abs(funcionHash(clave)) % tamañoTabla;
        int paso = 1 + (abs(funcionHash(clave)) % (tamañoTabla - 1));

        while (tabla[pos] && tabla[pos]->clave != clave)
        {
            pos = (pos + paso) % tamañoTabla;
        }
        return tabla[pos] && !tabla[pos]->eliminado;
    }

    void count(int &habilitados, int &deshabilitados)
    {
        habilitados = 0;
        deshabilitados = 0;

        for (int i = 0; i < tamañoTabla; i++)
        {
            if (tabla[i] && !tabla[i]->eliminado)
            {
                if (tabla[i]->valor.habilitado)
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

int funcionHash(int id)
{
    return id % 1000003;
}

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
    Hash<int, Libro> *tabla = new Hash<int, Libro>(tamañoTabla, funcionHash);

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
                Libro libro = tabla->recuperar(id);
                libro.habilitado = !libro.habilitado;
                tabla->insertar(id, libro);
            }
            break;
        }
        case 'A':
        {
            int id = extraerID(oracion, 4);
            string titulo = extraerTitulo(oracion, 4);
            Libro nuevoLibro(id, titulo);
            tabla->insertar(id, nuevoLibro);
            break;
        }
        case 'F':
        {
            int id = extraerID(oracion, 5);
            if (tabla->existe(id))
            {
                Libro libro = tabla->recuperar(id);
                cout << libro.titulo << " " << (libro.habilitado ? "H" : "D") << endl;
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

    delete tabla; // Liberar la memoria asignada dinámicamente

    return 0;
}
