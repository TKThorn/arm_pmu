#include <stdio.h>
#include <unistd.h>

#include "sensors.h"

int main() {
	for (;;) {
		printf("A15: %f W / %iºC, A7: %f W\n", read_a15_power(), read_a15_temperature(), read_a7_power());
		usleep(200000);
	}
	return 0;
}
