#include <stdio.h>

#include "sensors.h"

int main() {
	for (;;) printf("A15 Power: %f W, A7 Power: %f W\n", read_a15_power(), read_a7_power());
	return 0;
}
