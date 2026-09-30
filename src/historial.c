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

int agregar_a_historial(cancion *c)
{
	FILE *f;
	f = fopen(ruta_historial, "a");
	if (f == NULL)
	{
		printf("error abriendo historial...\n");
		return 121;
	}

	fprintf(f, "%u|%u|%u|%u|%s|%s|%s|%s\n",
			c->cid, c->duracion, c->anio, c->total_rep,
			c->titulo, c->artista, c->album, c->genero);

	fclose(f);
	return 0;

}

void limpiar_csv(const char *ruta)
{
	FILE *f;
	f = fopen(ruta, "w");
	fclose(f);
}


/*
 * IDEA: verificar el caso mas sencillo primero, que se repite la ultima cancion.
 * en caso de no, verificamos el historial cargado, deben ser maximo K canciones
 * si una nueva cancion llena el indice K, eliminamos la primera linea pase lo que pase
 * recorremos de forma lineal el historial buscando el id de la cancion que acrtualiza, si esta leemos
 * total_rep, aumentamos en uno, guardamos al final y eso o tiramos al archivo
 *
 */

void eliminar_linea_csv(const char *ruta, long linea)
{
	FILE *in;
	FILE *out;
	char linea_actual[1024];
	long i;
	/*
	 * "r+" abre el archivo al inicio sin sobreescribir, 
	 * sin embargo no tiene capacidad de crearlo si no existe
	 */
	in = fopen(ruta, "r+");
	if ( in == NULL)
	{
		printf("fallo abrir archivo, el archivo existe?");
		return;
	};
	
	fclose(in);
	fclose(out);
}

void agregar_reproduccion_a_ultimo_historial(cancion *c)
{
	FILE *f;
	char linea[1024];
	long int pos;
	f = fopen(ruta_historial, "a");

	if (f == NULL)
	{
		printf("error al abrir archivo...\n");
		return;
	}

}


