#include <iostream>
#include <fstream>
#include "./cola.cpp"
#include "./heap.cpp"
#define INF 99999
using namespace std;

template<class T1, class T2>
class Par {
    public:
        T1 elem1;
        T2 elem2;

        Par(T1 elem1, T2 elem2): elem1(elem1), elem2(elem2) {}
};

class Arista {
    public:
        int costo;
        bool existe;

        Arista() : costo(0), existe(false) {}
};

class Grafo {
    private:
        int** vertices;
        Arista* ** matAdy;
        int tope;
        int cantVertices;

        int primerHueco() {
            for (int i = 0; i < tope; i++)
            {
                if(!vertices[i])
                {
                    return i;
                }
            }
            return -1;
        }

        int posVertice(int v){
            for (int i = 0; i < tope; i++)
            {
                if(vertices[i] && *vertices[i] == v){
                    return i;
                }
            }
            return -1;
        }

        void DFSRec(int pos, bool*& vis){
            cout << *vertices[pos] << " ";
            vis[pos] = true;
            for (int j = 0; j < tope; j++)
            {
                if(!vis[j] && matAdy[pos][j]->existe){
                    DFSRec(j, vis);
                }
            }
        }

        int ObtenerPosSinIncidencias(int* cantIncidencias, bool* vis){
            for (int i = 0; i < tope; i++)
            {
                if(cantIncidencias[i]==0 && vertices[i] && !vis[i]){
                    return i;
                }
            }
            return -1;
        }

        int posMasCercanoNoVis(int* dist, bool* vis){
            int posMin = -1, min= INF;
            for (int i = 0; i < tope; i++)
            {
                if(!vis[i] && dist[i] < min)
                {
                    min = dist[i];
                    posMin = i;
                }
            }
            return posMin;
        }

        template<class T>
        void mostrarVec(T*vec, int largo){
            cout << "[";
            for (int i = 0; i < largo-1; i++)
            {
                cout << vec[i] << ", ";
            }
            cout << vec[largo-1] << "]" << endl;
        }

    public:

        Grafo(int tope, bool esDirigido = true) {
            this->tope = tope;
            this->cantVertices = 0;
            this->vertices = new int*[tope]();
            if(esDirigido){
                this->matAdy = new Arista* *[tope];
                for (int i = 0; i < tope; i++)
                {
                    this->matAdy[i] = new Arista*[tope];
                    for (int j = 0; j < tope; j++)
                    {
                        this->matAdy[i][j] = new Arista();
                    }
                }
            } else {
                this->matAdy = new Arista* *[tope];
                for (int i = 0; i < tope; i++)
                {
                    this->matAdy[i] = new Arista*[tope];
                }
                for (int i = 0; i < tope; i++)
                {
                    for (int j = i; j < tope; j++)
                    {
                        matAdy[i][j] = matAdy[j][i] = new Arista();
                    }
                }
            }
        }

        // Pre: !EstaLleno()
        void AgregarVertice(int v){
            int pos = primerHueco();
            vertices[pos] = new int(v);
            cantVertices++;
        }

        Lista<int>* ObtenerAdyacentes(int v){
            Lista<int>* ret = new Lista<int>();
            int pos = posVertice(v);
            for (int j = 0; j < tope; j++)
            {
                if(matAdy[pos][j]->existe){
                    ret->InsertarFin(*vertices[j]);
                }
            }
            return ret;
        }

        bool ExisteVertice(int v){
           return posVertice(v) != -1;
        }

        // Pre: Existe(v)
        void BorrarVertice(int v){
            int pos = posVertice(v);
            delete vertices[pos];
            vertices[pos] = NULL;
            for (int i = 0; i < tope; i++)
            {
                matAdy[i][pos]->existe = false;
            }
            cantVertices--;
        }

        // Pre: ExisteVertice(vOri) && ExisteVertice(vDes)
        void AgregarArista(int vOri, int vDes, int costo){
            int posO = posVertice(vOri);
            int posD = posVertice(vDes);
            matAdy[posO][posD]->costo = costo;
            matAdy[posO][posD]->existe = true;
        }

