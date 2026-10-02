/* Idea: While principal, interfaz con colores y ascii, etc.*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include "ui.h"

/* Prototipos de funciones externas ya implementadas en otros .c */
/* Cola y playlist */
int agregar_a_cola(playlist *cola, const cancion *c);
int quitar_de_cola_pos(playlist *cola, int pos);
int reproducir_de_cola(playlist *cola, playlist *catalogo, playlist *historial);
int buscar_cid_playlist(const playlist *pl, uint32_t cid);
/* Busqueda y ordenamiento */
void insertion_sort_int(playlist *pl, int criterio);
void ejecuta_busqueda_artista(playlist *pl);
/* Ranking */
int mas_escuchada_artista(const playlist *pl, char *c);
int mas_escuchada_genero(const playlist *pl, char *c);

/** @brief Alterna el estado de reproduccion entre reproducir y pausar.
 *
 * Si no hay cancion activa, toma la primera cancion de la cola y la reproduce.
 * Si ya hay una cancion reproduciendose, alterna entre estado pausado y reanudado.
 *
 * @param actual Puntero a la cancion en reproduccion actualmente.
 * @param en_pausa Puntero a la bandera de estado de pausa (0: reproduciendo, 1: en pausa).
 * @param cola Puntero a la cola de reproduccion.
 * @param pl Puntero al catalogo principal de canciones.
 * @param historial Puntero a la lista de historial de reproduccion.
 * @return Puntero a la cancion que queda en reproduccion, o NULL si la cola esta vacia.
 */
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

/** @brief Limpia la pantalla de la terminal mediante secuencias de escape ANSI. */
void limpiar_pantalla(void)
{
	// Limpieza de pantalla.
	printf("\033[H\033[J");
	system("clear");
}

/** @brief Muestra en consola la informacion de la cancion en reproduccion.
 *
 * @param c Puntero a la cancion actualmente en reproduccion, o NULL si no hay ninguna.
 */
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

/** @brief Imprime el menu principal con todas las opciones disponibles y arte ASCII. */
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
	printf(COLOR_MAGENTA "  [e]" COLOR_RESET " Exportar    ");
	printf(COLOR_MAGENTA "  [x]" COLOR_RESET " Extras   ");
	printf(COLOR_MAGENTA "  [a]" COLOR_RESET " L.Artistas    ");

	// Salir
	printf(COLOR_RED  "[q]" COLOR_RESET " Salir\n");

	// Banner hecho en ASCII art (opcional)
	printf(COLOR_CYAN);
	printf(" .▄▄ · ▄• ▄▌• ▌ ▄ ·. • ▌ ▄ ·.  ▄· ▄▌\n");
	printf(" ▐█ ▀. █▪██▌·██ ▐███▪·██ ▐███▪▐█▪██▌\n");
	printf(" ▄▀▀▀█▄█▌▐█▌▐█ ▌▐▌▐█·▐█ ▌▐▌▐█·▐█▌▐█▪\n");
	printf(" ▐█▄▪▐█▐█▄█▌██ ██▌▐█▌██ ██▌▐█▌ ▐█▀·.\n");
	printf("  ▀▀▀▀  ▀▀▀ ▀▀  █▪▀▀▀▀▀  █▪▀▀▀  ▀ • \t" COLOR_RESET);
}

/** @brief Lee y valida un caracter de entrada del usuario por consola.
 *
 * @return El caracter ingresado si es valido (alfanumerico), o '\0' si es invalido.
 */
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
		char c = input[i];
		if (!((c >= '0' && c <= '9') || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')))
			return '\0';
	}

	return input[0];
}

/** @brief Bucle principal de la interfaz de usuario para interaccion por consola.
 *
 * @return Retorna 0 al finalizar la ejecucion.
 */
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

/** @brief Pausa la ejecucion del programa hasta que el usuario presione Enter. */
void ui_pausa(void)
{
	printf(COLOR_CYAN "[Presione Enter para continuar....]" COLOR_RESET);
	getchar();
}

/* Funciones UI para busqueda */

/** @brief Solicita al usuario ingresar el nombre de un artista por consola. */
void ui_pedir_artista(void)
{
	printf(COLOR_CYAN "\n  >> Introduce el nombre del artista a buscar: " COLOR_RESET);
}

/** @brief Muestra el total de coincidencias encontradas para la busqueda de un artista.
 *
 * @param cantidad Numero de canciones encontradas.
 * @param artista Nombre del artista buscado.
 */
void ui_resultados_busqueda(int cantidad, char *artista)
{
	printf(COLOR_GREEN "\n  Encontrados %d resultados para el artista '%s'." COLOR_RESET, cantidad, artista);
}

/** @brief Muestra una cancion individual perteneciente a los resultados de busqueda.
 *
 * @param indice Posicion o numero de orden en los resultados.
 * @param c Puntero a la cancion a mostrar.
 */
