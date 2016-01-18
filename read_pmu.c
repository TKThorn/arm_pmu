#include <stdio.h>
#include <inttypes.h>

#include "helpers.h"
#include "armpmu_lib.h"

int main(int argc, char **argv)
{
	uint32_t pmuserenr;
	MRC_PMU(pmuserenr, PMUSERENR);
	if (!pmuserenr) {
		fprintf(stderr, "User access to performance counters disabled, exiting\n");
		return 1;
	}

	int cpu = getcpu();
	printf("Executing CPU: %d\n", cpu);
	// Set CPU affinity to make sure that the following code is executed on the
	// CPU printed above.
	setaff(cpu);

	enable_pmn();

	uint32_t pmcr, pmceid0, pmceid1;
	MRC_PMU(pmcr, PMCR);
	MRC_PMU(pmceid0, PMCEID0);
	MRC_PMU(pmceid1, PMCEID1);
	printf("PMCR: %"PRIx32"\n", pmcr);
	printf("PMCEID0: %"PRIx32"\n", pmceid0);
	printf("PMCEID1: %"PRIx32"\n", pmceid1);

	set_pmn(0, CPU_CYCLES);
	set_pmn(1, L1D_CACHE_LD);
	set_pmn(2, L1D_CACHE_ST);
	reset_pmn();
	reset_ccnt();
	// do some fake work
	for (int i = 0; i < 50; i++) printf(".");
	uint32_t cycles, cpu_cycles, l1d_cache_ld, l1d_cache_st;
	MRC_PMU(cycles, PMCCNTR);
	cpu_cycles = read_pmn(0);
	l1d_cache_ld = read_pmn(1);
	l1d_cache_st = read_pmn(2);
	printf("\nCPU cycles: %"PRIu32"\n", cpu_cycles);
	printf("Cycle Count: %"PRIu32"\n", cycles);
	printf("Level 1 data cache access, read: %"PRIu32"\n", l1d_cache_ld);
	printf("Level 1 data cache access, write: %"PRIu32"\n", l1d_cache_st);

	return 0;
}
