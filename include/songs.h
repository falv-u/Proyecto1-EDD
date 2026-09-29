#define MAXCANCIONES 3
#define RANGO_CID 100

#ifndef CANCIONES
#define CANCIONES

#include <stdint.h>
#include "ui.h"

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
	cancion *canciones; // Lista de IDs de canciones
	uint32_t pid; // ID de la playlist
	char *nombre; // Nombre de la playlist
	int cantidad; // Cantidad de canciones en la playlist
} playlist;

typedef struct {
    uint32_t *playlists; // Lista de IDs de playlists
    int cantidad; // Cantidad de canciones en la fonoteca
} fonoteca;

/* RUTAS */
static const char ruta_pl[] = "./playlist.csv";
static const char ruta_historial[] = "./historial.csv";

/* ----------FUNCIONES GENERALES ---------*/ 
/* Genera un ID único para una cancion. */
uint32_t generar_id();

/* ----------FUNCIONES CANCIONES ---------*/ 
/* Reproduce el mp3 de la ruta del archivo.
 * Retorna 0 en exito, -1 si falla. */
int reproducir_musica(void);

/* Inicializa ia informacion de las canciones desde un archivo CSV.
 * Retorna 0 en éxito, 121 si no puede abrir el archivo. */
void inicializar_canciones(cancion canciones[]);

/* Busca la cancion en la fonoteca por un string ingresado por el usuario.
 * Retorna el índice de la cancion si se encuentra, -1 si no. */
int binsearch_cancion(void);

/* ----------FUNCIONES PLAYLIST ---------*/ 
/* Verifica si el archivo CSV de la playlist existe.
 * Retorna 1 si existe, 0 si no. */
int existe_plcsv(void);

/* Duplica una cadena de texto reservando memoria dinámica.
 * Retorna un puntero a la nueva cadena, o NULL si falla. */
char *dup(const char *s);

/* Carga las canciones desde un archivo CSV a la playlist.
 * Retorna 0 en éxito, 121 si no puede abrir el archivo. */
int playlist_cargar_csv(playlist *pl, const char *ruta);

/* Libera toda la memoria asociada a una playlist. */
void liberar_pl(playlist *pl);

/* Crea una nueva playlist solicitando nombre al usuario.
 * Retorna la estructura playlist inicializada. */
playlist crear_playlist(void);

/* imprime toda la lista de canciones de una playlist */
void imprimir_playlist(const playlist *pl);

/* ----------FUNCIONES HISTORIAL ---------*/ 

/* verifica si existe el archivo de historial en caso de no, 
 * llama a escribir_archivo_historial 
 * Retorna 0 si todo salio bien, en caso de no existir y no poder crearse
 * devuelve un 1
 * */
int existe_historial(void);

/* crea o sobreescribe el archivo de ruta_historial
 * Retorna 0 si todo salio bien, caso contrario retorna 1
 */
int escribir_archivo_historial(void);
#endif
