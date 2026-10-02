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
/* Limpia la terminal. */
void limpiar_pantalla(void);

/* Imprime la cancion actual en la terminal. */
void imprime_cancion(cancion *c);

/* Imprime el menu de opciones en la terminal. */
void imprime_menu(void);

/* Lee una entrada del usuario en la terminal. */
char ui_input(void);

/* Funcion principal de la interfaz de usuario. */
int ui_principal(void);

/* Pausa la ejecucion del programa hasta que el usuario presione Enter. */
void ui_pausa(void);

/* ----------FUNCIONES UI MENU ---------*/
/* Play/Pause */
cancion *ui_toggle_play_pause(cancion *actual, int *en_pausa, playlist *cola, playlist *pl, playlist *historial);

/* Fila de reproduccion */
void ui_menu_fila(playlist *cola, playlist *catalogo);

/* Historial */
void ui_mostrar_historial(const playlist *historial);

/* Busqueda */
void ui_menu_busqueda(playlist *catalogo);
/* Ordenar */
void ui_menu_ordenar(playlist *catalogo);

/* Exportar */
void ui_menu_exportar(const playlist *catalogo);

/* ----------FUNCIONES UI BUSQUEDA ---------*/
/* Funcion UI para pedir el artista a buscar */
void ui_pedir_artista(void);
void ui_resultados_busqueda(int encontrados, char *artista);
void ui_cancion_busqueda(int num, cancion *c);
void ui_sin_resultados(char *artista);

#endif
