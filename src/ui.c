/* Idea: While principal, interfaz con colores y ascii, etc.*/

#include <stdio.h>
#include <string.h>
#include "songs.h"

void limpiar_pantalla(void)
{
    printf("\033[H\033[J");
}

void render_cancion(cancion *c)
{
    printf("\n");
    if (c != NULL && c->artista != NULL && c->titulo != NULL)
    {
        printf(COLOR_CYAN "  %s - %s\n" COLOR_RESET, c->artista, c->titulo);
    }
    else
    {
        printf(COLOR_YELLOW "\t[Sin reproducción activa]\n" COLOR_RESET);
    }
}

// creditos: panchito?
/*
void render_playlist(const playlist *pl)
{
	int i;

	if (pl == NULL || pl->cantidad == 0 || pl->canciones == NULL) {
		printf(COLOR_YELLOW " [Playlist vacia o no cargada.]\n" COLOR_RESET);
		return;
	}

	printf("Playlist #%u: %s\n", pl->pid, pl->nombre ? pl->nombre : "(sin nombre)");
	printf("Canciones: %d\n", pl->cantidad);
	printf("----------------------------------------------------------------------\n");
	printf("%-4s %-30s %-20s %-20s %-6s %-5s %-5s\n",
			"ID", "Titulo", "Artista", "Album", "Dur.", "Anho", "Rep");
	printf("----------------------------------------------------------------------\n");

	for (i = 0; i < pl->cantidad; i++) {
		const cancion *c = &pl->canciones[i];
		printf("%4u %30.30s %20.20s %20.20s %6u %-5u %-5u\n",
				c->cid,
				c->titulo  ? c->titulo  : "-",
				c->artista ? c->artista : "-",
				c->album   ? c->album   : "-",
				c->duracion,
				c->anio,
				c->total_rep);
	}
	printf("----------------------------------------------------------------------\n");
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
}

char ui_input(void)
{
    char input[32];
    printf(COLOR_CYAN "  >> " COLOR_RESET);

    if (fgets(input, sizeof(input), stdin) == NULL)
        return '\0';

    size_t len = strlen(input);
    size_t i = 0;
    // caso enter (\n)
    if (len <= 1)
        return '\0';

    // comprobar que el input solo contiene caracteres permitidos: 0-9, a-z, A-Z
    for (i = 0; i < len - 1; i++)
    {
        if (input[i] < 48 || (input[i] > 57 && input[i] < 97) || input[i] > 122)
            return '\0';
    }

    return input[0];
}

int ui_principal(void)
{
    int running = 1;
    while (running)
    {
        limpiar_pantalla();

        char c = ui_input();
        if (c == 'q' || c == 'Q')
            running = 0;
        render_menu();
    }
    return 0;
}
