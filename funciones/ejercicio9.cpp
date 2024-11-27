// Función para calcular la valoración de un jugador
int calcularValoracion(int ritmo, int tiro, int pase, int regate, int defensa, int fisico,
                       string forma, string confianza)
{
    int valoracion = (ritmo + tiro + pase + regate + defensa + fisico) / 6;

    if (forma == "Lesionado")
        return 0;
    if (forma == "Mala")
        valoracion -= 5;
    else if (forma == "Buena")
        valoracion += 5;

    if (confianza == "Baja")
        valoracion -= 5;
    else if (confianza == "Alta")
        valoracion += 5;

    // Ajustar valores fuera del rango [0, 99]
    if (valoracion < 0)
        valoracion = 0;
    if (valoracion > 99)
        valoracion = 99;

    return valoracion;
}
