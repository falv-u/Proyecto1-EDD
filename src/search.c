#include "songs.h"
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

/*
 * designado: Ariel
 * debe tener: busqueda binaria recursiva
 * ctype.h : isalnum, isalpha, isdigit, islower, isupper, tolower, toupper
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
    if (arr == NULL || artista_buscado == NULL)
        return -1;

    // lo convertimos a minusculas para comparar, usando la funcion creada
    char *buscado_lower = a_minusculas(artista_buscado);
    if (buscado_lower == NULL)
        return -1;

    for (int i = 0; i < n; i++)
    {
        if (arr[i].artista != NULL)
        {
            char *artista_lower = a_minusculas(arr[i].artista);
            if (artista_lower != NULL)
            {
                // comparar artista
                if (strcmp(artista_lower, buscado_lower) == 0)
                {
                    free(buscado_lower);
                    free(artista_lower);
                    return i;
                }
                free(artista_lower);
            }
        }
    }
    free(buscado_lower);
    return -1;
}

// funcion : convertira string a lowercase/minusculas
// *str debe ser un string valido (no NULL),
// funcion es char* : necesitamos comparar sin danhar datos del arreglo de canciones...
char* a_minusculas(char *str)
{
    if (str == NULL)
        return NULL;

    // malloc y evitar buffer overflow
    char *res = malloc(strlen(str)+1);
    if (res == NULL)
        return NULL;

    for (int i = 0; str[i]; i++)
        res[i] = tolower(str[i]);

    // y que res caracter final sea \0...
    res[strlen(str)] = '\0';

    return res;
}
