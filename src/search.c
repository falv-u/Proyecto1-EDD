#include "songs.h"

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
