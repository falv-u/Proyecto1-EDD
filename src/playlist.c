#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "songs.h"

int existe_plcsv(void)
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
 * usada en 'playlist_cargar_csv' poder reutilizar buffer sin perder informacion.
 */
char *copiar_cadena(const char *s)
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
	int total;
	int n;

	f = fopen(ruta, "r");

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
	rewind(f); /* vuelve al inicio del archivo */

	pl->canciones = malloc(sizeof(cancion) * total);
	pl->cantidad = 0;

	while (fgets(linea_actual, sizeof(linea_actual), f))
	{
		n = 0;
		/*
		 * string tokenizer (tokenizador de strings)
		 * Divide un string en pedazos segun los caracteres que le pases como separadores.
		 * strtok(linea, ";\n") es pedir que corte esta línea cada vez que encuentres ; o \n
		 *
		 */

		/*
		 * explicacion: para cierto char *p se le asigna un puntero al token
		 * y mientras el puntero p sea distinto de null y n sea menor a ocho,
		 * siendo n la cantidad de datos a extraer, para el avance se reasigna p
		 * pasandole el valor NULL a strtok para que continue donde quedo anteriormente
		 */
		for (char *p = strtok(linea_actual, ";\n"); p && n < 8; p = strtok(NULL, ";\n"))
			/* esto se lee como t[n] = p y luego n = n+1 ya que es un post-incremento */
			t[n++] = p;
		/*
		 * el for anterior solo para cuando p=NULL o n==8 es decir, si p es NULL antes de
		 * n llegar a 8 significa que ocurrio un error. al estar dentro de un while,
		 * el continue corta la iteracion actual.
		 */
		if (n < 8)
			continue;

		/* se usa strtoul para convertir string a un unsigned long integer */
		c = &pl->canciones[pl->cantidad++];
		c->cid       = strtoul(t[0], NULL, 10);
		c->duracion  = strtoul(t[1], NULL, 10);
		c->anio      = strtoul(t[2], NULL, 10);
		c->total_rep = strtoul(t[3], NULL, 10);
		c->titulo    = copiar_cadena(t[4]);
		c->artista   = copiar_cadena(t[5]);
		c->album     = copiar_cadena(t[6]);
		c->genero    = copiar_cadena(t[7]);
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

	/*
	 * pedimos un nombre que sera recogido desde la entrada estandar,
	 * enviado a buff con un tamano maximo de sizeof(buff)
	 */
	printf("agregue un nombre para la playlist: ");
	fgets(buff, sizeof(buff), stdin);
	/* 
	 * calcula el numero de bytes en buff sin contar el salto de linea
	 * devuelve el indice de '\n' que es remplazado por un '\0'
	 * entonces remplaza el salto de linea por un termino de linea
	 */
	buff[strcspn(buff, "\n")] = '\0';

	pl.nombre = malloc(strlen(buff) + 1);
	if (pl.nombre == NULL)
		exit(1);
	strcpy(pl.nombre, buff);

	if (existe_plcsv()) {
		playlist_cargar_csv(&pl, ruta_pl);
	}

	return pl;
}

void imprimir_playlist(const playlist *pl)
{
	int i;

	if (pl == NULL) {
		printf("playlist vacia\n");
		return;
	}

	printf("Playlist #%u: %s\n", pl->pid, pl->nombre ? pl->nombre : "(sin nombre)");
	printf("Canciones: %d\n", pl->cantidad);
	printf("----------------------------------------------------------------------------------------------------\n");
	printf("%-4s %-30s %-20s %-20s %-6s %-5s %-5s\n",
			"ID", "Titulo", "Artista", "Album", "Dur", "Anio", "Rep");
	printf("-----------------------------------------------------------------------------------------------------\n");

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
	printf("------------------------------------------------------------------------------------------------a---\n");
}

int agregar_cancion_playlist(const char *ruta, cancion *c, playlist *pl)
{
	FILE *f;
	cancion *nueva;
	int n;
	f = fopen(ruta, "a");
	if ( f == NULL )
	{
		printf("error abriendo archivo");
		return 121;
	}

	fprintf(f, "%u;%u;%u;%u;%s;%s;%s;%s\n",
			c->cid, c->duracion, c->anio, c->total_rep,
			c->titulo, c->artista, c->album, c->genero);
	fclose(f);

	n = pl->cantidad;
	
	nueva = malloc(sizeof(cancion)*(n+1));
	if (nueva == NULL)
	{
		printf("error de asignacion de memoria\n");
	}

	if (pl->canciones != NULL)
	{
		/* copiamos a nueva la lista de canciones recordando que dejamos memoria libre al final */
		memcpy(nueva, pl->canciones , sizeof(cancion)*n);
		free(pl->canciones);
	}

	/* para n+1 elementos el ultimo indice n sera la nueva cancion agregada */
	nueva[n] = *c;
	pl->canciones = nueva;
	pl->cantidad = n+1;
	
	return 0;
}

/* Escribe el playlist en ruta, una cancion por linea (separador ';').
 * Retorna 0 si salio bien, 1 si falla. */
int exportar_playlist(const playlist *pl, const char *ruta)
{
	FILE *f;
	int i;

	if (pl == NULL || pl->canciones == NULL || ruta == NULL)
		return 1;

	f = fopen(ruta, "w");
	if (f == NULL)
		return 1;

	for (i = 0; i < pl->cantidad; i++)
		fprintf(f, "%u;%u;%u;%u;%s;%s;%s;%s\n",
		        pl->canciones[i].cid, pl->canciones[i].duracion,
		        pl->canciones[i].anio, pl->canciones[i].total_rep,
		        pl->canciones[i].titulo, pl->canciones[i].artista,
		        pl->canciones[i].album, pl->canciones[i].genero);

	fclose(f);
	return 0;
}
