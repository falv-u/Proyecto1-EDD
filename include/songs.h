#ifndef CANCIONES
#define CANCIONES

#define MAXCANCIONES 3
#define RANGO_CID 100
#define MAX_HISTORIAL 30
#define MAX_COLA 300
#include <stdint.h>

/** @brief Estructura que contiene todos los datos relevantes de una cancion.
 *
 * Esta estructura solo contiene dos tipos de datos: para los numeros se
 * usan enteros sin signo con un numero fijo de bytes (8 y 4), y para
 * guardar cadenas de texto se usa un puntero hacia un espacio de memoria
 * asignado por malloc que contenga los datos.
 * */
typedef struct cancion {
    uint32_t cid;        /**< ID de la cancion, se busca que no tenga repeticion */
    uint32_t duracion;   /**< Duracion en segundos, entero de 8 bytes sin signo */
    uint32_t total_rep;  /**< Total de veces reproducida, entero de 4 bytes sin signo */
    uint16_t anio;       /**< Anio de lanzamiento, entero de 4 bytes sin signo */
    char    *titulo;     /**< Titulo de la cancion, asignado con malloc */
    char    *artista;    /**< Artista o banda */
    char    *album;      /**< Album al que pertenece */
    char    *genero;     /**< Genero musical */
} cancion;


/** @brief Estructura que contiene los datos de cada playlist.
 *
 * Esta estructura tambien
 */
typedef struct playlist {
	cancion *canciones; /* Lista de canciones en arreglo contiguo */
	uint32_t pid; // ID de la playlist
	char *nombre; // Nombre de la playlist
	char *ruta;	/* en caso de tener, caso contrario si solo esta alojada en memoria tipo NULL */
	int cantidad; /* Cantidad de canciones en la playlist */
} playlist;

/** @brief la fonoteca es aquella que guardara todas las canciones con su id.
 * La fonoteca contiene todas las canciones, sin repeticion.
 * Estas estan ordenadas por ID de menor a mayor, cosa de poder buscar canciones
 * mediante busqueda binaria.
 */
typedef struct {
	char *ruta_fonoteca;
	uint32_t cantidad;
} fonoteca;

/* RUTAS */
/* Rutas relativas, imposible modificarlas en ejecucion */
static const char ruta_pl[] = "./playlist.csv";
static const char ruta_historial[] = "./historial.csv";

/* ----------FUNCIONES GENERALES ---------*/
/* Genera un ID único para una cancion. */
uint32_t generar_id(void);

/* Compara dos strings sin distinguir mayusculas/minusculas */
int compara_strings(char *a, char *b);

/* ----------FUNCIONES CANCIONES ---------*/
/* @brief Reproduce el mp3 de la ruta del archivo.
 * Usa funcion proveniente del single-header de miniaudio.h y su
 * implementacion (2 lineas) miniaudio.c. Solo reproduce musica.

 * Retorna 0 en exito, -1 si falla. */
int reproducir_musica(const char *ruta, cancion *c);

/* @brief Inicializa ia informacion de las canciones desde un archivo CSV.
 * Retorna 0 en éxito, 121 si no puede abrir el archivo. */
void inicializar_canciones(cancion canciones[]);

/* ARREGLAR */
/* Busca la cancion en la fonoteca por un string ingresado por el usuario.
 * Retorna el índice de la cancion si se encuentra, -1 si no. */
int binsearch_cancion(playlist *pl, int izq, int der, uint32_t cid_buscado);

/* Lista todos los artistas disponibles en el catálogo sin repetir */
void listar_artistas(const playlist *pl);

/* Pide un género al usuario, busca las canciones, cuenta el total y las lista */
void interactuar_generos(playlist *pl);

/* ----------FUNCIONES PLAYLIST ---------*/
/**
 * Verifica si el archivo CSV de la playlist existe.
 * Retorna 1 si existe, 0 si no.
 */

/* ----------FUNCIONES PLAYLIST ---------*/
/** @brief busca si existe un archivo.
 * Verifica si el archivo CSV de la playlist existe tratando de abrir
 * el contenido en modo lectura.
 * @return Retorna 1 si existe, 0 si no.
 * */
int existe_plcsv(void);

/* @brief Duplica una cadena de texto reservando memoria dinámica.
 * @return Retorna un puntero a la nueva cadena, o NULL si falla. */
char *copiar_cadena(const char *s);

/** @brief Carga las canciones desde un archivo CSV a la playlist.
 *  para cierto archivo de ruta, se busca cargar todos sus datos a
 *  una playlist, primero se cuentan las lineas que tiene el archivo
 *  sabiendo el numero de canciones se reserva la memoria con malloc.
 *  se separa el texto segun un separador '|', cada dato separado se
 *  guarda en una direccion de memoria en ram, por tanto para no perder
 *  el acceso se guarda en un arreglo char *temporal, una vez guardada
 *  esa linea, se hace una verificacion sencilla de exito, y si en caso de que
 *  en alguna parte de el arreglo temporal apunte a NULL, n<datos_de_cancion
 *  en este caso 8, si sucede se salta esa iteracion y esos datos no se guardan
 *  en memoria. si caso contrario todo sale bien esos datos son transformados
 *  a sus respectivas unidades y se guardan en la playlist.
 *
 * @return Retorna 0 en éxito, 121 si no puede abrir el archivo. */
int playlist_cargar_csv(playlist *pl, const char *ruta);

/* @brief Libera toda la memoria asociada a una playlist. */
void liberar_pl(playlist *pl);

/* imprime toda la lista de canciones de una playlist */
void imprimir_playlist(const playlist *pl);

/** @brief
 * Funcion de creado que usa funciones auxiliares para modularidad, en si esta
 * funcion solo define por si misma el id de una playlist y su nombre.
 * @return Playlist
 */

playlist crear_playlist(void);


int agregar_cancion_playlist(const char *ruta, cancion *c, playlist *pl);

/* ----------FUNCIONES HISTORIAL ---------*/

/** @brief verifica si existe el archivo de historial en disco.
 *
 * trata de abrir el archivo en caso de no poder abrirlo, lo tratara de crear
 * mediante una llamada a la funcion escribir_archivo_historial();
 * Retorna 0 si todo salio bien, en caso de no existir y no poder crearse devuelve un 1
 */

int existe_historial(void);

/** @brief crea o sobreescribe el archivo de ruta_historial.
 *
 * Una funcion que abre el archivo con el parametro "w", si no existe
 * el archivo lo crea, si existe lo sobreescribe. se puede usar
 * para la primera inicializacion del programa o para
 * limpiar por completo el histoial
 *
 * @return retorna 0 si se pudo escribir en disco, caso contrario retorna 1
 */
int escribir_archivo_historial(void);

/* Ranking */
/** @brief muestra el top de canciones mas escuchadas
 *
 * recibe como parametro la playlist, donde se crea un arreglo
 * de pl->cantidad canciones, en donde solo se guardan los indices
 * de las canciones y por ordenamiento burbuja revisa caso a caso
 * si el total de repeticiones de la cancion en el indice idx 
 * es menor que el siguiente, en casi verdadero intercambia.
 */
void canciones_mas_escuchada(const playlist *pl);
int agregar_a_historial(playlist *h, const cancion *c);
#endif
