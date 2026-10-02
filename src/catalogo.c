 /* id;duracion;anio;reproducciones;titulo;artista;album;genero */
#include <stdio.h>
#include <stdlib.h>
#include "songs.h"

#define DUR_MIN   60
#define DUR_MAX   600
#define ANIO_MIN  1960
#define ANIO_MAX  2026
#define REP_MAX   100000
#define N_MAXIMO  5000000

#define N_ARTISTAS 15
#define N_GENEROS  9
#define N_PALABRAS 30

static const char *ARTISTAS[N_ARTISTAS] = {
	"Queen", "KISS", "Nirvana", "Metalica", "Mago de oz",
	"Los Prisioneros", "Los Bunkers", "Radiohead", "Gorillaz", "Bad Bunny",
	"Los Beatles", "Heroes del silencion", "31 minutos", "13 minutos", "Cuarteto de nos"
};
static const char *GENEROS[N_GENEROS] = {
	"Rock", "Hip-Hop", "Funk", "Pop", "Electronica", "Jazz", "Latina", "rap", "clasica"
};
static const char *PALABRAS[N_PALABRAS] = {
	"Noche", "Fuego", "Camino", "Sueno", "Ciudad", "Rojo",
	"Eterno", "Viento", "Sombra", "Cielo", "Danza", "Luz",
	"Lluvia", "hecho para amarte", "amor", "silencio", "hombre",
	"alas", "tren", "salgamos", "nosotros", "volamos", "palabras",
	"eterna", "crucificar", "diferente", "no escuchar", "angel",
	"Dios", "espejo"
};

/* Un tema: numeros y posiciones en los arreglos de textos. */
typedef struct {
	uint32_t dur, reps;
	uint16_t anio;
	uint8_t  p1, p2, p3, p4, artista, genero;
} tema;

/* Llena un tema con datos aleatorios. */
void llenar(tema *t)
{
	t->dur     = DUR_MIN + rand() % (DUR_MAX - DUR_MIN + 1);
	t->anio    = ANIO_MIN + rand() % (ANIO_MAX - ANIO_MIN + 1);
	t->reps    = rand() % (REP_MAX + 1);
	t->p1      = rand() % N_PALABRAS;
	t->p2      = rand() % N_PALABRAS;
	t->p3      = rand() % N_PALABRAS;
	t->p4      = rand() % N_PALABRAS;
	t->artista = rand() % N_ARTISTAS;
	t->genero  = rand() % N_GENEROS;
}

/* Retorna 1 si los valores estan en rango, 0 si no. */
int es_valido(const tema *t)
{
	return t->dur >= DUR_MIN && t->dur <= DUR_MAX
	    && t->anio >= ANIO_MIN && t->anio <= ANIO_MAX
	    && t->reps <= REP_MAX;
}

/* Fisher-Yates: mezcla v en el lugar, O(n). */
void mezclar(tema *v, int n)
{
	int i, j;
	tema aux;

	for (i = n - 1; i > 0; i--)
	{
		j = rand() % (i + 1);
		aux = v[i];
		v[i] = v[j];
		v[j] = aux;
	}
}

/* Retorna 1 si el archivo existe, 0 si no. */
int existe(const char *ruta)
{
	FILE *f = fopen(ruta, "r");

	if (f == NULL)
		return 0;
	fclose(f);
	return 1;
}

/* Escribe n temas en ruta. Usa un .tmp y rename para no dejar
 * archivos a medias. Retorno: 0 exito, -1 argumentos invalidos,
 * -2 sin memoria, -3 error de escritura. */
int escribir_csv(const char *ruta, int n)
{
	char tmp[256];
	tema *v;
	FILE *f;
	int i, ok;

	if (ruta == NULL || n <= 0 || n > N_MAXIMO)
		return -1;
	if (snprintf(tmp, sizeof(tmp), "%s.tmp", ruta) >= (int)sizeof(tmp))
		return -1;

	v = malloc(sizeof(tema) * n);
	if (v == NULL)
		return -2;

	for (i = 0; i < n; i++)
	{
		llenar(&v[i]);
		if (!es_valido(&v[i]))
		{
			free(v);
			return -3;
		}
	}
	mezclar(v, n);

	f = fopen(tmp, "w");
	if (f == NULL)
	{
		free(v);
		return -3;
	}
	for (i = 0; i < n; i++)
		fprintf(f, "%d;%u;%u;%u;%s %s;%s;%s %s;%s\n",
		        i + 1, v[i].dur, (unsigned)v[i].anio, v[i].reps,
		        PALABRAS[v[i].p1], PALABRAS[v[i].p2],
		        ARTISTAS[v[i].artista],
		        PALABRAS[v[i].p3], PALABRAS[v[i].p4],
		        GENEROS[v[i].genero]);
	free(v);

	ok = !ferror(f);
	if (fclose(f) != 0)
		ok = 0;
	if (!ok || rename(tmp, ruta) != 0)
	{
		remove(tmp);
		return -3;
	}
	return 0;
}

/* Si ruta no existe genera n canciones; si existe no hace nada.
 * Retorno: 0 si ya existia o se genero, negativo si fallo. */
int generar_catalogo_si_no_existe(const char *ruta, int n)
{
	if (ruta == NULL)
		return -1;
	if (existe(ruta))
		return 0;
	printf("catalogo no encontrado, generando %d canciones...\n", n);
	return escribir_csv(ruta, n);
}
/* cuenta lineas de archivo */
unsigned int contar_por_archivo(const char *ruta)
{
	FILE *f;
	char linea_actual[1024];
	int total;
	f = fopen(ruta, "r");
	if ( ruta == NULL || f == NULL)
	{
		printf("no se pudo contabilizar lineas, el archivo existe?\n");
		return 0;
	}
	total = 0;

	while (fgets(linea_actual, sizeof(linea_actual), f))
		if (linea_actual[0] != '\n')
			total++;
	return total;

}
/* busca coincidencias */
void canciones_por_genero(const playlist *catalogo)
{
	int cont_g[N_GENEROS] = {0};
	playlist *pl;
	int i;

	for (i = 0; i< catalogo->cantidad; i++)
	{
		if (catalogo->canciones[i].genero == NULL)
			continue;
		/* Clasificamos comparando con los generos definidos */
		for (j = 0; j < N_GENEROS; j++)
		{
			if (compara_strings(catalogo->canciones[i].genero, (char *)GENEROS[j]))
			{
				cont_g[j]++;
				break; /* Ya coincidió, pasa a la siguiente canción */
			}
		}
	}

	/* Mostrar info*/
	printf("\n--- Cantidad de canciones por genero ---\n");
	for (j = 0; j < N_GENEROS; j++)
	{
		printf("%-15s : %d canciones\n", GENEROS[j], cont_g[j]);
	}
}
