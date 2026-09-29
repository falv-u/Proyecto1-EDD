#include <stdio.h>
#include "songs.h"

int escribir_archivo_historial(void)
{
	FILE *f;
	f = fopen("ruta_historial", "w");
	if ( f == NULL ) 
	{
		printf("error al crearlo...\n");
		return 1;
	}
	return 0;

}
int existe_historial(void)
{
	FILE *f;
	int ver;
	f = fopen(ruta_historial, "r");
	if (f == NULL)
	{
		printf("archivo no encontrado, creandolo...\n");
		ver = escribir_archivo_historial();
		if (ver == 0)
			printf("archivo escrito correctamente en %s\n", ruta_historial);		
		if (ver == 1)
		{
			printf("algo ha salido mal, archivo no escrito en disco\n");
			return 1;
		}
	}
		
	return 0;
}

void agregar_a_historial(cancion c)
{
	FILE *f;
	f = fopen(ruta_historial, "a");

}
