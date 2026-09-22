#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "songs.h"
#define MAXCANCIONES 3
#define RANGO_CID 100

cancion crear_cancion(const char titulo[], const char artista[], const char album[], const char genero[])
{
    cancion cr;

    cr.cid = (rand() % RANGO_CID) + 1;
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
int main(void)
{
    srand(time(NULL));
    // reproducir_musica();

	playlist pl;

	pl.canciones = (uint32_t *)malloc(MAXCANCIONES * sizeof(uint32_t));

	/*for (i = 0; i < MAXCANCIONES; i++)
	{
	    pl.canciones->titulo[i] = (char)nombre_cancion;
	    printf("%c\n", pl.canciones->titulo[i]);
		nombre_cancion++;
		}*/

	return 0;
}
