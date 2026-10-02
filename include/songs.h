#ifndef CANCIONES
#define CANCIONES

#include  <stdint.h>

#define MAXCANCIONES 3
#define RANGO_CID 100
#define MAX_HISTORIAL 30
#define MAX_COLA 300

#define DUR_MIN 60
#define DUR_MAX 600
#define ANIO_MIN 1960
#define ANIO_MAX 2026
#define REP_MAX 100000
#define N_MAXIMO 5000000
#define N_CATALOGO_DEFAULT 100

#define N_ARTISTAS 15
#define N_GENEROS 9
#define N_PALABRAS 30

/** @brief Estructura que contiene todos los datos relevantes de una cancion.
 *
 * Esta estructura contiene datos numericos enteros sin signo con un numero
 * fijo de bytes (uint32_t para identificadores, duracion y reproducciones,
 * y uint16_t para el anio de lanzamiento), mientras que para las cadenas de
 * texto se usan punteros char* que apuntan a memoria dinamica reservada con
 * malloc, permitiendo almacenar cadenas de longitud variable.
 */
typedef struct cancion {
    uint32_t cid;        /**< @brief ID unico de la cancion, generado autoincrementalmente */
    uint32_t duracion;   /**< @brief Duracion de la cancion en segundos (entero sin signo de 4 bytes) */
    uint32_t total_rep;  /**< @brief Total acumulado de reproducciones (entero sin signo de 4 bytes) */
    uint16_t anio;       /**< @brief Anio de publicacion de la cancion (entero sin signo de 2 bytes) */
    char    *titulo;     /**< @brief Titulo de la cancion, direccion a bloque asignado en heap */
    char    *artista;    /**< @brief Nombre del interprete o agrupacion musical */
    char    *album;      /**< @brief Nombre del album discografico al que pertenece */
    char    *genero;     /**< @brief Categoria o genero musical correspondiente */
} cancion;

/** @brief Estructura que gestiona colecciones de canciones en memoria contigua.
 *
 * Esta estructura maneja arreglos dinamicos de canciones, asociando metadatos
 * como su identificador numerico, un nombre asignado por el usuario o sistema,
 * la ruta asociada en disco si persiste en archivo CSV, y el contador exacto
 * de elementos validos que residen actualmente en memoria. Se utiliza para
 * representar el catalogo general, la cola de reproduccion y el historial.
 */
typedef struct playlist {
    cancion *canciones; /* Arreglo contiguo de canciones reservado en memoria dinamica */
    uint32_t pid;       /* Identificador unico asignado a la playlist */
    char    *nombre;    /* Cadena con el nombre de la lista, asignada con malloc */
    char    *ruta;      /* Ruta relativa del archivo CSV asociado, o NULL si es volatil */
    int      cantidad;  /* Numero de canciones cargadas actualmente en el arreglo */
} playlist;

/** @brief Estructura temporal y compacta para generacion de catalogos.
 *
 * Emplea indices numericos hacia tablas estaticas de palabras, artistas
 * y generos para generar temas de forma rapida y eficiente en memoria
 * antes de volcar la informacion formateada al archivo CSV definitivo.
 */
typedef struct {
    uint32_t dur;       /* Duracion generada aleatoriamente en segundos */
    uint32_t reps;      /* Numero aleatorio de reproducciones simuladas */
    uint16_t anio;      /* Anio de publicacion dentro del rango valido */
    uint8_t  p1;        /* Indice a la primera palabra del titulo */
    uint8_t  p2;        /* Indice a la segunda palabra del titulo */
    uint8_t  p3;        /* Indice a la primera palabra del album */
    uint8_t  p4;        /* Indice a la segunda palabra del album */
    uint8_t  artista;   /* Indice posicional dentro del arreglo constante de artistas */
    uint8_t  genero;    /* Indice posicional dentro del arreglo constante de generos */
} tema;

/** @brief Estructura descriptiva para la fonoteca musical del sistema.
 *
 * Representa la base general de almacenamiento del catalogo donde las
 * canciones se mantienen indexadas sin repeticion y ordenadas por identificador
 * numerico para permitir algoritmos de busqueda eficiente.
 */
typedef struct {
    char    *ruta_fonoteca; /* Ruta hacia el archivo principal de la fonoteca */
    uint32_t cantidad;      /* Total de canciones registradas en la fonoteca */
} fonoteca;

/* ----------------------- RUTAS DE ARCHIVOS ----------------------- */
/* Rutas relativas del sistema para persistencia de datos en disco */
static const char ruta_pl[]        = "./playlist.csv";
static const char ruta_catalogo[]  = "./catalogo.csv";
static const char ruta_historial[] = "./historial.csv";

