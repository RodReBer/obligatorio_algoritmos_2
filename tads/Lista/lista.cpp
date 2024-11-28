#include <iostream>
#include "nodolista.cpp"
#include "iteradorlista.cpp"
#pragma once
using namespace std;

template<class T>
class Lista {
    private:
        nodoLista<T>* ppio;
        nodoLista<T>* fin;
        int largo;

        void MostrarRec(nodoLista<T>* nodo, void (*func)(T)){
            if(nodo != NULL){
                func(nodo->dato);
                MostrarRec(nodo->sig, func);
            }
        }

        bool ExisteRec(T elem, nodoLista<T>* nodo){
            return nodo && (nodo->dato == elem || ExisteRec(elem, nodo->sig));
        }

        // void InsertarFinRec(nodoLista*& nodo, int dato){
        //     if(nodo == NULL){
        //         nodo = new nodoLista(dato);
        //         largo++;
        //     } else {
        //         InsertarFinRec(nodo->sig, dato);
        //     }
        // }

        void VaciarRec(nodoLista<T>* &nodo){
            if(nodo!=NULL){
                VaciarRec(nodo->sig);
                delete nodo;
                nodo = NULL;
            }
        }
        
    public:
        Lista(): ppio(NULL), fin(NULL), largo(0){};

        ~Lista(){
            VaciarRec(ppio);
        };        

        void InsertarPpio(T dato) {
            ppio = new nodoLista<T>(dato, NULL, ppio);
            if(fin == NULL){
                fin = ppio;
            } else {
                ppio -> sig -> ant = ppio;
            }
            largo++;
        }

        void InsertarFin(T dato) {
            fin = new nodoLista<T>(dato, fin, NULL);
            if(ppio == NULL){
                ppio = fin;
            } else {
                fin -> ant -> sig  = fin;
            }
            largo++;
        }

        T* ToArray(){
            T* ret = new T[largo];
            nodoLista<T>* aux = ppio;
            int cont = 0;
            while(cont < largo){
                ret[cont++] = aux->dato;
                aux = aux->sig;
            }
            return ret;
        }

        // void InsertarFin(int dato) {
        //     if(ppio == NULL){
        //         InsertarPpio(dato);
        //     } else {
        //         nodoLista* nuevo = new nodoLista(dato);
        //         nodoLista* aux = ppio;
        //         while(aux->sig != NULL){
        //             aux = aux->sig;
        //         }
        //         aux->sig = nuevo;
        //         largo++;
        //     }
        // }

        // void InsertarFinTurbio(int dato) {
        //     nodoLista* nuevo = new nodoLista(dato);
        //     nodoLista** aux = &ppio;
        //     while(*aux != NULL){
        //         aux = &((*aux)->sig);
        //     }
        //     *aux = nuevo;
        //     largo++;
        // }

        void MostrarI(){
            nodoLista<T>* aux = ppio;
            while(aux != NULL){
                cout << aux->dato << endl;
                aux = aux->sig;
            }
        }

        void MostrarR(void (*func)(T)){
            MostrarRec(ppio, func);
            cout << endl;
        }

        // void InsertarFinR(int dato){
        //     InsertarFinRec(ppio, dato);
        // }

        // Pre: !EsVacia()
        T ObtenerPpio() {
            return ppio->dato;
        }

        // Pre: !EsVacia()
        T ObtenerFin() {
            return fin->dato;
        }

        // Pre: !EsVacia()
        void BorrarPpio(){
            nodoLista<T>* aBorrar = ppio;
            ppio = ppio->sig;
            delete aBorrar;
            if(ppio == NULL){
                fin = NULL;
            } else {
                ppio -> ant = NULL;
            }
            largo--;
        }

        // Pre: !EsVacia()
        void BorrarFin(){
            nodoLista<T>* aBorrar = fin;
            fin = fin->ant;
            delete aBorrar;
            if(fin == NULL){
                ppio = NULL;
            } else {
                fin -> sig = NULL;
            }
            largo--;
        }

        int Largo() {
            return largo;
        }

        bool EsVacia() {
            return ppio == NULL;
        }

        void Vaciar(){
            VaciarRec(ppio);
        }

        bool Existe(T elem){
            return ExisteRec(elem, ppio);
        }

        IteradorLista<T>* obtenerIterador(){
            return new IteradorLista<T>(ppio);
        }

        Lista<T>* Clon() {
            Lista<T>* l = new Lista<T>();
            nodoLista<T>* aux = ppio;
            while(aux){
                l->InsertarFin(aux->dato);
                aux=aux->sig;
            }
            return l;
        }
};

void hacerCOut(int dato){
    cout << "Mi dato es: " << dato << endl;
}

// int main(){
//     Lista<int>* l = new Lista<int>();
//     l->InsertarPpio(5);
//     l->InsertarPpio(4);
//     l->InsertarPpio(3);
//     l->InsertarPpio(2);
//     l->InsertarPpio(1);

//     // Lista<string>* lStr = new Lista<string>();
//     // lStr->InsertarPpio("Rigoberta Menchú 1");
//     // lStr->InsertarPpio("Rigoberta Menchú 2");
//     // lStr->InsertarPpio("Rigoberta Menchú 3");
//     // lStr->InsertarPpio("Rigoberta Menchú 4");
//     // lStr->InsertarPpio("Rigoberta Menchú 5");

//     for(IteradorLista<int>* it = l->obtenerIterador(); it->HayElemento(); it->Avanzar()){
//         cout << "Holi " << it->Elemento() << endl;
//     }
//     //lStr->MostrarR();

//     delete l;

//     return 0;
// }   