#include <stdio.h>
#include "songs.h"
#include "miniaudio/miniaudio.h"

int reproduir_musica(const char *ruta, cancion *c) 
{
	ma_result result;
	ma_engine engine;
	result = ma_engine_init(NULL, &engine);
	if (result != MA_SUCCESS) {
		return -1;
	}
	ma_engine_play_sound(&engine,ruta, NULL);
	printf("Press Enter to quit...");
	getchar();

	ma_engine_uninit(&engine);

	c->total_rep++;
	return 0;
}

