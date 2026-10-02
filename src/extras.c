#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "songs.h"
#include "miniaudio/miniaudio.h"

/** @brief Muestra los creditos del proyecto. */
void mostrar_creditos(void) {
	printf("\n\t===== Creditos =====\n");
	printf("\tProyecto 1 - EDD - SUMMY\n");
	printf("\tProgramación Musical\n");
	printf("\tAlumnos: \n");
	printf("\t- Francisco ALvarado\n");
	printf("\t\t- Cree el Docs, Makefile, playlist, catalogo, historial\n");
	printf("\t- Ariel Barria\n");
	printf("\t\t- UI, search, diseno el main y orquesto todas las funciones\n");
	printf("\t- Felipe Desmaras\n");
	printf("\t\t- Todos los sort, cola\n");
	printf("\tProfesor: Jacqueline Aldridge Aguila\n");
	printf("\tAgradecimientos a:\n\n");
	printf("\t - A la profe por ser buen profe y tenernos paciencia\n");
	printf("\t - A miltongoat por ensenarnos LaTex\n");
	printf("\t - A redbull, monster y score por\n");
	printf("\tpermitirnos terminar esto en tiempo record\n");
	printf("\tA zed por ser zed.\n");
	printf("\tYT music por dar ambiente en el entorno colaborativo\n");
	printf("\tY UNA DISCULPA POR: \b\n");
	printf("\tentregarlo tarde\n");
	printf("\tNO AGRADECEMOS A: \b\n");
	printf("\tDoxygen por hacer que nuestros comentarios se vean sucios\n");
	printf("\ty generar paginas de documentacion no muy agraciadas\n\n");
	printf("\tA las ias a las que intentamos pedir ayuda y nos intentaron\n");
	printf("\tcomplejizar y envenenar el codigo\n");
	printf("\tA las demas materias que nos aplastaron en pruebas e informes.\n");
	printf("\t====================\n\n");
}

/** @brief Simula o reproduce el audio de una pista musical.
 *
 * Inicializa el motor de audio miniaudio en memoria, ejecuta la reproduccion
 * del archivo apuntado por ruta y aguarda la confirmacion del usuario por consola.
 * Al concluir la reproduccion, desinicializa el motor para liberar recursos del
 * sistema e incrementa en una unidad el contador de reproducciones de la cancion.
 *
 * @param ruta Direccion del archivo de audio compatible en el sistema de archivos.
 * @param c Puntero a la cancion en reproduccion para actualizar su contador total_rep.
 * @return Retorna 0 en caso de exito, -1 si el motor de audio falla al inicializarse.
 */
int reproducir_musica(const char *ruta, cancion *c) 
{
	ma_result result;
	ma_engine engine;
	result = ma_engine_init(NULL, &engine);
	if (result != MA_SUCCESS) {
		return -1;
	}
	ma_engine_play_sound(&engine, ruta, NULL);
	printf("Press Enter to quit...");
	getchar();

	ma_engine_uninit(&engine);

	c->total_rep++;
	return 0;
}

/** @brief Funcion de pausa (no implementada). */
void pausar(void) 
{
	printf("Pausa no implementada.\n");
	printf("Presione Enter para continuar...");
	getchar();
}

/** @brief Reproduce una playlist desde un archivo CSV.
 *
 * Lee el archivo playlist.csv, para cada cancion construye la ruta del archivo
 * de audio en ./assets/{artista} - {titulo}.mp3 y lo reproduce usando
 * reproduir_musica. Solo esta parte reproduce musica de verdad.
 */
void reproducir_playlist_csv(void)
{
	playlist pl = {0};   // Initialize all fields to zero/NULL

	/* cargar por csv */
	if (playlist_cargar_csv(&pl, "./playlist.csv") != 0)
	{
		printf("Error: No se pudo cargar el archivo playlist.csv.\n");
		liberar_pl(&pl);
		return;
	}

	printf("Reproduciendo playlist desde CSV...\n");
	for (int i = 0; i < pl.cantidad; i++) {
		cancion *c = &pl.canciones[i];
		/* file path: ./assets/{artista} - {titulo}.mp3 */
		char ruta[256];
		snprintf(ruta, sizeof(ruta), "./assets/%s - %s.mp3", c->artista, c->titulo);

		FILE *f = fopen(ruta, "r");

		if (f) {
			fclose(f);
			printf("Reproduciendo: %s - %s\n", c->artista, c->titulo);
			reproducir_musica(ruta, c);
		} else {
			printf("Advertencia: No se encontró el archivo de audio para %s - %s\n", c->artista, c->titulo);
		}
	}

	liberar_pl(&pl);
}
