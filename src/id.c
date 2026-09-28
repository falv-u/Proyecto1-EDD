#include <stdint.h>
#include <stdlib.h>
#include <time.h>

uint32_t generar_id(void)
{
	srand(time(NULL));
	uint32_t a;

	a = rand() % UINT32_MAX;
	return a;
}
