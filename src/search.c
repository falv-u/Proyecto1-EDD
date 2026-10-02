#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include "songs.h"
#include "ui.h"
/*
 * designado: Ariel
 * debe tener: busqueda binaria recursiva
 * ctype.h : isalnum, isalpha, isdigit, islower, isupper, tolower, toupper
 */

// Prototipos locales para que las funciones se conozcan entre sí sin importar el orden
int binsearch_cancion(playlist *pl, int izq, int der, uint32_t cid_buscado);
int search_titulo(playlist *pl, char *titulo_buscado);
int search_artista(playlist *pl, char *artista_buscado, int *resultados, int max_resultados);
int search_genero(playlist *pl, char *genero_buscado, int *resultados, int max_resultados);
int search_album(playlist *pl, char *album_buscado, int *resultados, int max_resultados);
cancion busqueda_global_lenta(playlist *pl, int prioridad, unsigned int dato_numerico, char *dato_texto);

// compara dos strings sin distinguir mayúsculas/minúsculas
int compara_strings(char *a, char *b)
{
	if (a == NULL || b == NULL)
		return 0;

	// mientras la cadena de A o B no termine
	while (*a != '\0' && *b != '\0')
	{
		if (tolower((unsigned char)*a) != tolower((unsigned char)*b))
			return 0;
		a++;
		b++;
	}
	// El retorno es 0 o 1 dependiendo si *A es igual a *B
	return *a == *b;
}

/* parte de francisco. */
cancion busqueda_global_lenta(playlist *pl, int prioridad, unsigned int dato_numerico, char *dato_texto)
{
	int indice = -1;

	// Crear una cancion vacia si no se pasa ninguna
	cancion no_encontrada = {0};
	if (pl == NULL || pl->canciones == NULL || pl->cantidad <= 0)
		return no_encontrada;

	/*  
	    0: BUSCAR TODOS
	    1: BUSCAR POR CID
	    2: BUSCAR POR TITULO
	    3: BUSCAR POR ARTISTA
	    4: BUSCAR POR ALBUM 
	    */
	switch (prioridad)
	{
		case 1:
			indice = binsearch_cancion(pl, 0, pl->cantidad - 1, (unsigned int)dato_numerico);
			break;
		case 2:
			indice = search_titulo(pl, dato_texto);
			break;
		case 3:
			{
				int temp = -1;
				int encontrados = search_artista(pl, dato_texto, &temp, 10);
				if (encontrados > 0)
				{
					indice = temp;
				}
				break;
			}
		default:
			return no_encontrada;
	}

	if (indice >= 0 && indice < pl->cantidad)
	{
		return pl->canciones[indice];  
	}
	return no_encontrada;
}



// busca una cancion por cID dentro de una playlist ordenada por cID
int binsearch_cancion(playlist *pl, int izq, int der, uint32_t cid_buscado)
{
	if (pl == NULL || pl->canciones == NULL)
		return -1;

	if (izq < 0 || der >= pl->cantidad || izq > der)
		return -1;

	int med = izq + (der - izq)/2;

	if (pl->canciones[med].cid == cid_buscado)
	{
		return med; // si lo encontramos
	}
	else if (pl->canciones[med].cid < cid_buscado)
	{
		return binsearch_cancion(pl, med + 1, der, cid_buscado);
	}
	else
	{
		return binsearch_cancion(pl, izq, med - 1, cid_buscado);
	}
}

/* Busqueda por titulo, sin distinguir mayusculas (exacta). */
int search_titulo(playlist *pl, char *titulo_buscado)
{
	if (pl == NULL || pl->canciones == NULL || titulo_buscado == NULL)
		return -1;

	for (int i = 0; i < pl->cantidad; i++)
	{
		// comparar titulo (falta pasarlo a lowercase..)
		if (pl->canciones[i].titulo != NULL && compara_strings(pl->canciones[i].titulo, titulo_buscado))
			return i;
	}
	return -1;
}

/* Busqueda por artista, sin distinguir mayusculas (Todas las de un artista, nombre exacto) */
int search_artista(playlist *pl, char *artista_buscado, int *resultados, int max_resultados)
{
	if (pl == NULL || pl->canciones == NULL || artista_buscado == NULL || max_resultados <= 0)
		return 0;

	int j = 0;
	for (int i = 0; i < pl->cantidad; i++)
	{
		// Utilizamos la funcion comparadora de strings a lowercase
		if (pl->canciones[i].artista != NULL && compara_strings(pl->canciones[i].artista, artista_buscado))
		{
			// el contador j no puede superar los max resultados indicados en la llamada a la funcion.
			if (resultados != NULL && j < max_resultados)
				resultados[j] = i;
			j++;
		}
	}

	return j;
}

