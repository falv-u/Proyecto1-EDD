#include "songs.h"
#include <stdlib.h>

// --- IDEA SORT ---- les parece bien Selection sort???
// pipe: le doy, segun un articulo q vi por ahi selection sirve para cuando el arreglo esta muy desordenado, mas que insertion
void insertion_sort(playlist *pl)
{
    int n = pl->cantidad;
    for (int i = 1; i < n; i++)
    {
        // debe ser mismo tipo de dato.
        // antes comparabamos .cid con key -> ahora comparamos la estructura completa.
        cancion key = pl->canciones[i];
        int j = i - 1;

        while (j >= 0 && pl->canciones[j].cid > key.cid)
        {
            pl->canciones[j + 1] = pl->canciones[j];
            j--;
        }
        pl->canciones[j + 1] = key;
    }
}

// merge o quicksort?
// [version recursiva]
// de ariel a pipe: borre mergesort hecho por mi porque estaba mal realizado. puedes usar quicksort si deseas.