/* ----------------------- FUNCIONES GENERALES ----------------------- */

/** @brief Genera un identificador numerico unico y secuencial.
 *
 * Abre el archivo CSV de referencia en modo lectura y recorre caracter por
 * caracter contando la cantidad de saltos de linea presentes para determinar
 * el siguiente identificador libre. En caso de no existir el archivo, retorna 1.
 *
 * @return uint32_t correspondiente al siguiente ID autoincremental.
 */
uint32_t generar_id(void);

/** @brief Compara dos cadenas de texto ignorando mayusculas y minusculas.
 *
 * Recorre ambas cadenas de manera secuencial convirtiendo cada caracter a su
 * equivalente en minuscula mediante la funcion tolower(). Finaliza cuando encuentra
 * una discrepancia o cuando se alcanza el terminador nulo en alguna de las cadenas.
 *
 * @param a Puntero a la primera cadena de caracteres.
 * @param b Puntero a la segunda cadena de caracteres.
 * @return Retorna 1 si ambas cadenas son identicas en contenido, 0 si difieren o si alguna apunta a NULL.
 */
int compara_strings(char *a, char *b);

/* ----------------------- FUNCIONES CANCIONES ----------------------- */

/** @brief Simula o reproduce el audio de una pista musical.
 *
 * Inicializa el motor de audio miniaudio en memoria, ejecuta la reproduccion
 * del archivo apuntado por ruta y aguarda la confirmacion del usuario por consola.
 * Al concluir la reproduccion, desinicializa el motor para liberar recursos del
 * sistema e incrementa en una unidad el contador de reproducciones de la cancion.
 *
 * @param ruta Direccion del archivo de audio compatible en el sistema de archivos.
 * @param c Puntero a la cancion en reproduccion para actualizar su contador total_rep.
 * @return Retorna 0 en caso de exito, -1 si el motor de audio falla al inicializarse.
 */
int reproduir_musica(const char *ruta, cancion *c);

/** @brief Reserva e inicializa una estructura de cancion en memoria.
 *
 * Asigna dinamicamente mediante malloc el tamano exacto de memoria para cada una
 * de las cadenas pasadas por parametro y las copia con strcpy. Configura ademas
 * campos predeterminados para duracion, reproducciones y anio de publicacion.
 *
 * @param titulo Cadena constante con el titulo de la cancion.
 * @param artista Cadena constante con el nombre del artista o banda.
 * @param album Cadena constante con el nombre del album discografico.
 * @param genero Cadena constante con el genero musical.
 * @return Estructura cancion con sus punteros de texto apuntando a memoria asignada.
 */
cancion crear_cancion(const char titulo[], const char artista[], const char album[], const char genero[]);

/** @brief Libera los bloques de memoria dinamica de una cancion.
 *
 * Aplica la instruccion free a cada uno de los cuatro punteros de cadena
 * (titulo, artista, album, genero) asignados previamente con memoria en el heap,
 * evitando fugas de memoria al descartar canciones individuales.
 *
 * @param c Puntero a la estructura de la cancion a destruir.
 */
void eliminar_cancion(cancion *c);

/* ----------------------- FUNCIONES CATALOGO ----------------------- */

/** @brief Asigna valores aleatorios a los campos de una estructura tema.
 *
 * Genera valores dentro de rangos acotados mediante rand() para duracion en segundos,
 * anio de publicacion, cantidad de reproducciones e indices a arreglos constantes
 * de palabras para armar titulos y albumes, asi como artistas y generos.
 *
 * @param t Puntero a la estructura tema receptora de los datos generados.
 */
void llenar(tema *t);

/** @brief Valida que los campos numericos de un tema esten dentro de los limites reales.
 *
 * Evalua que la duracion se encuentre entre DUR_MIN y DUR_MAX, que el anio este
 * comprendido entre ANIO_MIN y ANIO_MAX, y que las reproducciones no excedan REP_MAX.
 *
 * @param t Puntero constante al tema cuyos datos numericos seran evaluados.
 * @return Retorna 1 si todos los atributos cumplen los limites, 0 si alguno es invalido.
 */
int es_valido(const tema *t);

/** @brief Algoritmo para barajar de forma aleatoria .
 *
 * Itera desde el final del arreglo hacia el inicio, seleccionando un indice aleatorio
 * entre 0 e i, e intercambiando la estructura actual con la seleccionada
 *
 * @param v Arreglo contiguo de temas en memoria a ser mezclado.
 * @param n Cantidad total de elementos dentro del arreglo.
 */
void mezclar(tema *v, int n);

