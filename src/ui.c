#include <stdio.h>
#include "songs.h"

/*
 * Idea: While principal, interfaz con colores y ascii, etc.
 */
void limpiar_pantalla(void)
{
    printf("\033[H\033[J");
}

void render_cancion(cancion *c)
{
    printf("\n");
    printf(COLOR_CYAN "  %s - %s\n" COLOR_RESET, c->artista, c->titulo);
}

/*
void render_playlist(playlist *pl, int indice)
{
}

void ui_input(void)
{
    // linea del input
    char input[32];
    printf(COLOR_CYAN "  >> " COLOR_RESET);

    if (fgets(input, sizeof(input), stdin) == NULL)
    {
        return '\0';
    }
    return input[0];
}

*/

void render_menu(void)
{
    printf("\n");
    printf(COLOR_BLUE "  [P]" COLOR_RESET " Play/Pause   ");
    printf(COLOR_BLUE "[N]" COLOR_RESET " Siguiente   ");
    printf(COLOR_BLUE "[B]" COLOR_RESET " Anterior   ");
    printf(COLOR_BLUE "[L]" COLOR_RESET " Cargar CSV   ");
    printf(COLOR_RED  "[Q]" COLOR_RESET " Salir\n");
    printf(COLOR_CYAN "  >> " COLOR_RESET);
}

int ui_principal(void)
{
    char input[8];
    int running = 1;
    while (running)
    {
        limpiar_pantalla();
        render_menu();
    }
}
