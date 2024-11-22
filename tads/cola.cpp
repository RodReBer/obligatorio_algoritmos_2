#include "../Lista/lista.cpp"

template<class T>
class Cola {
    private:
        Lista<T>* l;
    
    public:
        Cola(){
            this->l = new Lista<T>();
        }

        void Encolar(T dato){
            l->InsertarFin(dato);
        }

        T Frente() {
            return l->ObtenerPpio();
        }
        
        void Desencolar() {
            l->BorrarPpio();
        }

        int Largo() {
            return l->Largo();
        }

        bool EsVacia(){
            return l->EsVacia();
        }
};