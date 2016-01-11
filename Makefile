all: read_pmu

CFLAGS := -O3 -std=gnu11 

read_pmu: read_pmu.c armpmu_lib.o
	$(CC) $(CFLAGS) $^ -o $@

armpmu_lib.o: armpmu_lib.c armpmu_lib.h
	$(CC) -c $(CFLAGS) $< -o $@

clean:
	rm read_pmu

.PHONY: all clean
