#pragma once

template<class T>
class nodoLista {
    public:
        T dato;
        nodoLista<T> * sig;
        nodoLista<T> * ant;

        nodoLista(T dato, nodoLista<T>* ant, nodoLista<T>* sig):dato(dato), sig(sig), ant(ant){}
        nodoLista(T dato):dato(dato), sig(NULL), ant(NULL){}
};