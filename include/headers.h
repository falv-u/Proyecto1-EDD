#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

/* id unico que referencia a una sola cancion*/
typedef struct {
	uint32_t cid; 
	uint32_t duracion;
	uint16_t anio; 
	uint16_t total_rep; 

	char *titulo;
	char *artista;
	char *album;
	char *genero;

} cancion;

typedef struct {
	char *nombre;
	Cancion *canciones;
	int cantidad;
} playlist;

typedef struct {
    playlist *playlists;
    int cantidad;
} main; 

/* notas panchito: Main va en main??? o pq el nombre main? */ 

/* porfa, tabs y no espacios...*/
/* 
 * entonces para acceder al titulo de x cancion el viaje seria Main->playlist->nombre->titulo
 * si tenemos varios Main, o varias playlist se nos complicara un poco el codigo, podriamos tratar de reducirlo
 * por cada iteracion de esas tenemos al menos 5 mallocs sin contar los datos faltantes.
 * sugiero:
 *  - Tener una coleccion que guarde las playlist, las playlist en lugar de guardar nombres podrian guardar un id
 *  - cada cancion tendra un id, para reordenar o buscar, buscamos enteros en lugar de cadenas de texto complejas.
 *  - entonces podemos tratar reducir el uso de cadenas lo maximo posible.
 *
 *  estructura que pense de playlist:
 *  typedef struct playlist {
 *	char *nombre_pl;
 *	int *id_canciones;
 *	int total; <- canciones que tenemos realmente
 *	size_t capacidad; <- guarda cuantas canciones podemos tener
 *  }
 *
 *  no confudir los id de las canciones con los id de la playlist
 *  idc -> referencia a una cancion en especifico (no muta, no lo cambiamos)
 *  id  -> referencia el orden en el que aparecen (si muta, los reordenamos)
 */