/** @brief Comprueba si un archivo existe en el sistema de almacenamiento.
 *
 * Intenta abrir el archivo especificado en modo lectura ("r"). Si la llamada a
 * fopen retorna distinto de NULL, el archivo existe y es accesible; luego se cierra.
 *
 * @param ruta Cadena con la ruta absoluta o relativa del archivo a verificar.
 * @return Retorna 1 si el archivo existe y es legible, 0 en caso contrario.
 */
int existe(const char *ruta);

/** @brief Escribe n registros aleatorios en un archivo CSV empleando un archivo temporal.
 *
 * Reserva memoria dinamica para n estructuras tema, las inicializa aleatoriamente
 * asegurando consistencia numerica, las mezcla y las escribe en
 * un archivo con extension .tmp. Si todo el proceso y el cierre de archivo ocurren
 * sin errores I/O, renombra atomicamente el archivo temporal a la ruta destino final.
 *
 * @param ruta Nombre o camino del archivo CSV a generar.
 * @param n Cantidad de registros musicales que seran generados.
 * @return Retorna 0 en caso de exito, -1 en parametros invalidos, -2 por falta de memoria RAM, -3 en errores de escritura en disco.
 */
int escribir_csv(const char *ruta, int n);

/** @brief Genera el catalogo aleatorio en disco unicamente si no existe previamente.
 *
 * Verifica mediante existe() si el archivo de catalogo ya reside en el almacenamiento.
 * En caso negativo, procede a llamar a escribir_csv() para sintetizar los n temas.
 *
 * @param ruta Ruta del archivo CSV que almacena el catalogo general.
 * @param n Cantidad de canciones a generar en caso de ausencia del archivo.
 * @return Retorna 0 si ya existia o si se genero correctamente, valor negativo si ocurrio una falla.
 */
int generar_catalogo_si_no_existe(const char *ruta, int n);

/* ----------------------- FUNCIONES PLAYLIST ----------------------- */

/** @brief Verifica la existencia fisica del archivo CSV de la playlist en disco.
 *
 * Intenta abrir el archivo definido por ruta_pl en modo lectura ("r").
 *
 * @return Retorna 1 si el archivo existe en disco y puede abrirse, 0 si no existe.
 */
int existe_plcsv(void);

/** @brief Duplica una cadena de caracteres reservando memoria dinamica con malloc.
 *
 * Calcula el tamano de la cadena origen mediante strlen, solicita memoria para
 * alojar todos los bytes mas el caracter nulo de terminacion '\0' y copia el
 * contenido mediante strcpy para desacoplar buffers temporales.
 *
 * @param s Puntero a la cadena de texto de origen.
 * @return Puntero a la nueva zona de memoria reservada, o NULL en caso de error de memoria.
 */
char *copiar_cadena(const char *s);

/** @brief Carga las canciones desde un archivo CSV a la estructura playlist.
 *
 * Para cierto archivo de ruta, se busca cargar todos sus datos a una playlist;
 * primero se cuentan las lineas que tiene el archivo mediante fgets. Sabiendo
 * el numero de canciones se reserva la memoria contigua con malloc. Se procesa
 * cada linea usando strtok con separadores ";\\n" guardando los punteros en un
 * arreglo temporal char *t[8]. Se verifica que se hayan extraido exactamente los
 * 8 campos requeridos; de cumplirse, los datos numericos se transforman a enteros
 * con strtoul y los textos se copian en el heap con copiar_cadena().
 *
 * @param pl Puntero a la estructura playlist donde se cargaran las canciones.
 * @param ruta Ruta del archivo CSV en disco desde donde se leeran los datos.
 * @return Retorna 0 en exito, 121 si no fue posible abrir el archivo para lectura.
 */
int playlist_cargar_csv(playlist *pl, const char *ruta);

/** @brief Libera la totalidad de la memoria dinamica asociada a una playlist.
 *
 * Recorre todas las canciones de la lista liberando los cuatro punteros de cadena
 * (titulo, artista, album, genero) mediante free(). Posteriormente libera el arreglo
 * contiguo pl->canciones y la cadena con el nombre de la playlist pl->nombre.
 *
 * @param pl Puntero a la playlist que sera desasignada y limpiada de memoria.
 */
void liberar_pl(playlist *pl);

/** @brief Crea una playlist vacia reservando memoria para una capacidad determinada.
 *
 * Inicializa los campos de control (pid en 0, punteros a NULL, cantidad en 0) y
 * reserva memoria dinamica contigua con malloc para alojar capacidad estructuras cancion.
 *
 * @param capacidad Cantidad maxima de canciones que podra albergar el arreglo contiguo.
 * @return Estructura playlist con su bloque de memoria asignado o pl.canciones en NULL si falla malloc.
 */
