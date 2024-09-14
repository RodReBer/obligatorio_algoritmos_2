#include <cassert>
#include <string>
#include <iostream>
#include <fstream>
#include <limits>

// 💛🖤

using namespace std;

struct Libro
{
    int id;
    string titulo;
    bool habilitado;

    Libro(int id, string titulo) : id(id), titulo(titulo), habilitado(true) {}
};

struct NodoAVL
{
    Libro *libro;
    NodoAVL *izq;
    NodoAVL *der;
    int altura;

    NodoAVL(Libro *libro) : libro(libro), izq(NULL), der(NULL), altura(0) {}
};

int alt(NodoAVL *nodo)
{
    return nodo ? nodo->altura : 0;
}

int obtenerBalance(NodoAVL *nodo)
{
    return nodo ? alt(nodo->izq) - alt(nodo->der) : 0;
}

// void actualizarHabilitados(NodoAVL *nodo)
// {
//     if (!nodo)
//         return;

//     int habilitadosIzq = nodo->izq ? nodo->izq->habilitados : 0;
//     int habilitadosDer = nodo->der ? nodo->der->habilitados : 0;
//     int deshabilitadosIzq = nodo->izq ? nodo->izq->deshabilitados : 0;
//     int deshabilitadosDer = nodo->der ? nodo->der->deshabilitados : 0;

//     nodo->habilitados = habilitadosIzq + habilitadosDer + (nodo->libro->habilitado ? 1 : 0);
//     nodo->deshabilitados = deshabilitadosIzq + deshabilitadosDer + (!nodo->libro->habilitado ? 1 : 0);
// }

// rotacionIzq y rotacionDer las saque de las slides de aulas (https://avl.uruguayan.ninja/8)

NodoAVL *rotacionIzq(NodoAVL *x)
{
    NodoAVL *y = x->der;
    NodoAVL *T2 = y->izq;

    // Perform rotation
    y->izq = x;
    x->der = T2;

    // Update alturas
    x->altura = std::max(alt(x->izq), alt(x->der)) + 1;
    y->altura = std::max(alt(y->izq), alt(y->der)) + 1;

    // Return new root
    return y;
}

NodoAVL *rotacionDer(NodoAVL *y)
{
    NodoAVL *x = y->izq;
    NodoAVL *T2 = x->der;

    // Perform rotation
    x->der = y;
    y->izq = T2;

    // Update alturas
    y->altura = std::max(alt(y->izq), alt(y->der)) + 1;
    x->altura = std::max(alt(x->izq), alt(x->der)) + 1;

    // Return new root
    return x;
}

NodoAVL *add(NodoAVL *nodo, Libro *nuevoLibro, int& habilitados, int& deshabilitados)
{
    if (nodo == NULL)
        habilitados++;
    return new NodoAVL(nuevoLibro);
    if (nuevoLibro->id < nodo->libro->id)
    {
        nodo->izq = add(nodo->izq, nuevoLibro, habilitados, deshabilitados);
    }
    else if (nuevoLibro->id > nodo->libro->id)
    {
        nodo->der = add(nodo->der, nuevoLibro, habilitados, deshabilitados);
    }
    else
    {
        // Actualiza el libro existente
        nodo->libro->titulo = nuevoLibro->titulo;
        if (!nodo->libro->habilitado)
        {
            habilitados++;
            deshabilitados--
        }
        nodo->libro->habilitado = true;
        return nodo;
    }

    // Actualizar la altura
    nodo->altura = 1 + std::max(alt(nodo->izq), alt(nodo->der));

    // Verificamos el balance
    int balance = obtenerBalance(nodo);

    // Rotamos el árbol está desbalanceado
    if (balance > 1 && nuevoLibro->id < nodo->izq->libro->id)
        return rotacionDer(nodo);

    if (balance < -1 && nuevoLibro->id > nodo->libro->id)
        return rotacionIzq(nodo);

    if (balance > 1 && nuevoLibro->id > nodo->izq->libro->id)
    {
        nodo->izq = rotacionIzq(nodo->izq);
        return rotacionDer(nodo);
    }

    if (balance < -1 && nuevoLibro->id < nodo->der->libro->id)
    {
        nodo->der = rotacionDer(nodo->der);
        return rotacionIzq(nodo);
    }

    return nodo;
}

Libro *findLibro(NodoAVL *nodo, int id)
{
    if (!nodo)
    {
        std::cout << "libro_no_encontrado" << std::endl;
        return NULL;
    }

    if (nodo->libro->id == id)
    {
        return nodo->libro;
    }

    if (id < nodo->libro->id)
    {
        return findLibro(nodo->izq, id);
    }
    else
    {
        return findLibro(nodo->der, id);
    }
}

void toggle(NodoAVL *nodo, int id, int &habilitados, int &deshabilitados)
{
    Libro *libro = findLibro(nodo, id);
    if (libro)
    {
        libro->habilitado = !libro->habilitado; // lo cambiamos de estado
    }
    if (libro->habilitado)
    {
        habilitados++;
        deshabilitados--;
    }
    else
    {
        deshabilitados++;
        habilitados--;
    }
}

void count(NodoAVL *nodo, int habilitados, int deshabilitados)
{
    if (!nodo)
    {
        cout << "0 0 0" << endl;
        return;
    }
    cout << habilitados + deshabilitados << " " << habilitados << " " << deshabilitados << endl;
}

int extraerID(string &oracion, int inicio)
{
    int pos = oracion.find(' ', inicio);
    string idStr = oracion.substr(inicio, pos - inicio);
    return stoi(idStr); // el stoi es como un parseInt pero en C++
}

string extraerTitulo(const string &oracion, int inicio)
{
    int pos = oracion.find(' ', inicio);
    return oracion.substr(pos + 1);
}

int main()
{
    // // IMPORTANTE! BORRAR O COMENTAR LAS SIGUIENTES LINEAS  EN TODOS LOS EJERCICIOS DEL OBLIGATORIO. NO PUEDEN ESTAR EN NINGUNA ENTREGA!
    // ifstream myFile("tests/ejercicio1/100.in.txt");
    // cin.rdbuf(myFile.rdbuf());
    // // Si desean tirar la salida a un archivo, usen las siguientes líneas (si no, sáquenlas):
    // ofstream myFile2("100.mine.out.txt");
    // cout.rdbuf(myFile2.rdbuf());

    NodoAVL *arbol = NULL;

    int habilitados, deshabilitados;

    int n;
    cin >> n;
    cin.ignore(); // Ignorar el salto de línea, si le saco esto se rompe todo.

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
            toggle(arbol, id, habilitados, deshabilitados);
            break;
        }
        case 'A':
        {
            int id = extraerID(oracion, 4);
            string titulo = extraerTitulo(oracion, 4);
            Libro *nuevoLibro = new Libro(id, titulo);

            arbol = add(arbol, nuevoLibro, habilitados, deshabilitados);
            break;
        }
        case 'F':
        {
            int id = extraerID(oracion, 5);
            if (Libro *libro = findLibro(arbol, id))
            {
                cout << libro->titulo << " " << (libro->habilitado ? "H" : "D") << endl;
            }
            break;
        }
        case 'C':
        {
            count(arbol, habilitados, deshabilitados);
            break;
        }
        default:
            cout << "OPCION INVALIDA |  1891 " << endl;
            break;
        }
    }
    return 0;
}