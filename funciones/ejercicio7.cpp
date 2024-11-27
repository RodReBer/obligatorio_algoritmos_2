// Función para verificar si un tiempo máximo dado es válido
bool esValido(int canciones[], int N, int E, int tiempoMaximo) {
    int estudiantesUsados = 1; // Comenzamos con un estudiante
    int tiempoActual = 0;

    for (int i = 0; i < N; i++) {
        if (canciones[i] > tiempoMaximo) return false; // Una canción no puede ser asignada
        if (tiempoActual + canciones[i] > tiempoMaximo) {
            estudiantesUsados++; // Necesitamos un nuevo estudiante
            tiempoActual = canciones[i]; // Empezamos con la canción actual
            if (estudiantesUsados > E) return false; // Excedimos los estudiantes disponibles
        } else {
            tiempoActual += canciones[i]; // Continuamos sumando canciones al mismo estudiante
        }
    }
    return true; // Es posible dividir las canciones
}
