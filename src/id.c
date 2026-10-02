#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>

uint32_t generar_id(void)
{
	FILE* archivo;
	uint32_t num_lineas = 0;
	char caracter = '\0';

	archivo = fopen("./playlist.csv", "r");
	if(archivo == NULL)
	{
		printf("Error al abrir list.csv\n");
		return 1;
	}

	while(caracter != EOF)
	{
		caracter = fgetc(archivo);
		if(caracter == '\n')
		{
			num_lineas++;
		}
	}

	fclose(archivo);

	return num_lineas;
}
