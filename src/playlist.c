#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "songs.h"
static const char ruta_pl[] = "./playlist.csv";

int existe_csv(void)
{
	FILE *f;
	f = fopen(ruta_pl, "r");
	if (f == NULL)
	{
		return 0;
	}
	fclose(f);
	return 1;
}

/*
 * Funcion que copia una cadena de texto y devuelve el puntero a esa nueva cadena
 * usada para en 'playlist_cargar_csv' poder reutilizar buffer sin perder informacion.
 */
char *dup(const char *s)
{
	char *p = malloc(strlen(s) + 1);
	if (p)
		strcpy(p, s);
	return p;
}

int playlist_cargar_csv(playlist *pl, const char *ruta)
{
	FILE *f;
	char linea_actual[1024];
	cancion *c;
	char *t[8];
	int cap;
	int total;
	int n;

	f = fopen(ruta, "a+");

	if (f == NULL)
	{
		printf("error abriendo archivo...\n");
		return 121;
	}
	/* lee con fgets, restringido a linea_actual del tamano de linea_actual para el archivo f */
	total = 0;
	while (fgets(linea_actual, sizeof(linea_actual), f))
		if (linea_actual[0] != '\n')
			total++;
	rewind(f);
	pl->canciones = malloc(sizeof(cancion) * total);
	pl->cantidad = 0;
	cap = 0;

	while (fgets(linea_actual, sizeof(linea_actual), f))
	{
		n = 0;
		/*
		 * string tokenizer (tokenizador de strings)
		 * Divide un string en pedazos segun los caracteres que le pases como separadores.
		 * strtok(linea, "|\n") es pedir que corte esta línea cada vez que encuentres | o \n
		 *
		 */

		/*
		 * explicacion: para cierto char *p se le asigna un puntero al token
		 * y mientras el puntero p sea distinto de null y n sea menor a ocho,
		 * siendo n la cantidad de datos a extraer, para el avance se reasigna p
		 * pasandole el valor NULL a strtok para que continue donde quedo anteriormente
		 */
		for (char *p = strtok(linea_actual, "|\n"); p && n < 8; p = strtok(NULL, "|\n"))
			/* esto se lee como t[n] = p y luego n = n+1 ya que es un post-incremento */
			t[n++] = p;
		/*
		 * el for anterior solo para cuando p=NULL o n==8 es decir, si p es NULL antes de
		 * n llegar a 8 significa que ocurrio un error. al estar dentro de un while,
		 * el continue corta la iteracion actual.
		 */
		if (n < 8)
			continue;

		if (pl->cantidad == cap)
		{
			cap = cap ? cap * 2 : 16;
			pl->canciones = realloc(pl->canciones, sizeof(cancion) * cap);
		}
		/* se usa strtoul para convertir string a un unsigned long integer */
		c = &pl->canciones[pl->cantidad++];
		c->cid       = strtoul(t[0], NULL, 10);
		c->duracion  = strtoul(t[1], NULL, 10);
		c->anio      = strtoul(t[2], NULL, 10);
		c->total_rep = strtoul(t[3], NULL, 10);
		c->titulo    = dup(t[4]);
		c->artista   = dup(t[5]);
		c->album     = dup(t[6]);
		c->genero    = dup(t[7]);
	}
	fclose(f);
	return 0;
}

void liberar_pl(playlist *pl)
{
	int i;
	for (i = 0; i < pl->cantidad; i++)
	{
		free(pl->canciones[i].titulo);
		free(pl->canciones[i].artista);
		free(pl->canciones[i].album);
		free(pl->canciones[i].genero);
	}
	free(pl->canciones);
	free(pl->nombre);
	printf("se libero la playlist sin errores apartentes\n");
}
/*
 * Funcion de creado que usa funciones auxiliares para modularidad, en si esta
 * funcion solo define por si misma el id de una playlist y su nombre.
 */
playlist crear_playlist(void)
{
	playlist pl;
	char buff[124];

	pl.pid = generar_id();
	printf("agregue un nombre para la playlist: ");
	fgets(buff, sizeof(buff), stdin);
	buff[strcspn(buff, "\n")] = '\0';

	pl.nombre = malloc(strlen(buff) + 1);
	if (pl.nombre == NULL)
		exit(1);
	strcpy(pl.nombre, buff);

	if (existe_csv()) {
		playlist_cargar_csv(&pl, ruta_pl);
	}


	return pl;
}

// ui.c <-
void imprimir_playlist(const playlist *pl)
{
	int i;

	if (pl == NULL) {
		printf("playlist vacia\n");
		return;
	}

	printf("Playlist #%u: %s\n", pl->pid, pl->nombre ? pl->nombre : "(sin nombre)");
	printf("Canciones: %d\n", pl->cantidad);
	printf("----------------------------------------------------------------------\n");
	printf("%-4s %-30s %-20s %-20s %-6s %-5s %-5s\n",
			"ID", "Titulo", "Artista", "Album", "Dur", "Anio", "Rep");
	printf("----------------------------------------------------------------------\n");

	for (i = 0; i < pl->cantidad; i++) {
		const cancion *c = &pl->canciones[i];
		printf("%4u %30.30s %20.20s %20.20s %6u %-5u %-5u\n",
				c->cid,
				c->titulo  ? c->titulo  : "-",
				c->artista ? c->artista : "-",
				c->album   ? c->album   : "-",
				c->duracion,
				c->anio,
				c->total_rep);
	}
	printf("----------------------------------------------------------------------\n");
}
