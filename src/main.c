#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "songs.h"

int main(void)
{
    srand(time(NULL));
    // reproducir_musica();

	/*playlist pl;

	pl.canciones = (uint32_t *)malloc(MAXCANCIONES * sizeof(uint32_t));*/

	/*cancion c1 = crear_cancion("Creep", "Radiohead", "Pablo Honey", "Rock alternativo");
	cancion c2 = crear_cancion("Myths You Forgot", "Camellia (feat. Toby Fox)", "U.U.F.O", "Future bass");
	cancion c3 = crear_cancion("Headlock", "Imogen Heap", "Speak For Yourself (Deluxe Edition)", "Art pop");

	printf("%d\t%d\t%d\n", c1.cid, c2.cid, c3.cid);
	printf("%d\t%d\t%d\n", c1.duracion, c2.duracion, c3.duracion);
	printf("%u\t%u\t%u\n", c1.anio, c2.anio, c3.anio);
	printf("%u\t%u\t%u\n", c1.total_rep, c2.total_rep, c3.total_rep);
	printf("%s\t%s\t%s\n", c1.titulo, c2.titulo, c3.titulo);
	printf("%s\t%s\t%s\n", c1.artista, c2.artista, c3.artista);
	printf("%s\t%s\t%s\n", c1.album, c2.album, c3.album);
	printf("%s\t%s\t%s\n", c1.genero, c2.genero, c3.genero);

	eliminar_cancion(&c1);
	eliminar_cancion(&c2);
	eliminar_cancion(&c3);*/

	/*for (i = 0; i < MAXCANCIONES; i++)
	{
	    pl.canciones->cid[i] = (char)nombre_cancion;
	    printf("%c\n", pl.canciones->titulo[i]);
		nombre_cancion++;
		}*/

	return 0;
}