playlist crear_playlist_vacia(int capacidad);

/** @brief Libera unicamente el arreglo dinamico de canciones de una lista.
 *
 * Libera con free el puntero pl->canciones y restablece la cantidad a 0, sin
 * liberar las cadenas de texto internas. Se utiliza cuando las canciones son
 * referencias compartidas pertenecientes al catalogo principal.
 *
 * @param pl Puntero a la playlist cuyo arreglo se desea liberar.
 */
void liberar_arreglo_playlist(playlist *pl);

/** @brief Imprime por salida estandar el listado tabular de canciones de una lista.
 *
 * Despliega un encabezado con el identificador de la playlist, nombre y total de canciones,
 * seguido por una tabla con formato de columnas de ancho fijo mostrando ID, Titulo,
 * Artista, Album, Duracion, Anio y Reproducciones acumuladas.
 *
 * @param pl Puntero de solo lectura a la playlist que se va a imprimir.
 */
void imprimir_playlist(const playlist *pl);

/** @brief Solicita datos al usuario e inicializa una nueva playlist.
 *
 * Genera un identificador numerico unico mediante generar_id(), solicita por entrada
 * estandar el nombre de la lista, elimina el salto de linea resultante de fgets,
 * reserva memoria para pl.nombre y, si el archivo de playlist existe en disco,
 * carga las canciones almacenadas invocando a playlist_cargar_csv().
 *
 * @return Estructura playlist completamente inicializada y lista para uso.
 */
playlist crear_playlist(void);

/** @brief Agrega una cancion al arreglo dinamico de la playlist y la anexa a su CSV.
 *
 * Abre el archivo CSV especificado en modo anexo ("a") e imprime una nueva linea
 * con los campos delimitados por punto y coma. Posteriormente, realoca memoria para
 * el arreglo contiguo pl->canciones, copia las canciones previas, asigna la nueva
 * cancion en la ultima posicion e incrementa pl->cantidad.
 *
 * @param ruta Ruta del archivo CSV donde se anexara el registro.
 * @param c Puntero a la estructura de la cancion que sera agregada.
 * @param pl Puntero a la playlist en memoria donde se insertara el elemento.
 * @return Retorna 0 en caso de exito, 121 si no fue posible abrir el archivo en disco.
 */
int agregar_cancion_playlist(const char *ruta, cancion *c, playlist *pl);

/** @brief Exporta el arreglo completo de una playlist o catalogo a un archivo CSV.
 *
 * Crea o trunca el archivo en la ruta especificada en modo escritura ("w") y recorre
 * iterativamente el arreglo contiguo de canciones, escribiendo cada elemento linea por
 * linea con formato "id;duracion;anio;reps;titulo;artista;album;genero\\n".
 *
 * @param pl Puntero de solo lectura a la playlist cuyos registros se guardaran.
 * @param ruta Ruta del archivo CSV de destino.
 * @return Retorna 0 si la escritura concluyo exitosamente, 1 si fallan punteros o apertura.
 */
int exportar_playlist(const playlist *pl, const char *ruta);

/* ----------------------- FUNCIONES COLA ----------------------- */

/** @brief Localiza una cancion dentro de una playlist segun su ID unico (cid).
 *
 * Realiza un recorrido secuencial desde el indice 0 hasta pl->cantidad - 1 buscando
 * una coincidencia exacta en el campo cid de las canciones.
 *
 * @param pl Puntero de solo lectura a la playlist donde se realizara la busqueda.
 * @param cid Identificador numerico unico que se desea ubicar.
 * @return Indice entero de la cancion en el arreglo, o -1 si el identificador no existe.
 */
int buscar_cid_playlist(const playlist *pl, uint32_t cid);

/** @brief Remueve una cancion en una posicion determinada desplazando los elementos contiguos.
 *
 * Recorre los elementos desde pos + 1 hasta pl->cantidad - 1 moviendo cada estructura
 * una posicion a la izquierda para mantener la contiguidad del arreglo, y decrementa
 * pl->cantidad en una unidad.
 *
 * @param pl Puntero a la playlist que sera modificada.
 * @param pos Indice en base 0 del elemento a remover.
 */
void quitar_de_playlist(playlist *pl, int pos);

