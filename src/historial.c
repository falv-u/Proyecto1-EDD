#include <stdio.h>
#include <stdlib.h>
#include "songs.h"

/* Crea un historial vacio con espacio para MAX_HISTORIAL canciones.
 * Si falla malloc queda con canciones == NULL y cantidad 0. */
playlist crear_historial(void)
{
	playlist h;

	h.pid = 0;
	h.nombre = NULL;
	h.ruta = NULL;
	h.cantidad = 0;
	h.canciones = malloc(sizeof(cancion) * MAX_HISTORIAL);
	if (h.canciones == NULL) 
	{
		fprintf(stderr, "Error: sin memoria\n");
		exit(1);
	}
	return h;
}

/* Busca un cid en el historial.
 * Retorna su posicion, o -1 si no esta. */
int buscar_en_historial(const playlist *h, uint32_t cid)
{
	int i;

	for (i = 0; i < h->cantidad; i++)
		if (h->canciones[i].cid == cid)
			return i;
	return -1;
}

/* Quita la cancion en pos y corre las siguientes una posicion
 * a la izquierda. Asume que pos es valida. */
void quitar_de_historial(playlist *h, int pos)
{
	int i;

	for (i = pos + 1; i < h->cantidad; i++)
		h->canciones[i - 1] = h->canciones[i];
	h->cantidad--;
}

/* Agrega una cancion como la mas reciente (al final).
 * Si ya estaba, se mueve al final. Si esta lleno, descarta la mas antigua.
 * Retorna 0 en exito, 1 si h o c son invalidos. */
int agregar_a_historial(playlist *h, const cancion *c)
{
	int pos;

	if (h == NULL || h->canciones == NULL || c == NULL)
		return 1;

	pos = buscar_en_historial(h, c->cid);

	if (pos != -1)
	{
		printf("Eliminada cancion N%d\n", pos+1);
		quitar_de_historial(h, pos);
	}
	else if (h->cantidad == MAX_HISTORIAL)
	{
		printf("Eliminada cancion N%d\n", pos+1);
		quitar_de_historial(h, 0);
	}

	h->canciones[h->cantidad] = *c;
	h->cantidad++;
	return 0;
}

/* Libera solo el arreglo. Los textos pertenecen al catalogo. */
void liberar_historial(playlist *h)
{
	free(h->canciones);
	h->canciones = NULL;
	h->cantidad = 0;
}
