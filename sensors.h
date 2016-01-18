#ifndef SENSORS_H
#define SENSORS_H

// Reads CPU power usage in Watt.
float read_a15_power();
float read_a7_power();

// Reads A15 CPU temperature in ÂC.
int read_a15_temperature();

#endif