/** @brief Inserta una cancion al inicio de la cola de reproduccion.
 *
 * Desplaza todos los elementos existentes una posicion a la derecha para dejar libre
 * el indice 0 y coloca la nueva cancion al principio de la cola. Verifica que no se
 * supere MAX_COLA y valida mediante buscar_cid_playlist() que no este duplicada.
 *
 * @param cola Puntero a la estructura playlist que opera como cola de reproduccion.
 * @param c Puntero a la cancion que se incorporara al inicio de la fila.
 * @return Retorna 0 en caso de exito, 1 si la cola esta llena, si es puntero invalido o si la cancion ya figuraba en la fila.
 */
int agregar_a_cola(playlist *cola, const cancion *c);

/** @brief Remueve una cancion de la cola especificando su posicion en la fila.
 *
 * Valida que la posicion requerida se encuentre en el rango [0, cola->cantidad - 1]
 * y delega la operacion de eliminacion y desplazamiento a quitar_de_playlist().
 *
 * @param cola Puntero a la cola de reproduccion.
 * @param pos Indice del elemento que se desea extraer.
 * @return Retorna 0 en exito, 1 si el indice esta fuera de rango o el puntero es nulo.
 */
int quitar_de_cola_pos(playlist *cola, int pos);

/** @brief Remueve una cancion de la cola buscando por su identificador unico cid.
 *
 * Obtiene el indice del elemento mediante buscar_cid_playlist() y, de existir, lo
 * elimina de la fila desplazando los elementos restantes.
 *
 * @param cola Puntero a la cola de reproduccion.
 * @param cid Identificador de la cancion que se quiere descartar.
 * @return Retorna 0 en caso de exito, 1 si la cancion no fue encontrada o la cola es invalida.
 */
int quitar_de_cola_id(playlist *cola, uint32_t cid);

/** @brief Vacía completamente los elementos de la cola de reproducción.
 *
 * Restablece a 0 la propiedad cantidad de la estructura cola, dejando libre la
 * fila para nuevas inserciones sin necesidad de liberar y reasignar memoria.
 *
 * @param cola Puntero a la playlist correspondiente a la cola de reproduccion.
 */
void vaciar_cola(playlist *cola);

/** @brief Reproduce la primera cancion de la cola y actualiza las estructuras del sistema.
 *
 * Extrae la cancion posicionada en el indice 0 de la cola, busca su registro dentro del
 * catalogo general para incrementar su total de reproducciones (total_rep), la anade
 * al historial de reproduccion y muestra por pantalla el mensaje de reproduccion activa.
 *
 * @param cola Puntero a la cola de reproduccion.
 * @param catalogo Puntero al catalogo general donde se actualizara el total_rep.
 * @param historial Puntero a la estructura historial donde se registrara la pista.
 * @return Retorna 0 en exito, 1 si la cola esta vacia, si los punteros son NULL o si la cancion no reside en el catalogo.
 */
int reproducir_de_cola(playlist *cola, playlist *catalogo, playlist *historial);

/* ----------------------- FUNCIONES HISTORIAL ----------------------- */

/** @brief Reserva memoria e inicializa la estructura de historial de reproduccion.
 *
 * Configura una playlist con campos iniciales vacios y reserva memoria dinamica
 * contigua para almacenar un maximo acotado de MAX_HISTORIAL canciones.
 *
 * @return Estructura playlist dimensionada para trabajar como historial.
 */
playlist crear_historial(void);

/** @brief Localiza una cancion dentro del historial mediante su identificador cid.
 *
 * Recorre linealmente el arreglo de canciones del historial comparando el campo
 * cid de cada entrada contra el identificador buscado.
 *
 * @param h Puntero de solo lectura al historial de reproduccion.
 * @param cid Identificador numerico de la cancion.
 * @return Indice en el historial si es localizada, o -1 si no se encuentra registrada.
 */
int buscar_en_historial(const playlist *h, uint32_t cid);

/** @brief Remueve una cancion del historial desplazando los elementos a la izquierda.
 *
 * Desplaza las canciones desde pos + 1 hasta h->cantidad - 1 hacia la izquierda
 * sobreescribiendo el elemento en pos y decrementando el total de canciones registradas.
 *
 * @param h Puntero a la estructura de historial.
 * @param pos Indice en base 0 del elemento que sera eliminado.
 */
void quitar_de_historial(playlist *h, int pos);

/** @brief Inserta una cancion en la ultima posicion del historial.
 *
 * Verifica si la cancion ya habia sido reproducida previamente; si es asi, la remueve
 * de su ubicacion previa para reubicarla al final. Si el historial ha alcanzado
 * MAX_HISTORIAL elementos, descarta la cancion mas antigua ubicada en la posicion 0
 * y corre las demas antes de almacenar la nueva cancion en h->cantidad.
 *
 * @param h Puntero al historial de canciones reproducidas.
 * @param c Puntero a la cancion que acaba de ser reproducida.
 * @return Retorna 0 en caso exitoso, 1 si el puntero al historial o la cancion son invalidos.
 */
