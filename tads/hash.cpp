#include <iostream>
using namespace std;

template<class K, class V>
class NodoHash {
    public:
        K clave;
        V valor;
        bool susuki;
        NodoHash(K clave, V valor) : clave(clave), valor(valor), susuki(false){}
};


template<class K, class V>
class Hash {
    private:
        NodoHash<K,V>** hash;
        int cantidad;
        int largoArray;
        int maxCant;
        int (*fHash)(K);

        bool esPrimo(int num) {
            if(num <= 1) {
                return false;
            } else {
                for (int i = 2; i < num/2; i++)
                {
                    if(num%i==0) {
                        return false;
                    }
                }
                return true;
            }
        }

        int primoSup(int num){
            while(!esPrimo(++num));
            return num;
            // 302782: Pedro Chialanza
        }

    public:
        Hash(int maxCant, int (*fHash)(K)) {
            this->cantidad = 0;
            this->maxCant = maxCant;
            this->largoArray = primoSup(maxCant*2);
            this->hash = new NodoHash<K,V>*[this->largoArray]();
            this->fHash = fHash;
        }   

        ~Hash() {
            for (int i = 0; i < largoArray; i++)
            {
                if(hash[i]){
                    delete hash[i];
                }
            }
            
            delete [] this->hash;
        }   

        // Pre: !EsLleno()
        void Insertar(K clave, V valor) {
            int posHash = abs(fHash(clave)) % largoArray;
            while(hash[posHash] && !hash[posHash]->susuki){
                posHash = (posHash + 1) % largoArray;
            }
            if(hash[posHash]){
                delete hash[posHash];
            }
            hash[posHash] = new NodoHash<K, V>(clave, valor);
            cantidad++;
        }

        // Pre: Existe(clave)
        void Eliminar(K clave) {
            int posHash = abs(fHash(clave)) % largoArray;
            while(hash[posHash]->clave != clave){
                posHash = (posHash + 1) % largoArray;
            }
            hash[posHash]->susuki = true;
        }

        // Pre: Existe(clave)
        V Recuperar(K clave) {
            int posHash = abs(fHash(clave)) % largoArray;
            while(hash[posHash]->clave!=clave){
                posHash = (posHash + 1) % largoArray;
            }
            return hash[posHash]->valor;
        }

        bool Existe(K clave) {
            int posHash = abs(fHash(clave)) % largoArray;
            while(hash[posHash] && hash[posHash]->clave!=clave){
                posHash = (posHash + 1) % largoArray;
            }
            return hash[posHash] && !hash[posHash]->susuki;
        }

        void Inspeccionar(){
            for (int i = 0; i < largoArray; i++)
            {
                cout << i <<":";
                if(!hash[i]){
                    cout << "<VACIO>";
                } else if(hash[i]->susuki) {
                    cout << "<BANIO URGENTE>";
                } else {
                    cout << "K: " << hash[i]->clave << " - V: " << hash[i]->valor;
                }
                cout << endl;
            }
            
        }

};

// Ref: https://cseweb.ucsd.edu/~kube/cls/100/Lectures/lec16/lec16-15.html
int fHash(string key) {
  int h = 0;
  for (int i = 0; i < key.length(); i++)
    h = 31 * h + int(key[i]);
  return h;
}

int main() {
    int largo, ret = 0;
    string palabra;
    cin >> largo;
    Hash<string, int>* hash = new Hash<string, int>(largo, fHash);
    for (int i = 0; i < largo; i++)
    {
        cin >> palabra;
        bool existePalabra = hash->Existe(palabra);
        if(!existePalabra){
            hash->Insertar(palabra, 1);
        } else {
            int ocurrencias = hash->Recuperar(palabra);
            if(ocurrencias == 1) {
                ret++;
            } else if (ocurrencias == 2) {
                ret--;
            }
            hash->Eliminar(palabra);
            hash->Insertar(palabra, ocurrencias+1);
        }
    }
    cout << ret;
}