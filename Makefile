all: read_pmu read_sensors

CFLAGS := -O3 -std=gnu11 

read_pmu: read_pmu.c armpmu_lib.o
	$(CC) $(CFLAGS) $^ -o $@

read_sensors: read_sensors.c sensors.o
	$(CC) $(CFLAGS) $^ -o $@

armpmu_lib.o: armpmu_lib.c armpmu_lib.h

.c.o:
	$(CC) -c $(CFLAGS) $< -o $@

clean:
	rm read_pmu

.PHONY: all clean