int agregar_a_historial(playlist *h, const cancion *c);

/** @brief Libera la memoria del arreglo dinamico reservado para el historial.
 *
 * Aplica free al arreglo h->canciones y resetea h->cantidad a 0 sin desasignar
 * las cadenas de texto, dado que son referencias pertenecientes al catalogo.
 *
 * @param h Puntero a la estructura de historial a liberar.
 */
void liberar_historial(playlist *h);

/** @brief Verifica la presencia fisica del archivo de historial en disco.
 *
 * Intenta abrir ruta_historial en modo lectura ("r"). Si no existe, invoca a
 * escribir_archivo_historial() para inicializarlo en disco.
 *
 * @return Retorna 0 si el archivo existe o pudo crearse correctamente, 1 si se produce una falla.
 */
int existe_historial(void);

/** @brief Inicializa o limpia el archivo fisico del historial en disco.
 *
 * Abre o crea el archivo ruta_historial usando el modo de apertura "w", dejandolo
 * completamente vacio para un nuevo ciclo de ejecucion o borrado del historial.
 *
 * @return Retorna 0 si el archivo fue creado y cerrado correctamente, 1 si fopen falla.
 */
int escribir_archivo_historial(void);

/* ----------------------- FUNCIONES BUSQUEDA ----------------------- */

/** @brief Busqueda binaria en version recursiva para localizar una cancion por cid.
 *
 * Asume que el arreglo pl->canciones se encuentra ordenado ascendentemente por cid.
 * Calcula el punto medio evitando desbordamientos aritmeticos; si el elemento coincide
 * retorna su posicion, de lo contrario se invoca recursivamente sobre la mitad
 * izquierda o derecha segun el valor de cid_buscado respecto al elemento central.
 *
 * @param pl Puntero a la playlist ordenada por identificador.
 * @param izq Indice inferior del rango de busqueda.
 * @param der Indice superior del rango de busqueda.
 * @param cid_buscado Identificador numerico exacto que se desea encontrar.
 * @return Indice de la cancion en el arreglo, o -1 si no fue localizada dentro del intervalo.
 */
int binsearch_cancion(playlist *pl, int izq, int der, uint32_t cid_buscado);

/** @brief Busca linealmente una cancion por titulo ignorando mayusculas y minusculas.
 *
 * Recorre secuencialmente el arreglo de canciones comparando el titulo mediante
 * compara_strings(). Retorna inmediatamente al hallar la primera coincidencia exacta.
 *
 * @param pl Puntero a la estructura donde se efectuara la busqueda.
 * @param titulo_buscado Cadena de texto con el titulo de la cancion requerida.
 * @return Indice de la primera cancion coincidente, o -1 si no existe.
 */
int search_titulo(playlist *pl, char *titulo_buscado);

/** @brief Localiza todas las canciones de un artista y guarda sus indices en un arreglo.
 *
 * Recorre secuencialmente la lista comparando pl->canciones[i].artista con la cadena
 * suministrada. Cuando hay coincidencia, guarda el indice en el arreglo resultados[]
 * siempre que no se sobrepase max_resultados, contabilizando el total encontrado.
 *
 * @param pl Puntero a la estructura con las canciones.
 * @param artista_buscado Nombre del artista o conjunto que se desea rastrear.
 * @param resultados Arreglo de enteros donde se guardaran los indices de las coincidencias.
 * @param max_resultados Limite maximo de elementos que pueden ser escritos en resultados[].
 * @return Cantidad total de canciones que coincidieron con el artista buscado.
 */
int search_artista(playlist *pl, char *artista_buscado, int *resultados, int max_resultados);

/** @brief Localiza canciones pertenecientes a un genero y recopila sus indices.
 *
 * Recorre linealmente el catalogo contrastando el genero de cada pista musical con
 * genero_buscado, almacenando los indices correspondientes dentro del arreglo de salida.
 *
 * @param pl Puntero a la playlist o catalogo a inspeccionar.
 * @param genero_buscado Nombre del genero a localizar.
 * @param resultados Arreglo de enteros para almacenar los indices encontrados.
 * @param max_resultados Cantidad maxima admisible en el arreglo de enteros.
 * @return Total de canciones que coincidieron con el criterio de genero.
 */
int search_genero(playlist *pl, char *genero_buscado, int *resultados, int max_resultados);