void ui_cancion_busqueda(int indice, cancion *c)
{
	if (c != NULL && c->artista != NULL && c->titulo != NULL)
	{
		printf("    %d. %s - %s\n", indice, c->artista, c->titulo);
	}
}

/** @brief Informa al usuario que no se hallaron resultados para el criterio solicitado.
 *
 * @param solicitud Cadena de texto que origino la busqueda sin resultados.
 */
void ui_sin_resultados(char *solicitud)
{
	printf(COLOR_RED "\n  Sin resultados para %s." COLOR_RESET, solicitud);
}

/* Funciones UI para lista de canciones */

/** @brief Muestra el submenu interactivo para la gestion de la cola/fila de reproduccion.
 *
 * Permite listar canciones en cola, agregar canciones por ID, retirar la primera,
 * retirar por posicion o ID, y vaciar la cola por completo.
 *
 * @param cola Puntero a la playlist que representa la cola de reproduccion.
 * @param catalogo Puntero al catalogo principal de canciones disponibles.
 */
void ui_menu_fila(playlist *cola, playlist *catalogo)
{
	printf(COLOR_CYAN "\n === Fila de rep. [%d canciones] ===\n" COLOR_RESET, cola->cantidad);
	for (int i = 0; i < cola->cantidad; i++)
	{
		printf("    %d. [ID %u] %s - %s\n", i + 1, cola->canciones[i].cid, cola->canciones[i].artista, cola->canciones[i].titulo);
	}

	if (cola->cantidad == 0) 
		printf(COLOR_YELLOW "\n  [La fila esta vacia. Usa [F] para agregar canciones.]" COLOR_RESET);

	printf("\n\tOpciones:\n(1) Agregar inicio por ID (2) Quitar 1ra (3) Vaciar (4) Quitar por posición (5) Quitar por ID (0) Volver\n");
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
	else if (opc == '4')
	{
		int pos;
		printf("  Ingrese posición a quitar (1-%d): ", cola->cantidad);
		if (scanf("%d", &pos) == 1)
		{
			while (getchar() != '\n');
			if (pos >= 1 && pos <= cola->cantidad)
			{
				quitar_de_cola_pos(cola, pos-1);
				printf(COLOR_GREEN "  Posición %d retirada de la fila.\n" COLOR_RESET, pos);
			}
			else
			{
				printf(COLOR_RED "  Posición fuera de rango.\n" COLOR_RESET);
			}
		}
		ui_pausa();
	}
	else if (opc == '5')
	{
		unsigned int id;
		printf("  Ingrese ID de la canción a quitar: ");
		if (scanf("%u", &id) == 1)
		{
			while (getchar() != '\n');
			if (quitar_de_cola_id(cola, id) == 0)
			{
				printf(COLOR_GREEN "  Canción con ID %u retirada de la fila.\n" COLOR_RESET, id);
			}
			else
			{
				printf(COLOR_RED "  Error: ID no encontrado o no se pudo retirar.\n" COLOR_RESET);
			}
		}
		ui_pausa();
	}
}

/** @brief Despliega el submenu del historial con las ultimas canciones reproducidas.
 *
 * @param historial Puntero a la playlist que contiene el historial de reproducciones.
 */
void ui_menu_historial(const playlist *historial)
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

/** @brief Despliega el submenu de busqueda (por ID, artista o titulo).
 *
 * @param catalogo Puntero al catalogo donde se realizaran las busquedas.
 */
