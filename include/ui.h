#ifndef UI_H
#define UI_H

#include "songs.h"

/* CODIGOS ANSI DE LOS COLORES, DEBEN IR ACOMPAÑADOS DE UN COLOR_RESET TERMINANDO FRASES */
#define COLOR_RESET   "\033[0m"
#define COLOR_BOLD    "\033[1m"
#define COLOR_CYAN    "\033[1;36m"
#define COLOR_GREEN   "\033[1;32m"
#define COLOR_YELLOW  "\033[1;33m"
#define COLOR_RED     "\033[1;31m"
#define COLOR_MAGENTA "\033[1;35m"
#define COLOR_BLUE    "\033[1;34m"
#define COLOR_DIM     "\033[2m"

#define MAX_LINE 256

/* ----------FUNCIONES UI ---------*/
/** @brief Limpia la terminal. */
void limpiar_pantalla(void);

/** @brief Imprime la cancion actual en la terminal. */
void imprime_cancion(cancion *c);

/** @brief Imprime el menu de opciones en la terminal. */
void imprime_menu(void);

/** @brief Lee una entrada del usuario en la terminal. */
char ui_input(void);

/** @brief Funcion principal de la interfaz de usuario. */
int ui_principal(void);

/** @brief Pausa la ejecucion del programa [no es Play/pause] hasta que el usuario presione Enter. */
void ui_pausa(void);

/* ----------FUNCIONES UI MENU ---------*/
/** @brief Play/Pause */
cancion *ui_toggle_play_pause(cancion *actual, int *en_pausa, playlist *cola, playlist *pl, playlist *historial);

/** @brief Fila de reproduccion */
void ui_menu_fila(playlist *cola, playlist *catalogo);

/** @brief Historial */
void ui_menu_historial(const playlist *historial);

/* Busqueda */

/** @brief Muestra el menu de busqueda */
void ui_menu_busqueda(playlist *catalogo);

/** @brief Muestra el menu de ordenar */
void ui_menu_ordenar(playlist *catalogo);

/** @brief Muestra el menu de exportar */
void ui_menu_exportar(const playlist *catalogo);

/** @brief Muestra el menu de ranking */
void ui_menu_ranking(const playlist *pl);

/* ----------FUNCIONES UI BUSQUEDA ---------*/
/** @brief Pide el artista a buscar */
void ui_pedir_artista(void);
/** @brief Muestra los resultados de busqueda por artista */
void ui_resultados_busqueda(int encontrados, char *artista);
/** @brief Muestra una cancion encontrada en la busqueda */
void ui_cancion_busqueda(int num, cancion *c);
/** @brief Muestra mensaje de no resultados */
void ui_sin_resultados(char *artista);

#endif
/** @brief Muestra el menu de extras */
void ui_menu_extras(playlist *catalogo);
