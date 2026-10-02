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
