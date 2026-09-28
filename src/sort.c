#include "songs.h"

void insertion_sort(void);

// --- IDEA SORT ---- les parece bien Selection sort???
// pipe: le doy, segun un articulo q vi por ahi selection sirve para cuando el arreglo esta muy desordenado, mas que insertion
/*
void insertion_sort(playlist *pl)
{
    int n = pl->cantidad;
    for (int i = 1; i < n; i++)
    {
        int key = pl->canciones[i].cid;
        int j = i - 1;

        while (j >= 0 && pl->canciones[j].cid > key)
        {
            pl->canciones[j + 1].cid = pl->canciones[j].cid;
            j--;
        }
        pl->canciones[j + 1].cid = key;
    }
}
*/

//
// merge o quicksort :ooooo
// sea cual sea iria por aca <<<<<