void ui_menu_busqueda(playlist *catalogo)
{
	printf(COLOR_CYAN "\n  --- Busqueda ---\n" COLOR_RESET);
	printf("  [1] Buscar por ID [recursiva]\n");
	printf("  [2] Buscar por artista\n");
	printf("  [0] Salir\n");

	char opc = ui_input();
	if (opc == '1')
	{
		unsigned long long id;
		printf("  Ingrese ID de la canción: ");
		if (scanf("%llu", &id) == 1)
		{
			while (getchar() != '\n'); // eliminar enter de la cadena

			/* Comprobacion simple con el maximo de un unsigned int */
			if (id > UINT_MAX)
			{
				printf(COLOR_RED "  Error: El ID supera el valor maximo permitido (%u).\n" COLOR_RESET, UINT_MAX);
			}
			else
			{
				int pos = binsearch_cancion(catalogo, 0, catalogo->cantidad - 1, (unsigned int)id);
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
		}
		else
		{
			while (getchar() != '\n');
			printf(COLOR_RED "  Error: Entrada invalida.\n" COLOR_RESET);
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

/** @brief Despliega el submenu de ordenamiento del catalogo segun algoritmos y criterios.
 *
 * @param catalogo Puntero a la playlist del catalogo a ordenar.
 */
void ui_menu_ordenar(playlist *catalogo)
{
	printf(COLOR_CYAN "\n  --- Ordenar [Insertion Sort] ---\n" COLOR_RESET);
	printf("  [1] Insertion Sort (criterio)  [2] Merge Sort  [0] Volver al menu\n");

	char opc_alg = ui_input();
	if (opc_alg == '1')
	{
		printf(COLOR_CYAN "\n  --- Criterio [Insertion Sort] ---\n" COLOR_RESET);
		printf("  [0] ID  [1] Duracion  [2] Ano  [3] Reproducciones\n");
		printf("  [4] Titulo  [5] Artista  [6] Album  [7] Genero\n");
		char opc_crit = ui_input();
		if (opc_crit >= '0' && opc_crit <= '3')
		{
			int criterio = opc_crit - '0';
			insertion_sort_int(catalogo, criterio);
			printf(COLOR_GREEN "  Catalogo ordenado!\n" COLOR_RESET);
		}
		else
		{
			printf(COLOR_RED "  Opcion invalida.\n" COLOR_RESET);
		}
	}
	else if (opc_alg == '2')
	{
		if (catalogo != NULL && catalogo->cantidad > 0)
		{
			printf(COLOR_CYAN "  Ordenando por ID con Merge Sort (recursivo)...\n" COLOR_RESET);
			merge_sort(catalogo, 0, catalogo->cantidad - 1);
			printf(COLOR_GREEN "  Catalogo ordenado!\n" COLOR_RESET);
		}
	}
	else if (opc_alg == '0')
	{
		return;
	}
	else
	{
		printf(COLOR_RED "  Opcion invalida.\n" COLOR_RESET);
	}
	ui_pausa();
}

/** @brief Despliega el submenu para exportar el catalogo actual a un archivo CSV.
 *
 * @param catalogo Puntero a la playlist del catalogo a persistir en disco.
 */
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

/** @brief Despliega el submenu de rankings y estadisticas de reproduccion.
 *
 * @param pl Puntero al catalogo de canciones sobre el que se calculan las estadisticas.
 */
void ui_menu_ranking(const playlist *pl)
{
	printf(COLOR_CYAN "\n  --- Rankings & Stats ---\n" COLOR_RESET);
	printf("  [1] Top mas escuchadas\n");
	printf("  [2] La mas escuchada por artista\n");
	printf("  [3] La mas escuchada por genero\n");

	char opc = ui_input();
	if (opc == '1')
	{
		canciones_mas_escuchada(pl);
		ui_pausa();
	}
	else if (opc == '2')
	{
		char artista[100];
		printf(COLOR_CYAN "\n  >> Introduce el nombre del artista: " COLOR_RESET);
		if (fgets(artista, sizeof(artista), stdin) != NULL)
		{
			// Eliminamos salto de linea y final de cadena
			int i = 0;
			while (artista[i] != '\0')
			{
				if (artista[i] == '\n')
				{
					artista[i] = '\0';
					break;
				}
				i++;
			}

			int pos = mas_escuchada_artista(pl, artista);
			if (pos != -1)
			{
				printf(COLOR_GREEN "  Cancion mas escuchada de" COLOR_CYAN "%s...\n" COLOR_RESET, artista);
				printf(COLOR_CYAN "    %d. %s - %s\n" COLOR_RESET, pos + 1, pl->canciones[pos].artista, pl->canciones[pos].titulo);
			}
			else
			{
				printf(COLOR_RED "  No se encontro la cancion del artista [%s].\n" COLOR_RESET, artista);
			}
		}
		ui_pausa();
	}
	else if (opc == '3')
	{
		char genero[100];
		printf(COLOR_CYAN "\n  >> Introduce el genero (ej. Rock): " COLOR_RESET);
		if (fgets(genero, sizeof(genero), stdin) != NULL)
		{
			// Eliminamos salto de linea y final de cadena
			int i = 0;
			while (genero[i] != '\0')
			{
				if (genero[i] == '\n')
				{
					genero[i] = '\0';
					break;
				}
				i++;
			}

			int pos = mas_escuchada_genero(pl, genero);
			if (pos != -1)
			{
				printf(COLOR_GREEN "  Cancion mas escuchada del genero " COLOR_CYAN "%s...\n" COLOR_RESET, genero);
				printf(COLOR_CYAN "    %d. %s - %s\n" COLOR_RESET, pos + 1, pl->canciones[pos].artista, pl->canciones[pos].titulo);
			}
			else
			{
				printf(COLOR_RED "  No se encontro la cancion del genero [%s].\n" COLOR_RESET, genero);
			}
		}
		ui_pausa();
	}
}

/** @brief Despliega el submenu de extras (reproduccion de audio real y creditos). */
void ui_menu_extras(void)
{
	printf(COLOR_CYAN "\n  --- Extras ---\n" COLOR_RESET);
	printf("  [1] Reproducir playlist\n");
	printf("  [2] Mostrar creditos\n");
	printf("  [0] Salir\n");

	char opc = ui_input();
	if (opc == '1')
	{
		reproducir_playlist_csv();
		ui_pausa();
	}
	if (opc == '2')
	{
		mostrar_creditos();
	}
	else if (opc == '0')
	{
		printf(COLOR_YELLOW "\n  [Saliendo...]" COLOR_RESET);
	}
	ui_pausa();
}
