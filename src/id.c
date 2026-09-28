#include <stdint.h>
#include <stdlib.h>

uint32_t generar_id(void)
{
	uint32_t a;

	a = rand() % UINT32_MAX;
	return a;
}
