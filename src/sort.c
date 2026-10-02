#include "songs.h"
#include <stdlib.h>
#include <string.h>

// --- IDEA SORT ---- les parece bien Selection sort???
// pipe: le doy, segun un articulo q vi por ahi selection sirve para cuando el arreglo esta muy desordenado, mas que insertion
void insertion_sort_int(playlist *pl, int criterio)
{
	/* ariel: comprobaciones de null y cantidad
	*/
	if (pl == NULL || pl->canciones == NULL || pl->cantidad <= 1)
		return;

	int n = pl->cantidad;
	for (int i = 1; i < n; i++)
	{
		// debe ser mismo tipo de dato.
		// antes comparabamos .cid con key -> ahora comparamos la estructura completa.
		cancion key = pl->canciones[i];
		int j = i - 1;

		/*
		 * Comparacion de cada elemento con el key
		 * 0) id 1) duracion 2) anio 3) reps
		 * 4) titulo 5) artista
		 * 6) album 7) genero
		 */
		int es_mayor = 0;

		while (j >= 0)
		{
			switch (criterio)
			{
				case 0:
					es_mayor = (pl->canciones[j].cid > key.cid);
					break;

				case 1:
					es_mayor = (pl->canciones[j].duracion > key.duracion);
					break;

				case 2:
					es_mayor = (pl->canciones[j].anio > key.anio);
					break;

				case 3:
					es_mayor = (pl->canciones[j].total_rep > key.total_rep);
					break;

				case 4:
					es_mayor = (strcmp(pl->canciones[j].titulo, key.titulo) > 0);
					break;

				case 5:
					es_mayor = (strcmp(pl->canciones[j].artista, key.artista) > 0);
					break;

				case 6:
					es_mayor = (strcmp(pl->canciones[j].album, key.album) > 0);
					break;

				case 7:
					es_mayor = (strcmp(pl->canciones[j].genero, key.genero) > 0);
					break;

				default:
					es_mayor = (pl->canciones[j].cid > key.cid);
					break;
			}

			if (!es_mayor)
				break;

			// Desplazo hacia la derecha
			pl->canciones[j + 1] = pl->canciones[j];
			j--;
		}

		// codigo pipe ==============================
		/*
		   switch(criterio)
		   {
		   case 0:
		   while (j >= 0 && strcmp(pl->canciones[j].titulo, key.titulo) <= 0)
		   {
		   pl->canciones[j + 1] = pl->canciones[j];
		   j--;
		   }
		   break;
		   case 1:
		   while (j >= 0 && strcmp(pl->canciones[j].artista, key.artista) <= 0)
		   {
		   pl->canciones[j + 1] = pl->canciones[j];
		   j--;
		   }
		   break;
		   case 2:
		   while (j >= 0 && strcmp(pl->canciones[j].album, key.album) <= 0)
		   {
		   pl->canciones[j + 1] = pl->canciones[j];
		   j--;
		   }
		   break;
		   case 3:
		   while (j >= 0 && strcmp(pl->canciones[j].genero, key.genero) <= 0)
		   {
		   pl->canciones[j + 1] = pl->canciones[j];
		   j--;
		   }
		   break;
		   default:
		   while (j >= 0 && strcmp(pl->canciones[j].titulo, key.titulo) <= 0)
		   {
		   pl->canciones[j + 1] = pl->canciones[j];
		   j--;
		   }
		   break;
		   }*/

		pl->canciones[j + 1] = key;
	}
}

// merge o quicksort?
// [version recursiva]
// de ariel a pipe: borre mergesort hecho por mi porque estaba mal realizado. puedes usar quicksort si deseas.

