/* Idea: While principal, interfaz con colores y ascii, etc.*/
// FALTA UN CLI, reproducir, pausar
// LS para listar las canciones completas (Panchito)
// Crear y eliminar playlists/cancion: Orquestado

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "ui.h"

/* Prototipos de funciones externas ya implementadas en otros .c */
// Cola y playlist
int agregar_a_cola(playlist *cola, const cancion *c);
int quitar_de_cola_pos(playlist *cola, int pos);
int reproducir_de_cola(playlist *cola, playlist *catalogo, playlist *historial);
int buscar_cid_playlist(const playlist *pl, uint32_t cid);
// busqueda y ordenamiento
void insertion_sort_int(playlist *pl, int criterio);
void ejecuta_busqueda_artista(playlist *pl);
// ranking
int mas_escuchada_artista(const playlist *pl, char *c);
int mas_escuchada_genero(const playlist *pl, char *c);

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
    // Limpieza de pantalla.
    printf("\033[H\033[J");
    system("clear");

    // Banner hecho en ASCII art (opcional)
    printf(COLOR_CYAN);
    printf(" .▄▄ · ▄• ▄▌• ▌ ▄ ·. • ▌ ▄ ·.  ▄· ▄▌\n");
    printf(" ▐█ ▀. █▪██▌·██ ▐███▪·██ ▐███▪▐█▪██▌\n");
    printf(" ▄▀▀▀█▄█▌▐█▌▐█ ▌▐▌▐█·▐█ ▌▐▌▐█·▐█▌▐█▪\n");
    printf(" ▐█▄▪▐█▐█▄█▌██ ██▌▐█▌██ ██▌▐█▌ ▐█▀·.\n");
    printf("  ▀▀▀▀  ▀▀▀ ▀▀  █▪▀▀▀▀▀  █▪▀▀▀  ▀ • \n" COLOR_RESET);
}

void imprime_cancion(cancion *c)
{
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
    printf(COLOR_BLUE "  [p]" COLOR_RESET " Play/Pause   ");
    printf(COLOR_BLUE "  [f]" COLOR_RESET " Cola/Fila   ");
    printf(COLOR_BLUE   "  [h]" COLOR_RESET " Historial  ");
    printf(COLOR_BLUE "[n]" COLOR_RESET " Sig.   ");
    printf(COLOR_BLUE "[b]" COLOR_RESET " Ant.   ");

    printf("\n  \t" COLOR_BOLD "Buscar:\n" COLOR_RESET);
    // Opciones Busqueda
    printf(COLOR_MAGENTA "  [s]" COLOR_RESET " ID/Artista   ");
    printf(COLOR_MAGENTA "  [g]" COLOR_RESET " Generos     ");
    printf(COLOR_MAGENTA "  [r]" COLOR_RESET " Ranking  ");

    printf("\n  \t" COLOR_BOLD "Opciones:\n" COLOR_RESET);
    // Opciones Catalogo
    printf(COLOR_MAGENTA "  [t]" COLOR_RESET " Ordenar      ");
    printf(COLOR_MAGENTA "  [e]" COLOR_RESET " Exportar      ");

    // Salir
    printf(COLOR_RED  "[q]" COLOR_RESET " Salir\n");
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
    printf(COLOR_CYAN "\n === Fila de rep. [%d canciones] ===\n" COLOR_RESET, cola->cantidad);
    for (int i = 0; i < cola->cantidad; i++)
    {
        printf("    %d. [ID %u] %s - %s\n", i + 1, cola->canciones[i].cid, cola->canciones[i].artista, cola->canciones[i].titulo);
    }

    if (cola->cantidad == 0) 
        printf(COLOR_YELLOW "\n  [La fila esta vacia. Usa [F] para agregar canciones.]" COLOR_RESET);

    printf("\n\tOpciones:\n(1) Agregar inicio por ID (2) Quitar 1ra (3) Vaciar (0) Volver\n");
    char opc = ui_input();

    if (opc == '1')
    {
        unsigned int id;
        printf("  Ingrese ID de la canción: ");
        if (scanf("%u", &id) == 1)
        {
            while (getchar() != '\n');
            int pos = buscar_cid_playlist(catalogo, id);
            if (pos != -1 && agregar_a_cola(cola, &catalogo->canciones[pos]) == 0)
            {
                printf(COLOR_GREEN "  Canción añadida.\n" COLOR_RESET);
            }
            else
            {
                printf(COLOR_RED "  Error: ID no es valido o ya esta agregada.\n" COLOR_RESET);
            }
        }
        ui_pausa();
    }
    else if (opc == '2')
    {
        quitar_de_cola_pos(cola, 0);
        printf(COLOR_GREEN "  Primera cancion retirada de la fila.\n" COLOR_RESET);
        ui_pausa();
    }
    else if (opc == '3')
    {
        vaciar_cola(cola);
        printf(COLOR_GREEN "  Fila vaciada.\n" COLOR_RESET);
        ui_pausa();
    }
}