/* Busqueda por genero, sin distinguir mayusculas (exacto) */
int search_genero(playlist *pl, char *genero_buscado, int *resultados, int max_resultados)
{
	if (pl == NULL || pl->canciones == NULL ||genero_buscado == NULL || max_resultados <=0)
		return 0;

	int j = 0;
	for (int i = 0; i < pl->cantidad; i++)
	{
		if (pl->canciones[i].genero != NULL && compara_strings(pl->canciones[i].genero, genero_buscado))
		{
			if (resultados != NULL && j < max_resultados)
				resultados[j] = i;
			j++;
		}
	}
	return j;
}

/* Busqueda por album, sin distinguir mayusculas (exacto)*/
int search_album(playlist *pl, char *album_buscado, int *resultados, int max_resultados)
{
	if (pl == NULL || pl->canciones == NULL || album_buscado == NULL)
		return 0;

	int j = 0;
	for(int i = 0; i < pl->cantidad; i++)
	{
		if (pl->canciones[i].album != NULL && compara_strings(pl->canciones[i].album, album_buscado))
		{
			if (resultados != NULL && j < max_resultados)
				resultados[j] = i;
			j++;
		}
	}
	return j;
}

// Puede ir en search o en main.c
// Ejecuta la funcion de busqueda del artista y devuelve el indice de la cancion
void ejecuta_busqueda_artista(playlist *pl)
{
	char dato_texto[128];
	int max_resultados = 20;
	int temp[max_resultados];

	// Pedimos a ui.h que imprima.

	// usamos fgets para leer una linea de la entrada estandar
	if(fgets(dato_texto, sizeof(dato_texto), stdin) != NULL)
	{
		// Revisar la ultima posicion usando strlen
		size_t len = strlen(dato_texto);
		if (len > 0 && dato_texto[len - 1] == '\n')
		{
			dato_texto[len - 1] = '\0';
		}
	}

	// llamamos a la funcion de busqueda
	int encontrados = search_artista(pl, dato_texto, temp, max_resultados);
	if (encontrados > 0)
	{
		ui_resultados_busqueda(encontrados, dato_texto);

		int limite_iteracion = encontrados;
		if (encontrados > max_resultados)
		{
			limite_iteracion = max_resultados;
		}

		for (int i = 0; i < limite_iteracion; i++)
		{
			cancion *c = &pl->canciones[temp[i]];
			ui_cancion_busqueda(i+1, c);
		}
	}
	else
	{
		ui_sin_resultados(dato_texto);
	}
	ui_pausa();
}

void listar_artistas(const playlist *pl)
{
	if (pl == NULL || pl->canciones == NULL || pl->cantidad <= 0)
	{
		printf(COLOR_YELLOW "\n Catalogo vacio.\n" COLOR_RESET);
		return;
	}
	printf(COLOR_CYAN "\n  --- Artistas Disponibles ---:\n" COLOR_RESET);

	for (int i = 0; i < pl->cantidad; i++)
	{
		// Si no hay artista lo saltamos
		if (pl->canciones[i].artista == NULL) continue;

		// Comprobar si el artista ya fue impreso revisando las posiciones anteriores
		// desde 0 hasta i-1
		int repetido = 0;
		for (int j = 0; j < i; j++)
		{
			if (pl->canciones[j].artista != NULL && compara_strings(pl->canciones[j].artista, pl->canciones[i].artista))
			{
				repetido = 1;
				break;
			}
		}

		if (!repetido)
		{
			printf("    - %s\n", pl->canciones[i].artista);
		}
	}
}

void interactuar_genero(playlist *pl)
{
	if (pl == NULL || pl->canciones == NULL || pl->cantidad <= 0)
	{
		ui_sin_resultados("genero");
	}

	char genero_buscado[128];
	int max_resultados = pl->cantidad;

	// pl->cantidad puede ser muy grande
	int *resultados = malloc(sizeof(int) * max_resultados);
	if (resultados == NULL)
	{
		printf(COLOR_RED "\nError: No hay memoria suficiente." COLOR_RESET);
		return;
	}

	canciones_por_genero(pl);

	printf(COLOR_CYAN "\n  >> Introduce el genero a buscar: " COLOR_RESET);
	if (fgets(genero_buscado, sizeof(genero_buscado), stdin) != NULL)
	{
		size_t len = strlen(genero_buscado);
		if (len > 0 && genero_buscado[len - 1] == '\n')
		{
			genero_buscado[len - 1] = '\0';
		}
	}

	int encontrados = search_genero(pl, genero_buscado, resultados, max_resultados);
	if (encontrados > 0)
	{
		printf(COLOR_GREEN "\n  Se encontraron %d canciones del genero '%s':\n" COLOR_RESET, encontrados, genero_buscado);
		for (int i = 0; i < encontrados; i++) 
		{
			cancion *c = &pl->canciones[resultados[i]];
			printf("    %d. %s - %s\n", i + 1, c->artista, c->titulo);
		}
	}
	else
	{
		printf(COLOR_RED "\n  No se encontraron canciones del genero '%s'." COLOR_RESET, genero_buscado);
	}
	free(resultados);
}
