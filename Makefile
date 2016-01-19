all: analyze read_pmu read_sensors specific_tests

CFLAGS := -O3 -std=gnu11


analyze: analyze.c armpmu_lib.o helpers.o sensors.o test_programs.o
	$(CC) $(CFLAGS) -pthread $^ -o $@

read_pmu: read_pmu.c helpers.o armpmu_lib.o
	$(CC) $(CFLAGS) $^ -o $@

read_sensors: read_sensors.c sensors.o
	$(CC) $(CFLAGS) $^ -o $@

specific_tests: specific_tests.c
    $(CC) $(CFLAGS) -pthread $^ -o $@

armpmu_lib.o: armpmu_lib.c armpmu_lib.h

.c.o:
	$(CC) -c $(CFLAGS) $< -o $@

clean:
	rm read_pmu

.PHONY: all clean