/* Submenu historial */
void ui_mostrar_historial(const playlist *historial)
{
    printf(COLOR_CYAN "\n === Historial [%d canciones] ===\n" COLOR_RESET, historial->cantidad);
    for (int i = historial->cantidad - 1; i >= 0; i--)
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
    printf(COLOR_CYAN "\n  --- Busqueda ---\n" COLOR_RESET);
    printf("  [1] Buscar por ID [recursiva]\n");
    printf("  [2] Buscar por artista\n");
    printf("  [0] Salir\n");

    char opc = ui_input();
    if (opc == '1')
    {
        unsigned int id;
        printf("  Ingrese ID de la canción: ");
        if (scanf("%u", &id) == 1)
        {
            while (getchar() != '\n'); // eliminar enter de la cadena

            int pos = binsearch_cancion(catalogo, 0, catalogo->cantidad - 1, id);
            if (pos != -1)
            {
                printf(COLOR_GREEN "  Canción encontrada.\n" COLOR_RESET);
                printf(COLOR_CYAN "    %d. %s - %s\n" COLOR_RESET, pos + 1, catalogo->canciones[pos].artista, catalogo->canciones[pos].titulo);
            }
            else
            {
                printf(COLOR_RED "  No se encontró la canción. [Ordenar por ID con [T] primero]\n" COLOR_RESET);
            }
        }
        ui_pausa();
    }
    else if (opc == '2')
    {
        ui_pedir_artista();
        ejecuta_busqueda_artista(catalogo);
    }
    else if (opc == '3')
    {
        char titulo[100];
        printf(COLOR_CYAN "\n  >> Introduce el titulo a buscar: " COLOR_RESET);
        if (fgets(titulo, sizeof(titulo), stdin) != NULL)
        {
            // Eliminamos salto de linea y final de cadena
            int i = 0;
            while (titulo[i] != '\0')
            {
                if (titulo[i] == '\n')
                {
                    titulo[i] = '\0';
                    break;
                }
                i++;
            }

            int pos = search_titulo(catalogo, titulo);
            if (pos != -1)
            {
                printf(COLOR_GREEN "  Canción encontrada.\n" COLOR_RESET);
                printf(COLOR_CYAN "    %d. %s - %s\n" COLOR_RESET, pos + 1, catalogo->canciones[pos].artista, catalogo->canciones[pos].titulo);
            }
        }
        ui_pausa();
    }
}

/* Submenu ordenar */
void ui_menu_ordenar(playlist *catalogo)
{
    printf(COLOR_CYAN "\n  --- Ordenar [Insertion Sort] ---\n" COLOR_RESET);
    printf("  [0] ID  [1] Duracion  [2] Ano  [3] Reproducciones\n");
    
    char opc = ui_input();

    /* Convierte el caracter a entero para poder usarlo como indice */
    if (opc >= '0' && opc <= '3')
    {
        int criterio = opc - '0';
        insertion_sort_int(catalogo, criterio);
        printf(COLOR_GREEN "  Catalogo ordenado!\n" COLOR_RESET);
    }
    else
    {
        printf(COLOR_RED "  Opcion invalida.\n" COLOR_RESET);
    }
    ui_pausa();
}

/* Submenu exportar */
void ui_menu_exportar(const playlist *catalogo)
{
    char buf[100];
    FILE *archivo;

    printf(COLOR_CYAN "\n  --- Exportar a CSV ---\n" COLOR_RESET);
    printf("  Nombre del archivo (ej: playlist.csv): ");

    if (scanf("%99s", buf) == 1)
    {
        while (getchar() != '\n');
        archivo = fopen(buf, "w");
        if (archivo != NULL)
        {
            for (int i = 0; i < catalogo->cantidad; i++)
            {
                cancion *c = &catalogo->canciones[i];
                fprintf(archivo, "%u;%u;%u;%u;%s;%s;%s;%s\n",
                        c->cid, c->duracion, c->anio, c->total_rep,
                        c->titulo, c->artista, c->album, c->genero);
            }
            fclose(archivo);
            printf(COLOR_GREEN "  Archivo exportado con éxito -> %s.\n" COLOR_RESET, buf);
        }
        else
        {
            printf(COLOR_RED "  Error al abrir el archivo.\n" COLOR_RESET);
        }
    }
    ui_pausa();
}

/* Submenu ranking */
void ui_mostrar_ranking(const playlist *pl)
{
    
}