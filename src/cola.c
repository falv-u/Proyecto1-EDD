#include <stdio.h>
#include <stdlib.h>
#include "songs.h"

/* Busca un cid. Retorna su posicion, o -1 si no esta. */
int buscar_cid_playlist(const playlist *pl, uint32_t cid)
{
	int i;

	for (i = 0; i < pl->cantidad; i++)
		if (pl->canciones[i].cid == cid)
			return i;
	return -1;
}

/* Quita la cancion en pos y corre las siguientes a la izquierda. */
void quitar_de_playlist(playlist *pl, int pos)
{
	int i;

	for (i = pos + 1; i < pl->cantidad; i++)
		pl->canciones[i - 1] = pl->canciones[i];
	pl->cantidad--;
}

/* Crea un playlist vacio con espacio para capacidad canciones.
 * Si falla malloc queda con canciones == NULL. */
playlist crear_playlist_vacia(int capacidad)
{
	playlist pl;

	pl.pid = 0;
	pl.nombre = NULL;
	pl.ruta = NULL;
	pl.cantidad = 0;
	pl.canciones = malloc(sizeof(cancion) * capacidad);
	return pl;
}

/* Libera solo el arreglo (los textos son del catalogo). */
void liberar_arreglo_playlist(playlist *pl)
{
	free(pl->canciones);
	pl->canciones = NULL;
	pl->cantidad = 0;
}

/* Agrega una cancion al comienzo. Falla si hay argumentos invalidos,
 * si ya estaba o si la cola esta llena. */
int agregar_a_cola(playlist *cola, const cancion *c)
{
	int i;

	if (cola == NULL || cola->canciones == NULL || c == NULL)
		return 1;
	if (cola->cantidad == MAX_COLA || buscar_cid_playlist(cola, c->cid) != -1)
		return 1;

	for (i = cola->cantidad; i > 0; i--)
		cola->canciones[i] = cola->canciones[i - 1];
	cola->canciones[0] = *c;
	cola->cantidad++;
	return 0;
}

/* Quita la cancion en pos (0 = primera). Falla si pos es invalida. */
int quitar_de_cola_pos(playlist *cola, int pos)
{
	if (cola == NULL || pos < 0 || pos >= cola->cantidad)
		return 1;
	quitar_de_playlist(cola, pos);
	return 0;
}

/* Quita la cancion con ese cid. Falla si no esta. */
int quitar_de_cola_id(playlist *cola, uint32_t cid)
{
	if (cola == NULL)
		return 1;
	return quitar_de_cola_pos(cola, buscar_cid_playlist(cola, cid));
}

/* Deja la cola vacia. */
void vaciar_cola(playlist *cola)
{
	if (cola != NULL)
		cola->cantidad = 0;
}

/* Saca la primera de la cola, sube su total_rep en el catalogo y la
 * agrega al historial. Falla si la cola esta vacia o la cancion
 * ya no existe en el catalogo. */
int reproducir_de_cola(playlist *cola, playlist *catalogo, playlist *historial)
{
	int pos;

	if (cola == NULL || catalogo == NULL || historial == NULL || cola->cantidad == 0)
		return 1;

	pos = buscar_cid_playlist(catalogo, cola->canciones[0].cid);
	quitar_de_playlist(cola, 0);
	if (pos == -1)
		return 1;

	catalogo->canciones[pos].total_rep++;
	agregar_a_historial(historial, &catalogo->canciones[pos]);
	printf("reproduciendo: %s - %s\n",
			catalogo->canciones[pos].artista, catalogo->canciones[pos].titulo);
	return 0;
}
