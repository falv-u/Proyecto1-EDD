#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "songs.h"

int main(void)
{
	srand(time(NULL));
	playlist pl;
	pl = crear_playlist();
	imprimir_playlist(&pl);
	liberar_pl(&pl);
	return 0;
}
