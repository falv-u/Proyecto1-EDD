#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "songs.h"
#include "ui.h"


int main(void)
{
	srand(time(NULL));
	playlist pl = crear_playlist();
	cancion *actual = NULL;

	int corriendo = 1;
	int indice_actual = -1; // para recorrer pl.canciones

	while (corriendo)
	{
		limpiar_pantalla();
		imprime_cancion(actual);
		imprimir_playlist(&pl);
		imprime_menu();

		char opc = ui_input();
		switch (opc)
		{
			case 'P':
			case 'p':
			case '2':
				if (actual != NULL)
				{
					printf(COLOR_CYAN "\n  Reproduciendo/Pausando: %s..." COLOR_RESET, actual->titulo);
				}
				else
				{
					printf(COLOR_RED "\n No hay ninguna canción seleccionada." COLOR_RESET);
				}
				ui_pausa();
    			break;
			
			case 'N':
			case 'n':
				if (pl.cantidad > 0) // Siguiente canción
				{
					// Ciclicamente (%)
					indice_actual = (indice_actual + 1) % pl.cantidad;
					actual = &pl.canciones[indice_actual];
				}
				break;

			case 'B':
			case 'b':
				if (pl.cantidad > 0) // Anterior canción
				{
					// Ciclicamente (%)
					// le sumamos + pl.cantidad para que no hayan negativos. 
					indice_actual = (indice_actual - 1 + pl.cantidad) % pl.cantidad;
					actual = &pl.canciones[indice_actual];
				}
				break;

			case 'L':
			case 'l':
				// cargar la lista usando el separador indicado para el csv "," o "|"
				break;

			case 'T':
			case 't':
				// insertar funcion ordenamiento
				if (pl.cantidad > 0)
				{
					printf("\n");
				}
				ui_pausa();
				break;

            case 'Q':
            case 'q':
            case '0':
                corriendo = 0;
      		break;

    		default:
    			break;
		}
	}

	liberar_pl(&pl);
	return 0;
}
