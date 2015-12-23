all: read_pmu

read_pmu: read_pmu.c armpmu_lib.h
	$(CC) -O3 -std=gnu11 $< -o $@

clean:
	rm read_pmu

.PHONY: all clean
