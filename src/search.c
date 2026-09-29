#include "songs.h"
#include <string.h>

// designado: ariel
/*
 * debe tener: busqueda binaria recursiva
 */

int binsearch_cancion(cancion *arr, int izq, int der, uint32_t cid_buscado)
{
    if (izq > der)
        return -1;

    int med = izq + (der - izq)/2;
    if (arr[med].cid == cid_buscado)
    {
        return med; // si lo encontramos
    }
    else if (arr[med].cid < cid_buscado)
    {
        return binsearch_cancion(arr, med + 1, der, cid_buscado);
    }
    else
    {
        return binsearch_cancion(arr, izq, med - 1, cid_buscado);
    }
}

// busqueda por nombre y titulo
int search_titulo(cancion *arr, int n, char *titulo_buscado)
{
    if (arr == NULL || titulo_buscado == NULL)
        return -1;

    for (int i = 0; i < n; i++)
    {
        if (arr[i].titulo != NULL)
        {
            // comparar titulo (falta pasarlo a lowercase..)
            if (strcmp(arr[i].titulo, titulo_buscado) == 0)
            {
                return i;
            }
        }
    }

    return -1;
}

// busqueda por artista (todas las de 1 artista)
int search_artista(cancion *arr, int n, char *artista_buscado)
{
    return 0;
}
