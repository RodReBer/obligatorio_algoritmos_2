//Rodrigo Rey y Román Ferrero
//Estructura de datos y algoritmos 2
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

// extraido desde las ppt de aulas (https://avl.uruguayan.ninja/7)

NodoAVL *add(NodoAVL *nodo, Libro *nuevoLibro, int &habilitados, int &deshabilitados)
{
    /* 1. Perform the normal BST insertion */
    if (nodo == NULL)
    {
        habilitados++;
        return new NodoAVL(nuevoLibro);
    }
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
            deshabilitados--;
        }
        nodo->libro->habilitado = true;
        return nodo;
    }

    /* 2. Update height of this ancestor node */
    nodo->altura = 1 + std::max(alt(nodo->izq), alt(nodo->der));

    /* 3. Get the balance factor of this ancestor node to check whether this node became unbalanced */
    int balance = obtenerBalance(nodo);

    // If this node becomes unbalanced, then there are 4 cases

    // Left Left Case
    if (balance > 1 && nuevoLibro->id < nodo->izq->libro->id)
        return rotacionDer(nodo);

    // Right Right Case
    if (balance < -1 && nuevoLibro->id > nodo->der->libro->id)
        return rotacionIzq(nodo);

    // Left Right Case
    if (balance > 1 && nuevoLibro->id > nodo->izq->libro->id)
    {
        nodo->izq = rotacionIzq(nodo->izq);
        return rotacionDer(nodo);
    }

    // Right Left Case
    if (balance < -1 && nuevoLibro->id < nodo->der->libro->id)
    {
        nodo->der = rotacionDer(nodo->der);
        return rotacionIzq(nodo);
    }

    /* return the (unchanged) node pointer */
    return nodo;
}

Libro *findLibro(NodoAVL *nodo, int id)
{
    while (nodo)
    {
        if (nodo->libro->id == id)
        {
            return nodo->libro;
        }
        if (id < nodo->libro->id)
        {
            nodo = nodo->izq;
        }
        else
        {
            nodo = nodo->der;
        }
    }
    std::cout << "libro_no_encontrado" << std::endl;
    return NULL;
}

void toggle(NodoAVL *nodo, int id, int &habilitados, int &deshabilitados)
{
    Libro *libro = findLibro(nodo, id);
    if (libro)
    {
        if (libro->habilitado)
        {
            habilitados--;
            deshabilitados++;
        }
        else
        {
            habilitados++;
            deshabilitados--;
        }
        libro->habilitado = !libro->habilitado;
    }
}

void count(NodoAVL *nodo, int habilitados, int deshabilitados)
{
    cout << habilitados + deshabilitados << " " << habilitados << " " << deshabilitados << endl;
}

void liberarMemoria(NodoAVL *nodo)
{
    if (nodo == NULL)
        return;

    liberarMemoria(nodo->izq);
    liberarMemoria(nodo->der);

    delete nodo->libro;
    delete nodo;
}

int main()
{
    //  // IMPORTANTE! BORRAR O COMENTAR LAS SIGUIENTES LINEAS  EN TODOS LOS EJERCICIOS DEL OBLIGATORIO. NO PUEDEN ESTAR EN NINGUNA ENTREGA!
    //  ifstream myFile("tests/ejercicio1/100.in.txt");
    //  cin.rdbuf(myFile.rdbuf());
    //  // Si desean tirar la salida a un archivo, usen las siguientes líneas (si no, sáquenlas):
    //  ofstream myFile2("100.mine.out.txt");
    //  cout.rdbuf(myFile2.rdbuf());

    NodoAVL *arbol = NULL;
    int habilitados = 0;
    int deshabilitados = 0;

    int n;
    cin >> n;
    cin.ignore(); // Ignorar el salto de línea después del número

    for (int i = 0; i < n; i++)
    {
        string funcion;
        cin >> funcion;

        if (funcion == "ADD")
        {
            int id;
            string titulo;
            cin >> id;
            cin.ignore();
            getline(cin, titulo);

            Libro *nuevoLibro = new Libro(id, titulo);
            arbol = add(arbol, nuevoLibro, habilitados, deshabilitados);
        }
        else if (funcion == "TOGGLE")
        {
            int id;
            cin >> id;

            toggle(arbol, id, habilitados, deshabilitados);
        }
        else if (funcion == "FIND")
        {
            int id;
            cin >> id;
            if (Libro *libro = findLibro(arbol, id))
            {
                cout << libro->titulo << " " << (libro->habilitado ? "H" : "D") << endl;
            }
        }
        else if (funcion == "COUNT")
        {
            count(arbol, habilitados, deshabilitados);
        }
        else
        {
            cout << "OPCION INVALIDA | 1891" << endl;
        }
    }

    liberarMemoria(arbol); // buena practica pa

    return 0;
}