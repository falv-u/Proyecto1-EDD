#include <stdint.h>
#include <stdio.h>

#include "songs.h"
#define N_ME 10

void canciones_mas_escuchada(const playlist *pl)
{
	if (pl == NULL || pl->canciones == NULL)
	{
		printf("playlist apunta NULL...\n");
		return;
	}
	if (pl->cantidad <= 0)
	{
		printf("valores invalidos\n");
		return;
	}
	/* posiciones de las canciones */
	int idx[pl->cantidad];
	int i, j, tmp, n;

	for (i = 0; i < pl->cantidad; i++)
		idx[i] = i;

	/* burbuja sobre los indices, comparando total_rep */
	for (i = 0; i < pl->cantidad - 1; i++)
	{
		for (j = 0; j < pl->cantidad - 1 - i; j++)
			if (pl->canciones[idx[j]].total_rep < pl->canciones[idx[j + 1]].total_rep)
			{
				tmp = idx[j];
				idx[j] = idx[j + 1];
				idx[j + 1] = tmp;
			}
	}
	/* si N_ME es menor a cantidad de canciones, toma el valor NM_e caso contrario toma cantidad */
	n = (N_ME < pl->cantidad) ? N_ME : pl->cantidad;
	for (i = 0; i < n; i++)
		printf("%2d. %s - %s (%u)\n", i + 1,
		       pl->canciones[idx[i]].artista,
		       pl->canciones[idx[i]].titulo,
		       pl->canciones[idx[i]].total_rep);
}


void artista_mas_escuchado(playlist *pl)
{
	int i,j,mejor,repetido;
	if (pl == NULL || pl->canciones == NULL || pl->cantidad <= 0)
	{	
		printf("playlist invalida\n");
		return;
	}

	for ( i = 0; i < pl->cantidad; i++)
	{
		if (pl->canciones[i].artista == NULL)
			continue;	
		if (compara_strings(pl->canciones[j].artista, pl->canciones[i].artista)
			&& pl->canciones[j].total_rep > pl->canciones[mejor].total_rep)
			mejor = j;

		printf("%-25s %s (%u reps)\n",
		       pl->canciones[mejor].artista,
		       pl->canciones[mejor].titulo,
		       pl->canciones[mejor].total_rep);
	}
}
