#include <stdio.h>
#include "songs.h"
#define MAX_K 19

long contar_lineas(const char *ruta)
{
	FILE *f;
	char buff[1024];
	long linea;	
	f = fopen(ruta, "r");
	if ( f == NULL )
	{
		printf("error, el archivo existe? \m");
		return -1;
	}

	while(fgets(buff,sizeof(buff),f))
		linea++;
	
	fclose(f);
	return linea;
	
}
long buscar_cid_csv(const char *ruta, unsigned int id_buscar, unsigned int rep)
{
	FILE *f;
	char buff[1024];
	unsigned int id, dur, anio, reps;

	f = fopen(ruta, "r");

	if (f == NULL)
	{
		printf("fallo al abrir archivo, el archivo existe?\n");
		return -1;
	}
	i = 0;

	while(fgets(buff,sizeof(buff),f))
	{
		sscanf("%u|%u|%u|%u|", &id, &dur, &anio, &reps);
		if (id == cid)
		{
			*rep = reps;
			fclose(f);
			return i;
		}
		i++;
	}
	return -1;
}

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
	long pos;
	unsigned int rep;

	pos = buscar_cid(ruta_historial, c->cid, &rep);

	if (pos != -1)
	{
		eliminar_linea_csv(ruta_historial, pos);
		c->total_rep = rep + 1;
	}
	else if (contar_lineas(ruta_historial) >= MAX_K)
	{
		eliminar_linea_csv(ruta_historial, 0);
	}

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



void eliminar_linea_csv(const char *ruta, long linea)
{
	FILE *in;
	FILE *out;
	char buff[1024];
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

	out = fopen("el.tmp", "w");
	if ( out == NULL)
	{
		printf("fallo al crear el archivo temporal");
		return;
	};

	i = 0;
	
	/* 
	 * toma todo lo que es el historial y usa fputs para llevar
	 * lo del buffer hacia el archivo de salida saltandose
	 * la linea a eliminar
	 */
	while (fgets(buff, sizeof(buff), in) != NULL)
	{
		if (i != linea)
			fputs(buff, out);
		i++;
	}
	
	remove(ruta);
	rename("el.tmp", ruta);
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


