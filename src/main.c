#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "songs.h"
#include "ui.h"


int main(void)
{
	srand(time(NULL));
	playlist pl;
	pl = crear_playlist();
	cancion *actual = NULL;
	int corriendo = 1;

	while (corriendo)
	{
		limpia_pantalla();
		imprime_cancion(actual);
		imprimir_playlist(&pl);
		imprime_menu();

		char opc = ui_input();
		switch (opc)
		{
    		case '2':
    			ui_pausa();
    			break;
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