void merge(playlist *p, int low, int med, int high, int criterio)
{
	// Declaracion de variables
	int i, j, k;
	int n_1 = (med - low) + 1; // Cantidad de elementos en el subarreglo izquierdo
	int n_2 = (high - med); // Cantidad de elementos en el subarreglo derecho

	// playlist *subarr_left, *subarr_right; // Subarreglos izquierdo y derecho

	// Asignacion de memoria
	// cambiado de playlist a cancion para que no se quede sin memoria
	cancion *subarr_left = malloc(n_1 * sizeof(cancion));
	cancion *subarr_right = malloc(n_2 * sizeof(cancion));

	// Copia de datos del arreglo A en los subarreglos L y R
	for (i = 0; i < n_1; i++)
	{
		subarr_left[i] = p->canciones[low + i];
		//subarr_left->canciones[i] = *(p->canciones + low + i);
	}

	for (j = 0; j < n_2; j++)
	{
		subarr_right[j] = p->canciones[med + j + 1];
		//subarr_right->canciones[j] = *(p->canciones + med + j + 1);
	}

	i = 0;
	j = 0;

	// Fusion de datos respetando el valor minimos entre dos arreglos
	for (k = low; k < high + 1; k++)
	{
		if (i == n_1)
		{
			p->canciones[k] = subarr_right[j];
			//p->canciones + k = subarr_right->canciones + j;
			j = j + 1;
		}
		else if(j == n_2)
		{
			p->canciones[k] = subarr_left[i];
			//p->canciones + k = subarr_left->canciones + i;
			i = i + 1;
		}
		else
		{
			int es_mayor = 0;

			// Comparacion según el criterio
			switch (criterio)
			{
				case 0:
					es_mayor = (subarr_left[i].cid > subarr_right[j].cid);
					break;
				case 1:
					es_mayor = (subarr_left[i].duracion > subarr_right[j].duracion);
					break;
				case 2:
					es_mayor = (subarr_left[i].anio > subarr_right[j].anio);
					break;
				case 3:
					es_mayor = (subarr_left[i].total_rep > subarr_right[j].total_rep);
					break;
				case 4:
					es_mayor = (strcmp(subarr_left[i].titulo, subarr_right[j].titulo) > 0);
					break;
				case 5:
					es_mayor = (strcmp(subarr_left[i].artista, subarr_right[j].artista) > 0);
					break;
				case 6:
					es_mayor = (strcmp(subarr_left[i].album, subarr_right[j].album) > 0);
					break;
				case 7:
					es_mayor = (strcmp(subarr_left[i].genero, subarr_right[j].genero) > 0);
					break;
				default:
					es_mayor = (subarr_left[i].cid > subarr_right[j].cid);
					break;
			}

			if (!es_mayor)
			{
				p->canciones[k] = subarr_left[i];
				//p->canciones[k] = *(subarr_left->canciones + i);
				i = i + 1;
			}
			else
			{
				p->canciones[k] = subarr_right[j];
				//*(p->canciones + k) = *(subarr_right->canciones + j);
				j = j + 1;
			}
		}
	}

	// liberar memoria
	free(subarr_left);
	free(subarr_right);
}

void merge_sort(playlist *p, int low, int high, int criterio)
{
	if (low < high)
	{
		// Dividir el problema en subproblemas
		int med = (low + high)/2;

		// Resolver el problema de manera recursiva hasta llegar a una solucion trivial
		merge_sort(p, low, med, criterio);
		merge_sort(p, med + 1, high, criterio);

		// Fusion de resultados parciales
		merge(p, low, med, high, criterio);
	}
}

// p: indice del primer elemento del arreglo
// r: indice del ultimo elemento del arreglo
// q: indice del elemento medio del arreglo

/*int main()
  {
  int i;
  int *p1 = (int *)malloc(8 * sizeof(int));

  merge_sort(p1, 0, p1.cantidad - 1, 0); // 0 = criterio ID

  for (i = 0; i < p1.cantidad; i++)
  {
  printf(" %i ", *(p1 + i));
  }
  printf("\n");

  eliminar_playlist(p1);

  return 0;
  }*/