/** @brief Busca todas las canciones asociadas a un album discografico especifico.
 *
 * Compara secuencialmente el campo album de cada cancion almacenada y recolecta los
 * indices correspondientes en el arreglo de resultados.
 *
 * @param pl Puntero a la lista de canciones a filtrar.
 * @param album_buscado Nombre del album a comparar.
 * @param resultados Arreglo donde se registraran los indices resultantes.
 * @param max_resultados Limite superior de indices a almacenar.
 * @return Cantidad total de pistas que coinciden con el album indicado.
 */
int search_album(playlist *pl, char *album_buscado, int *resultados, int max_resultados);

/** @brief Centraliza la busqueda de canciones bajo diferentes criterios de seleccion.
 *
 * Permite ejecutar busquedas segun el parametro de prioridad: 1 para busqueda binaria
 * por ID (requiere orden ascendente de cid), 2 para busqueda por titulo, y 3 para
 * busqueda por artista. Retorna una estructura vacia si no hay coincidencias.
 *
 * @param pl Puntero a la lista de canciones donde se buscara.
 * @param prioridad Codigo numerico del criterio (1: cid, 2: titulo, 3: artista).
 * @param dato_numerico Identificador numerico utilizado en caso de prioridad == 1.
 * @param dato_texto Cadena de texto utilizada para contrastar titulo o artista.
 * @return Estructura cancion con la pista localizada, o estructura inicializada en cero si falla.
 */
cancion busqueda_global_lenta(playlist *pl, int prioridad, unsigned int dato_numerico, char *dato_texto);

/** @brief Gestiona la interaccion de busqueda por artista desde consola.
 *
 * Solicita el nombre del artista por teclado, procesa el buffer de entrada descartando
 * el salto de linea final, invoca a search_artista() y presenta los resultados.
 *
 * @param pl Puntero a la playlist o catalogo donde se efectuara la consulta.
 */
void ejecuta_busqueda_artista(playlist *pl);

/** @brief Imprime en la consola el listado de artistas disponibles sin repetir nombres.
 *
 * Recorre todas las canciones verificando si el artista ya aparecio en una posicion
 * anterior del arreglo mediante comparacion de cadenas; si no habia aparecido, lo
 * imprime por pantalla garantizando una lista de nombres unicos.
 *
 * @param pl Puntero de solo lectura al catalogo general de canciones.
 */
void listar_artistas(const playlist *pl);

/** @brief Solicita un genero al usuario por teclado, cuenta las ocurrencias y lista las canciones.
 *
 * Pide la cadena del genero por consola, reserva dinamicamente un arreglo auxiliar
 * para almacenar los indices coincidentes, invoca a search_genero() para obtener
 * el conteo y recorre los resultados imprimiendo artista y titulo de cada cancion.
 *
 * @param pl Puntero a la estructura donde se consultaran los generos.
 */
void interactuar_genero(playlist *pl);

/* ----------------------- FUNCIONES ORDENAMIENTO ----------------------- */

/** @brief Ordena el arreglo de canciones in-situ mediante el algoritmo Insertion Sort iterativo.
 *
 * Recorre la coleccion desde el segundo elemento (i = 1 hasta n - 1), almacenando la
 * estructura actual en una variable de respaldo (key) y desplazando hacia la derecha
 * aquellos elementos previos que resulten mayores segun el criterio configurado:
 * 0 para cid, 1 para duracion, 2 para anio, 3 para total_rep, 4 para titulo,
 * 5 para artista, 6 para album, y 7 para genero.
 *
 * @param pl Puntero a la playlist cuyo arreglo de canciones sera ordenado en memoria.
 * @param criterio Criterio de ordenamiento utilizado para guiar las comparaciones.
 */
void insertion_sort_int(playlist *pl, int criterio);

/** @brief Fusiona ordenadamente dos subarreglos contiguos de canciones en O(n).
 *
 * Reserva memoria dinamica temporal para dos arreglos de canciones auxiliares (subarr_left
 * y subarr_right), copia los elementos de ambas mitades delimitadas por low, med y high,
 * y luego intercala secuencialmente los elementos de menor duracion de regreso en el
 * arreglo original p->canciones, liberando la memoria auxiliar al concluir.
 *
 * @param p Puntero a la playlist contenedora del arreglo de canciones.
 * @param low Indice de inicio del primer subarreglo.
 * @param med Indice intermedio de division entre ambas mitades.
 * @param high Indice de finalizacion del segundo subarreglo.
 * @param criterio Criterio de ordenamiento utilizado para guiar las comparaciones.
 */
void merge(playlist *p, int low, int med, int high, int criterio);

