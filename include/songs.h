#ifndef CANCIONES
#define CANCIONES

#include <stdint.h>

/* id unico que referencia a una sola cancion*/
typedef struct {
	uint32_t cid; // ID de la cancion
	uint32_t duracion; // Duracion en segundos
	uint16_t anio; // Anio de lanzamiento
	uint16_t total_rep; // Total de reproducciones

	char *titulo;
	char *artista;
	char *album;
	char *genero;
	//char *ruta; //?? sera necesario <- ariel aca, lo mismo en playlsit
} cancion;

typedef struct {
	uint32_t *canciones; // Lista de IDs de canciones
	uint32_t pid; // ID de la playlist
	char *nombre; // Nombre de la playlist
	int cantidad; // Cantidad de canciones en la playlist
} playlist;

typedef struct {
    uint32_t *playlists; // Lista de IDs de playlists
    int cantidad; // Cantidad de canciones en la fonoteca
} fonoteca;

int reproducir_musica(void);

/* notas ariel aca...: puse un define.h para poner el maximo de canciones y playlist a soportar en nuestro proyecto */
/* tengo una duda de como organizamos el proyecto... si lo hacemos archivo a archivo, punto a punto lo mencionado
 * en el pdf.. por ej:
 * Punto 1 Gestion de catalogo -> gestor.c
 * Punto 2 Ordenamiento y busqueda -> sort.c
 * ... Punto 7 interfaz -> interfaz.c
 * Quiero hacer mi primera funcion :D --------- cambio y fuera
 */

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

/* notas pipe:
 * TENEMOS DEPENDENCIAS!!!
 * (todo lo que tenga que ver con) miniaudio!!!!!!!!
 */

#endif