        // Pre: ExisteVertice(vOri) && ExisteVertice(vDes)
        bool ExisteArista(int vOri, int vDes){
            int posO = posVertice(vOri);
            int posD = posVertice(vDes);
            return matAdy[posO][posD]->existe;
        }

        // Pre: ExisteArista(vOri,vDes)
        void BorrarArista(int vOri, int vDes){
            int posO = posVertice(vOri);
            int posD = posVertice(vDes);
            matAdy[posO][posD]->existe = false;
        }

        bool EstaLleno() {
            return tope == cantVertices;
        }

        // Pre: ExisteVertice(v)
        void DFS(int v){
            int pos = posVertice(v);
            bool*vis = new bool[tope]();
            DFSRec(pos, vis);
            delete[] vis;
            cout << endl;
        }
        
        // Pre: ExisteVertice(v)
        void BFS(int v){
            bool*vis = new bool[tope]();
            Cola<int>*cola = new Cola<int>();
            int pos = posVertice(v);
            cola->Encolar(pos);
            vis[pos]=true;
            while(!cola->EsVacia()){
                int proxPos = cola->Frente();
                cola->Desencolar();
                cout << *vertices[proxPos] << endl;
                for (int j = 0; j < tope; j++)
                {
                    if(!vis[j] && matAdy[proxPos][j]->existe) {
                        cola->Encolar(j);
                        vis[j]=true;
                    }
                }
            }
        }

        void OrdenTopologico(){
            int*cantIncidencias = new int[tope]();
            bool*vis=new bool[tope]();
            for (int j = 0; j < tope; j++)
            {
                for (int i = 0; i < tope; i++)
                {
                    if(matAdy[i][j]->existe){
                        cantIncidencias[j]++;
                    }
                }
            }
            int verticesRestantes = cantVertices;
            while(verticesRestantes > 0){
                int pos = ObtenerPosSinIncidencias(cantIncidencias, vis);
                if(pos == -1){
                    cout << "NOOOOOO! CICLOOOO!" << endl;
                    break;
                } else {
                    cout << *vertices[pos] << endl;
                    vis[pos]=true;
                    for (int j = 0; j < tope; j++)
                    {
                        if(matAdy[pos][j]->existe){
                            cantIncidencias[j]--;
                        }
                    }
                }
                verticesRestantes--;
            }            
        }

        void Dijkstra(int vOri, int vDes){
            int posOri = posVertice(vOri);
            int posDes = posVertice(vDes);

            int* dist=new int[tope];
            bool* vis=new bool[tope];
            int* ant=new int[tope];

            // Inicializo vectores
            for (int i = 0; i < tope; dist[i]=INF, vis[i]=false, ant[i]=-1, i++);

            dist[posOri]=0;

            for (int k = 0; k < cantVertices; k++)
            {
                // 1er paso: Busco vértice de menor distancia no visitado
                int candidato = posMasCercanoNoVis(dist, vis);
                if(candidato == -1){
                    break;
                }
                // 2do paso: Visito al vértice, marcando su distancia
                vis[candidato] = true;

                // 3er paso: Evalúo y actualizo sus adyacentes
                for (int j = 0; j < tope; j++)
                {
                    if(!vis[j] && matAdy[candidato][j]->existe){
                        int nuevaDist = dist[candidato] + matAdy[candidato][j]->costo;
                        if(nuevaDist < dist[j]){
                            dist[j] = nuevaDist;
                            ant[j] = candidato;
                        }
                    }
                }
            }
            cout << "Dist:" << endl;
            mostrarVec(dist, tope);
            cout << "Ant:" << endl;
            mostrarVec(ant, tope);
            cout << "Vis:" << endl;
            mostrarVec(vis, tope);
            
            Lista<int>* l = new Lista<int>();
            l->InsertarPpio(*vertices[posDes]);
            int aux = posDes;
            while(ant[aux]!=-1){
                aux = ant[aux];
                l->InsertarPpio(*vertices[aux]);
            }
            l->MostrarI();
            
            delete l;
            delete[] dist;
            delete[] ant;
            delete[] vis;
        }

