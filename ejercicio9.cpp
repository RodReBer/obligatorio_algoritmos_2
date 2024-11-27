#include <iostream>
#include <string>
#include <cassert>
#include "funciones/ejercicio9.cpp"
using namespace std;

int main()
{
    int J;
    cin >> J;
    assert(J > 0 && J <= 1000); // Validamos el número de jugadores

    // Declaramos los datos de los jugadores
    int ritmo[J + 1], tiro[J + 1], pase[J + 1], regate[J + 1], defensa[J + 1], fisico[J + 1], sueldo[J + 1];
    string forma[J + 1], extranjero[J + 1], confianza[J + 1];

    for (int i = 0; i < J; i++)
    {
        cin >> ritmo[i] >> tiro[i] >> pase[i] >> regate[i] >> defensa[i] >> fisico[i] >> forma[i] >> sueldo[i] >> extranjero[i] >> confianza[i];
    }

    int P, E;
    cin >> P >> E;
    assert(P >= 1 && P <= 100000);
    assert(E >= 0 && E <= 11);

    int K = 11; // Queremos seleccionar exactamente 11 jugadores

    // Crear tabla DP de 4 dimensiones
    int ****dp = new int ***[J + 1];
    for (int j = 0; j <= J; j++)
    {
        dp[j] = new int **[K + 1];
        for (int k = 0; k <= K; k++)
        {
            dp[j][k] = new int *[P + 1];
            for (int p = 0; p <= P; p++)
            {
                dp[j][k][p] = new int[E + 1];
                for (int e = 0; e <= E; e++)
                {
                    dp[j][k][p][e] = -1; // Inicializar manualmente con -1 (valor imposible)
                }
            }
        }
    }

    // Caso base
    dp[0][0][0][0] = 0; // Si no hemos seleccionado jugadores, la valoración es 0.

    // Programación dinámica
    for (int j = 1; j <= J; j++)
    {
        int valoracion = calcularValoracion(ritmo[j - 1], tiro[j - 1], pase[j - 1], regate[j - 1],
                                            defensa[j - 1], fisico[j - 1], forma[j - 1], confianza[j - 1]);
        int costo = sueldo[j - 1];
        int esExtranjero = (extranjero[j - 1] == "Si");

        for (int k = 0; k <= K; k++)
        {
            for (int p = 0; p <= P; p++)
            {
                for (int e = 0; e <= E; e++)
                {
                    // Caso sin incluir al jugador
                    if (dp[j - 1][k][p][e] != -1)
                    {
                        dp[j][k][p][e] = max(dp[j][k][p][e], dp[j - 1][k][p][e]);
                    }

                    // Caso incluyendo al jugador
                    if (k > 0 && p >= costo && e >= esExtranjero && dp[j - 1][k - 1][p - costo][e - esExtranjero] != -1)
                    {
                        dp[j][k][p][e] = max(dp[j][k][p][e],
                                             dp[j - 1][k - 1][p - costo][e - esExtranjero] + valoracion);
                    }
                }
            }
        }
    }

    // Obtener el mejor resultado para exactamente 11 jugadores
    int mejorValoracion = -1;
    for (int p = 0; p <= P; p++)
    {
        for (int e = 0; e <= E; e++)
        {
            mejorValoracion = max(mejorValoracion, dp[J][K][p][e]);
        }
    }

    if (mejorValoracion == -1)
        cout << 0 << endl; // No es posible formar el equipo
    else
        cout << mejorValoracion / 11 << endl; // Promedio del mejor once inicial

    // Liberar memoria
    for (int j = 0; j <= J; j++)
    {
        for (int k = 0; k <= K; k++)
        {
            for (int p = 0; p <= P; p++)
            {
                delete[] dp[j][k][p];
            }
            delete[] dp[j][k];
        }
        delete[] dp[j];
    }
    delete[] dp;

    return 0;
}
