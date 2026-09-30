#include "songs.h"
#include <stdlib.h>
#include <ctype.h>

/*
 * designado: Ariel
 * debe tener: busqueda binaria recursiva
 * ctype.h : isalnum, isalpha, isdigit, islower, isupper, tolower, toupper
 */

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
int search_titulo(cancion *arr, int n, char *titulo_buscado)
{
    if (arr == NULL || titulo_buscado == NULL)
        return -1;

    for (int i = 0; i < n; i++)
    {
        // comparar titulo (falta pasarlo a lowercase..)
        if (arr[i].titulo != NULL && compara_strings(arr[i].titulo, titulo_buscado))
            return i;
    }
    return -1;
}

/* Busqueda por artista, sin distinguir mayusculas (Todas las de un artista, nombre exacto) */
int search_artista(cancion *arr, int n, char *artista_buscado, int *resultados, int max_resultados)
{
    if (arr == NULL || artista_buscado == NULL)
        return -1;

    if (max_resultados < 0)
        return 1;

    int j = 0;
    for (int i = 0; i < n; i++)
    {
        // Utilizamos la funcion comparadora de strings a lowercase
        if (arr[i].artista != NULL && compara_strings(arr[i].artista, artista_buscado))
        {
            // el contador j no puede superar los max resultados indicados en la llamada a la funcion.
            if (resultados != NULL && j < max_resultados)
                resultados[j] = i;
            j++;
        }
    }

    return j;
}

// funcion : convertira string a lowercase/minusculas
// *str debe ser un string valido (no NULL),
// funcion es char* : necesitamos comparar sin danhar datos del arreglo de canciones...
/*
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
*/
