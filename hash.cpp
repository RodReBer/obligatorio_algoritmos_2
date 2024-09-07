#include <iostream>

using namespace std;

template <class T>
class NodoHash
{
public:
	T dato;
	int cantidadElem;
	int puntos;
	NodoHash<T> *sig;
	NodoHash(T dato, int cantidadElem, NodoHash<T> *sig) : dato(dato), cantidadElem(cantidadElem), puntos(0), sig(sig) {}
	NodoHash(T dato) : dato(dato), cantidadElem(0), puntos(0), sig(NULL) {}
	~NodoHash(){
		if(sig){
			delete sig;
		}
	}
};

template <class T>
class Hash
{
private:
	NodoHash<T> **vec;
	int cota;
	int cantidad;
	int (*funcionHash)(T, int); 
	void borrarh(NodoHash<T> *&l, T i)
	{
		if (l != NULL)
		{
			if (l->dato == i)
			{
				NodoHash<T> *aBorrar = l;
				l = l->sig;
				delete aBorrar;
			}
			else
			{
				borrarh(l->sig, i);
			}
		}
	}

public:
	Hash(int cota, int (*func)(T, int))
	{
		this->cota = cota;
		this->funcionHash = func;
		this->cantidad = 0;
		this->vec = new NodoHash<T> *[cota];
		for (int i = 0; i < cota; vec[i++] = NULL)
			;
	}

	~Hash()
	{
		for (int i = 0; i < cota; i++){
			if(vec[i]){
				delete vec[i];
			}	
		}
		cota = 0;
		delete[] vec;
	}

	void agregar(Hash <T>* &d, T dato)
	{
		if (!pertenece(d, dato))
		{
			NodoHash<T> *nuevo = new NodoHash<T>(dato);
			// nuevo->dato = dato;
			nuevo->sig = d->vec[d->funcionHash(dato, d->cota)];
			d->vec[d->funcionHash(dato, d->cota)] = nuevo;
			d->cantidad++;
		}
	}

	void borrar(Hash<T>* &d, T dato)
	{
		if (pertenece(d, dato))
		{
			borrarh(d->vec[d->funcionHash(dato, d->cota)], dato);
			d->cantidad--;
		}
	}

	bool pertenece(Hash<T> *d, T dato)
	{
		NodoHash<T> *h = d->vec[d->funcionHash(dato, d->cota)];
		while (h != NULL && h->dato != dato)
		{
			h = h->sig;
		}
		return h != NULL;
	}

	NodoHash<T>* elemento(Hash<T> * &d, T dato)
	{
		if (pertenece(d, dato)){
			NodoHash<T> *h = d->vec[d->funcionHash(dato, d->cota)];
			while (h != NULL && h->dato != dato)
			{
				h = h->sig;
			}
			if(h != NULL){
			return h;
			}
		}
		return NULL;
	}

//Pre: pertenece(Hash<T> *d, T dato)
	void agregarOcurrencia(Hash<T> *&d, T dato){
		NodoHash<T> *h = d->vec[d->funcionHash(dato, d->cota)];
		h->cantidadElem++;
	}
//Pre: pertenece(Hash<T> *d, T dato)
	void eliminarOcurrencia(Hash<T> *&d, T dato){
		NodoHash<T> *h = d->vec[d->funcionHash(dato, d->cota)];
		h->cantidadElem--;
	}

	int cantOcurrencias(Hash<T> *&d, T dato){
	NodoHash<T> *h = d->vec[d->funcionHash(dato, d->cota)];
	return h->cantidadElem;
	}

	bool esVacio(Hash<T> *d)
	{
		return (d->cantidad == 0);
	}

	unsigned int cantidadElementos(Hash<T> *d)
	{
		return d->cantidad;
	}

	Hash* clon(Hash <T>* d, int (*func)(T, int))
	{
		Hash<T> *ret = new Hash<T>(d->cota, func);
		for (int i = 0; i < ret->cota; i++)
		{
			NodoHash<T> *alias = d->vec[i];
			while (alias != NULL)
			{
				agregar(ret, alias->dato);
				alias = alias->sig;
			}
		}
		return ret;
	}

	int mostrar(Hash <T>*d)
	{
		if (!esVacio(d))
		{
			for (int i = 0; i < d->cota; i++)
			{
				if (d->vec[i] != NULL)
				{
					cout << d->vec[i]->dato << endl;
				}
			}
		}
		return 0;
	}
	/*void destruir(Hash& d) {
		for (int i = 0; i <d->cota; i++)
			destruirh(d->vec[i]);
		delete[] d->vec;
		delete d;
	}*/
};

int fun(int num, int cota)
{
	return num % cota;
}

int main1()
{
	Hash<int> *h = new Hash<int>(8, fun);

	h->agregar(h, 1);
	h->agregar(h, 2);
	h->agregar(h, 3);
	h->agregar(h, 4);
	h->agregar(h, 5);
	h->agregar(h, 6);
	h->agregar(h, 7);
	h->mostrar(h);
	NodoHash<int>* l=h->elemento(h, 3);
	cout << "cantidad de 3 :" ;
	cout << l->cantidadElem << endl;
	l->cantidadElem+=22;
	cout << "cantidad de 3 modifcado con + 22 :" ;
	cout << l->cantidadElem << endl;
	cout << "l es :" << endl;
	cout <<  l->dato << endl;
	if (h->pertenece(h, 2))
	{
		cout << "ok" << endl;
	}
	h->borrar(h, 2);
	if (h->pertenece(h, 2))
	{
		cout << "Si esta el 2" << endl;
	}
	else
	{
		cout << "No esta el 2" << endl;
	}
	h->mostrar(h);
	cout << "esp" << endl;
	Hash<int> *h2 = h->clon(h, fun);
	h->mostrar(h2);
	cout << "agrego 2" << endl;
	h->agregar(h2,2);
	h->mostrar(h2);
	cout << "hola" << endl;
	h->mostrar(h);

	return 0;
};