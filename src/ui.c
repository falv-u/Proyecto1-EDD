/* Idea: While principal, interfaz con colores y ascii, etc.*/
// FALTA UN CLI, reproducir, pausar
// LS para listar las canciones completas (Panchito)
// Crear y eliminar playlists/cancion: Orquestado

#include <stdio.h>
#include <string.h>
#include "ui.h"

void limpiar_pantalla(void)
{
    printf("\033[H\033[J");
}

void imprime_cancion(cancion *c)
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

void imprime_menu(void)
{
    printf("\n");

    // Opciones reproduccion
    printf(COLOR_BLUE "  [P]" COLOR_RESET " Play/Pause   ");
    printf(COLOR_BLUE "[N]" COLOR_RESET " Siguiente   ");
    printf(COLOR_BLUE "[B]" COLOR_RESET " Anterior   ");
    printf(COLOR_BLUE "[L]" COLOR_RESET " Cargar CSV   ");

    printf("\n  Ordenar por:\n");
    // Opciones catalogo y estadisticas
    printf(COLOR_MAGENTA "  [A]" COLOR_RESET " Artistas   ");
    printf(COLOR_MAGENTA "  [G]" COLOR_RESET " Generos   ");
    printf(COLOR_MAGENTA "  [R]" COLOR_RESET " Ranking  ");
    printf(COLOR_MAGENTA "  [S]" COLOR_RESET " Busqueda  ");

    // Salir
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
        imprime_menu();
    }
    return 0;
}

void ui_pausa(void)
{
    printf(COLOR_CYAN "\nPresione Enter para continuar...." COLOR_RESET);
    getchar();
}

/* Funciones UI para busqueda */

void ui_pedir_artista(void)
{
    printf(COLOR_CYAN "\n  >> Introduce el nombre del artista a buscar: " COLOR_RESET);
}

void ui_resultados_busqueda(int cantidad, char *artista)
{
    printf(COLOR_GREEN "\n  Encontrados %d resultados para el artista '%s'." COLOR_RESET, cantidad, artista);
}

void ui_cancion_busqueda(int indice, cancion *c)
{
    if (c != NULL && c->artista != NULL && c->titulo != NULL)
    {
        printf("    %d. %s - %s\n", indice, c->artista, c->titulo);
    }
}

void ui_sin_resultados(char *solicitud)
{
    printf(COLOR_RED "\n  Sin resultados para %s." COLOR_RESET, solicitud);
}