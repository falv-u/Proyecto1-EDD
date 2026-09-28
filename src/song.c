#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "songs.h"

cancion crear_cancion(const char titulo[], const char artista[], const char album[], const char genero[])
{
    cancion cr;

    cr.cid = generar_id();
    cr.anio = 1999;
    cr.total_rep = 0;
    cr.duracion = 255;

    cr.titulo = (char *)malloc(strlen(titulo) + 1);
    strcpy(cr.titulo, titulo);
    cr.artista = (char *)malloc(strlen(artista) + 1);
    strcpy(cr.artista, artista);
    cr.album = (char *)malloc(strlen(album) + 1);
    strcpy(cr.album, album);
    cr.genero = (char *)malloc(strlen(genero) + 1);
    strcpy(cr.genero, genero);

    return cr;
}

void eliminar_cancion(cancion *c)
{
    free(c->titulo);
    free(c->artista);
    free(c->album);
    free(c->genero);
}

void inicializar_canciones(cancion canciones[])
{
    char titulo[100], artista[100], album[100], genero[100];
    FILE* archivo;

    archivo = fopen("./assets/list.csv", "r");
    if (archivo == NULL)
    {
        printf("Error al abrir list.csv\n");
        exit(1);
    }

    for (int i = 0; i < MAXCANCIONES; i++)
    {
        fscanf(archivo, "%[^|],%[^|],%[^|],%[^\n]\n", titulo, artista, album, genero);
        canciones[i] = crear_cancion(titulo, artista, album, genero);
    }

    fclose(archivo);
}
