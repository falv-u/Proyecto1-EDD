#include <stdio.h>
#include <stdlib.h>

typedef struct {
    char* titulo;
    char* artista;
    int duracion;
} Cancion;

typedef struct {
    char* nombre;
    Cancion* canciones;
    int cantidad;
} Playlist;

typedef struct {
    Playlist* playlists;
    int cantidad;
} Main;