/** @brief Ordena el arreglo de canciones recursivamente aplicando Merge Sort.
 *
 * Aplica el paradigma divide y venceras: calcula el punto medio entre los indices
 * low y high, se llama recursivamente para ordenar la mitad izquierda y la mitad
 * derecha por separado, y finalmente combina ambas secuencias mediante la funcion merge().
 *
 * @param p Puntero a la estructura playlist que sera ordenada recursivamente.
 * @param low Limite inferior del subarreglo actual.
 * @param high Limite superior del subarreglo actual.
 * @param criterio Criterio de ordenamiento utilizado para guiar las comparaciones.
 */
void merge_sort(playlist *p, int low, int high, int criterio);

/* ----------------------- FUNCIONES RANKING ----------------------- */

/** @brief Despliega el ranking de las N canciones con mayor reproduccion del sistema.
 *
 * 
 * @param pl Puntero de solo lectura a la estructura con las canciones a rankear.
 */
void canciones_por_genero(const playlist *pl);

/** @brief Despliega el ranking de las N canciones con mayor reproduccion del sistema.
 *
 * Crea un arreglo auxiliar local de indices de tamano pl->cantidad para preservar el
 * orden original de la playlist. Aplica un ordenamiento burbuja descendente comparando
 * pl->canciones[idx[j]].total_rep e intercambiando unicamente los indices numericos.
 * Al terminar, imprime por pantalla las canciones en las primeras posiciones del ranking.
 *
 * @param pl Puntero de solo lectura a la estructura con las canciones a rankear.
 */
void canciones_mas_escuchada(const playlist *pl);

/** @brief Muestra por consola cada artista unico junto a su tema mas reproducido.
 *
 * Recorre iterativamente cada cancion del catalogo verificando si el artista ya fue
 * evaluado anteriormente. Si es la primera vez que aparece, busca a traves del resto del
 * catalogo cual es su pista con mayor total_rep e imprime el resultado.
 *
 * @param pl Puntero de solo lectura al catalogo musical.
 */
void mas_escuchado(const playlist *pl);

/** @brief Identifica la cancion con mas reproducciones segun criterio de artista o genero.
 *
 * Valida que los parametros sean consistentes y que campo valga 1 (filtro por artista)
 * o 2 (filtro por genero). Itera secuencialmente la lista evaluando las pistas que
 * coincidan con la cadena c, manteniendo el indice de aquella que presente el mayor
 * registro en su campo total_rep.
 *
 * @param pl Puntero de solo lectura a la estructura de canciones.
 * @param c Cadena con el nombre exacto del artista o del genero que se evaluara.
 * @param campo Criterio de evaluacion (1: coincidencia por artista, 2: coincidencia por genero).
 * @return Indice de la cancion lider en reproducciones, o -1 si no hubo coincidencias o argumentos invalidos.
 */
int mas_escuchada_de(const playlist *pl, char *c, int campo);

/** @brief Obtiene el indice de la cancion mas escuchada de un artista determinado.
 *
 * Funcion envoltorio que delega la ejecucion a mas_escuchada_de() configurando el
 * parametro de campo en 1 para busqueda por artista.
 *
 * @param pl Puntero a la coleccion de canciones.
 * @param c Cadena que contiene el nombre del artista a consultar.
 * @return Indice de la cancion mas reproducida del artista, o -1 si no se encontro.
 */
int mas_escuchada_artista(const playlist *pl, char *c);

/** @brief Obtiene el indice de la cancion mas escuchada de un genero determinado.
 *
 * Funcion envoltorio que delega la ejecucion a mas_escuchada_de() configurando el
 * parametro de campo en 2 para busqueda por genero musical.
 *
 * @param pl Puntero a la coleccion de canciones.
 * @param c Cadena con el nombre del genero a consultar.
 * @return Indice de la cancion mas reproducida perteneciente al genero, o -1 si no existe.
 */
int mas_escuchada_genero(const playlist *pl, char *c);

/** @brief Muestra los creditos del proyecto. */
void mostrar_creditos(void);

/* ----------------------- FUNCIONES EXTRAS ----------------------- */
/** @brief Reproduce una playlist desde un archivo CSV.
 *
 * Lee el archivo playlist.csv, para cada cancion construye la ruta del archivo
 * de audio en ./assets/{artista} - {titulo}.mp3 y lo reproduce usando
 * reproduir_musica. Solo esta parte reproduce musica de verdad.
 */
void reproducir_playlist_csv(void);

/** @brief Funcion de pausa. */
void pausar(void);

/** @brief Reproduce el audio de una cancion. */
int reproducir_musica(const char *ruta, cancion *c);

/** @brief Muestra los creditos del proyecto. */
void mostrar_creditos(void);

#endif /* CANCIONES */
