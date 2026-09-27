#include "songs.h"

int binsearch_cancion(void)
{
    return 0;
}

// binsearch : en su version recursiva : se debe llamar a si mismo
/*
int binsearch_cancion(cancion *arr, int izq, int der, uint32_t cid_buscado)
{
    if (bajo > alto)
        return -1;

    int med = izq + (der - izq)/2;
    if (arr[med].id == cid_buscado)
    {
        return med; // si lo encontramos
    }
    else if (arr[med].id < cid_buscado)
    {
        return binsearch_cancion(arr, med + 1, der, cid_buscado);
    }
    else
    {
        return binsearch_cancion(arr, izq, med - 1, cid_buscado);
    }
}
 */
