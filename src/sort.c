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
// investigando bien: para este caso nos conviene merge sort[tipo divide y venceras]. queremos estabilidad y no fallos
// merge sort(ordenamiento x mezcla): version recursiva
void merge_sort(playlist *pl)
{
    // Caso invalido.
    if (pl == NULL || pl->canciones == NULL || pl->cantidad <= 1)
        return;

    int n = pl->cantidad;
    int mid = n / 2;

    // dividir en mitades
    playlist left, right;
    left.canciones = pl->canciones;
    left.cantidad = mid;
    right.canciones = pl->canciones + mid;
    right.cantidad = n - mid;

    // se ordena de forma recursiva ambas mitades izq y der (importante)
    merge_sort(&left);
    merge_sort(&right);

    // cancion *temp = (cancion *)malloc(n * sizeof(cancion));
    cancion *temp = malloc(n * sizeof(cancion));
    if (temp == NULL)
        return;

    // indice para:
    // left(i), right(j), variable temporal(k)...
    int i = 0, j = 0, k = 0;

    // se ejecuta mientras ambas mitades tengan elementos pendientes...
    // compara la parte left (izq) y la parte actual right (der)
    // el que tenga cID menor/igual se copia primero en temp, y luego se avanza el indice correspondiente
    // avanza el puntero de la mitad elegida
    while (i < left.cantidad && j < right.cantidad)
    {
        if (left.canciones[i].cid <= right.canciones[j].cid)
        {
            temp[k] = left.canciones[i];
            i++;
        }
        else
        {
            temp[k] = right.canciones[j];
            j++;
        }
        k++;
    }

    // vacia los sobrantes de la parte left (izq), revisa si esa mitad tiene elementos aun..
    while (i < left.cantidad)
    {
        temp[k] = left.canciones[i];
        i++;
        k++;
    }

    // ahora con la parte right (der)
    while (j < right.cantidad)
    {
        temp[k] = right.canciones[j];
        j++;
        k++;
    }

    // copiar el resultado de vuelta a pl->canciones, el arreglo original
    for (k = 0; k < n; k++)
    {
        pl->canciones[k] = temp[k];
    }

    // y liberamos con free...
    free(temp);
}
