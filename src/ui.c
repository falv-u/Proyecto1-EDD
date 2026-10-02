/* Idea: While principal, interfaz con colores y ascii, etc.*/
// FALTA UN CLI, reproducir, pausar
// LS para listar las canciones completas (Panchito)
// Crear y eliminar playlists/cancion: Orquestado

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "ui.h"

/* Prototipos de funciones externas ya implementadas */
int agregar_a_cola(playlist *cola, const cancion *c);
int quitar_de_cola_pos(playlist *cola, int pos);
int vaciar_cola(playlist *cola);
int reproducir_de_cola(playlist *cola, playlist *catalogo, playlist *historial);
int buscar_cid_playlist(const playlist *pl, uint32_t cid);
void insertion_sort_int(playlist *pl, int criterio);
void ejecuta_busqueda_artista(playlist *pl);

/* Falta funcion de play/pause */
cancion *ui_toggle_play_pause(cancion *actual, int *en_pausa, playlist *cola, playlist *pl, playlist *historial)
{
    int pos;
    
    // 1)Si no hay sonando, reproduce la primera de la fila
    if (actual == NULL)
    {
        if (cola == NULL || cola->cantidad == 0)
        {
            printf(COLOR_YELLOW "\n  [La fila esta vacia. Usa [F] para agregar canciones.]" COLOR_RESET);
            return NULL;
        }

        pos = buscar_cid_playlist(pl, cola->canciones[0].cid);
        reproducir_de_cola(cola, pl, historial);
        *en_pausa = 0;

        if (pos != -1)
            return &pl->canciones[pos];
        return NULL;
    }

    // 2)Si hay sonando, cambia entre play/pause
    if (*en_pausa == 0)
    {
        printf(COLOR_YELLOW "\n  [Pausando: %s - %s]" COLOR_RESET, actual->artista, actual->titulo);
        *en_pausa = 1;
        return actual;
    }
    else
    {
        printf(COLOR_CYAN "\n  [Reanuda: %s - %s]" COLOR_RESET, actual->artista, actual->titulo);
    }
    return actual;
}

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
    printf("  \t" COLOR_BOLD "Reproducir:\n" COLOR_RESET);

    // Opciones reproduccion
    printf(COLOR_BLUE "  [P]" COLOR_RESET " Play/Pause   ");
    printf(COLOR_BLUE "  [F]" COLOR_RESET " Cola/Fila   ");
    printf(COLOR_BLUE   "  [H]" COLOR_RESET " Historial  ");
    printf(COLOR_BLUE "[N]" COLOR_RESET " Sig.   ");
    printf(COLOR_BLUE "[B]" COLOR_RESET " Ant.   ");

    printf("\n  \t" COLOR_BOLD "Buscar:\n" COLOR_RESET);
    // Opciones Busqueda
    printf(COLOR_MAGENTA "  [S]" COLOR_RESET " ID/Artista   ");
    printf(COLOR_MAGENTA "  [A]" COLOR_RESET " Artista     ");
    printf(COLOR_MAGENTA "  [G]" COLOR_RESET " Generos  ");
    printf(COLOR_MAGENTA "  [R]" COLOR_RESET " Ranking  ");

    printf("\n  \t" COLOR_BOLD "Opciones:\n" COLOR_RESET);
    // Opciones Catalogo
    printf(COLOR_MAGENTA "  [T]" COLOR_RESET " Ordenar      ");
    printf(COLOR_MAGENTA "  [E]" COLOR_RESET " Exportar      ");

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

/* Funciones UI para lista de canciones */
/* Submenu fila de reproduccion*/
void ui_menu_fila(playlist *cola, playlist *catalogo)
{
    char buf[64];
    printf(COLOR_CYAN "\n === Fila [%d canciones] ===\n" COLOR_RESET, cola->cantidad);
    for (int i = 0; i < cola->cantidad; i++)
    {
        printf("    %d. [ID %u] %s - %s\n", i + 1, cola->canciones[i].cid, cola->canciones[i].artista, cola->canciones[i].titulo);
    }

    if (cola->cantidad == 0)
    {
        printf(COLOR_YELLOW "\n  [La fila esta vacia. Usa [F] para agregar canciones.]" COLOR_RESET);
    }
}

/* Submenu historial */
void ui_mostrar_historial(const playlist *historial)
{
    printf(COLOR_CYAN "\n === Historial [%d canciones] ===\n" COLOR_RESET, historial->cantidad);
    for (int i = 0; i < historial->cantidad; i++)
    {
        printf("  - %s - %s (Reps: %u)\n", historial->canciones[i].artista, historial->canciones[i].titulo, historial->canciones[i].total_rep);
    }

    if (historial->cantidad == 0)
        printf(COLOR_YELLOW "\n  [Sin canciones reproducidas/registradas aun.]" COLOR_RESET);
    ui_pausa();
}

/* Submenu busqueda */
void ui_menu_busqueda(playlist *catalogo)
{
    char buf[128];
    printf(COLOR_CYAN "\n  --- Busqueda ---\n" COLOR_RESET);
}

/* Submenu ordenar */
void ui_menu_ordenar(playlist *catalogo)
{
    char buf[32];
    printf(COLOR_CYAN "\n  --- Ordenar (Insertion Sort) ---\n" COLOR_RESET);
    printf(" [0] ID  [1] Duracion  [2] Ano  [3] Reproducciones\n >> ");

    if (fgets(buf, sizeof(buf), stdin) == NULL)
    {
    }
    ui_pausa();
}

/* Submenu exportar */
void ui_menu_exportar(const playlist *catalogo)
{
    char buf[128];
    printf(COLOR_CYAN "\n  --- Exportar a CSV ---\n" COLOR_RESET);

    ui_pausa();
}

