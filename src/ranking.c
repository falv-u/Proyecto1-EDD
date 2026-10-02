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

void mas_escuchado(const playlist *pl)
{
	int i, j, mejor, repetido;

	if (pl == NULL || pl->canciones == NULL || pl->cantidad <= 0)
	{
		printf("playlist invalida\n");
		return;
	}

	for (i = 0; i < pl->cantidad; i++)
	{
		if (pl->canciones[i].artista == NULL)
			continue;

		/* saltar si este artista ya salio antes */
		repetido = 0;
		for (j = 0; j < i && !repetido; j++)
			if (compara_strings(pl->canciones[j].artista, pl->canciones[i].artista))
				repetido = 1;
		if (repetido)
			continue;

		/* primera vez: buscar su cancion mas escuchada */
		mejor = i;
		for (j = i + 1; j < pl->cantidad; j++)
			if (compara_strings(pl->canciones[j].artista, pl->canciones[i].artista)
			    && pl->canciones[j].total_rep > pl->canciones[mejor].total_rep)
				mejor = j;

		printf("%-25s %s (%u reps)\n",
		       pl->canciones[mejor].artista,
		       pl->canciones[mejor].titulo,
		       pl->canciones[mejor].total_rep);
	}
}

int mas_escuchada_de(const playlist *pl, char *c, int campo)
{
	int i; 
	int mejor;
	mejor = -1;

	if (pl == NULL || pl->canciones == NULL || c == NULL || campo < 1 || campo > 2)
		return -1;

	switch (campo)
	{
		case 1:
		for (i = 0; i < pl->cantidad; i++)
		{
			if (compara_strings(pl->canciones[i].artista, c)
					&& (mejor == -1 || pl->canciones[i].total_rep > pl->canciones[mejor].total_rep))
				mejor = i;
		}
		break;

		case 2:
		for (i = 0; i < pl->cantidad; i++)
		{
			if (compara_strings(pl->canciones[i].genero, c)
					&& (mejor == -1 || pl->canciones[i].total_rep > pl->canciones[mejor].total_rep))
				mejor = i;
		}
		break;

		default:
		printf("campo invalido...como llegaste aqui?\n");
		return -1;
	}

	return mejor;
}

int mas_escuchada_artista(const playlist *pl, char *c)
{
	return mas_escuchada_de(pl, c, 1);
}

int mas_escuchada_genero(const playlist *pl, char *c)
{
	return mas_escuchada_de(pl, c, 2);
}
