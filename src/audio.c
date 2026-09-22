#include <stdio.h>
#include "miniaudio/miniaudio.h"

int reproducir_musica() 
{

	const char ruta[] = "./assets/take_me_out.mp3";
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

	return 0;
}

