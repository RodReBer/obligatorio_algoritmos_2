
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