        void Kruskal() {
            Lista<int>** compConexas=new Lista<int>*[tope];
            for (int i = 0; i < tope; i++)
            {
                compConexas[i] = new Lista<int>();
                compConexas[i]->InsertarPpio(i);
            }
            Heap<Par<int, int>*> * heap = new Heap<Par<int, int>*>(tope);
            for (int i = 0; i < tope; i++)
            {
                for (int j = 0; j < tope; j++)
                {
                    if(matAdy[i][j]->existe){
                        heap->Insertar(matAdy[i][j]->costo, new Par<int, int>(i,j));
                    }
                } 
            }
            int cantAristas = 0;
            while(cantAristas < cantVertices-1){
                Par<int, int>* minArista = heap->ObtenerMin();
                heap->BorrarMin();
                int posO=-1, posD=-1;
                for (int i = 0; i < tope; i++)
                {
                    if(compConexas[i]->Existe(minArista->elem1)){
                        posO = i;
                    }
                    if(compConexas[i]->Existe(minArista->elem2)){
                        posD = i;
                    }
                }
                if(posO != posD){
                    cout << minArista->elem1 << " <-> " << minArista->elem2 << " = " << matAdy[minArista->elem1][minArista->elem2]->costo << endl;
                    cantAristas++;
                    Lista<int>* origen, *destino;
                    if(compConexas[posO]->Largo()<= compConexas[posD]->Largo()){
                        origen = compConexas[posO];
                        destino = compConexas[posD];
                    } else {
                        origen = compConexas[posD];
                        destino = compConexas[posO];
                    }
                    for(IteradorLista<int>* it = origen->obtenerIterador(); it->HayElemento(); it->Avanzar()){
                        destino->InsertarFin(it->Elemento());
                    }
                    origen->Vaciar();
                }
            }
        }

};

void DFSRec(int pos, int tope, bool*&vis, Grafo* g){
    vis[pos] = true;
    Lista<int>* ady = g->ObtenerAdyacentes(pos);
    for (int j = 1; j <= tope; j++)
    {
        if(!vis[j] && ady->Existe(j)){
            DFSRec(j, tope, vis, g);
        }
    }
    delete ady;
}

void ejP20201029NocPuentes(Grafo* g, int tope, int a){
    for (int i = 1; i <= tope; i++)
    {
        Lista<int>* ady = g->ObtenerAdyacentes(i);
        for(IteradorLista<int>* it = ady->obtenerIterador(); it->HayElemento(); it->Avanzar()){
            g->BorrarArista(i, it->Elemento());
            bool*vis = new bool[tope+1]();
            DFSRec(i,tope, vis, g);
            for (int j = 1; j <= tope; j++)
            {
                if(!vis[j] && i < it->Elemento()){
                    cout << i << " - " << it->Elemento() << endl;
                    break;
                }
            }
            delete[] vis;
            g->AgregarArista(i, it->Elemento(), 1);
        }
        delete ady;
    }    
}

int main() {
    ifstream myFile("Grafo/GrafoMA/ej20201029NocPuentes.in.txt");
    cin.rdbuf(myFile.rdbuf());
    // Si desean tirar la salida a un archivo, usen las siguientes líneas (si no, sáquenlas):
    ofstream myFile2("Grafo/GrafoMA/ej20201029NocPuentes.out.txt");
    cout.rdbuf(myFile2.rdbuf());

    int tope;
    cin >> tope;
    Grafo* g = new Grafo(tope, false);
    for (int i = 0; i < tope; i++)
    {
        int v;
        cin >> v;
        g->AgregarVertice(v);
    }
    int cantAristas, vOri, vDes, costo;
    cin >> cantAristas;
    for (int i = 0; i < cantAristas; i++)
    {
        cin >> vOri >> vDes >> costo;
        g->AgregarArista(vOri, vDes, costo);
    }
    
    ejP20201029NocPuentes(g, tope, cantAristas);
    
    return 0;
}