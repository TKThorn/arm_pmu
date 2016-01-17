#define _GNU_SOURCE
#include <stdio.h>
#include <inttypes.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <sched.h>

#include "armpmu_lib.h"

// sched_getcpu isn't defined here for some reason.
int getcpu() {
	int cpu = -1;
	if (syscall(SYS_getcpu, &cpu, NULL, NULL) == -1) {
		return -1;
	}
	return cpu;
}

void setaff(int cpu) {
	cpu_set_t set;
	CPU_ZERO(&set);
	CPU_SET(cpu, &set);
	if (sched_setaffinity(0, sizeof(set), &set)) {
		perror("Could not set affinity.");
	}
}

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

	uint32_t cycles;
	MRC_PMU(cycles, PMCCNTR);
	printf("Cycle Count: %"PRIu32"\n", cycles);

	uint32_t pmceid0, pmceid1;
	MRC_PMU(pmceid0, PMCEID0);
	MRC_PMU(pmceid1, PMCEID1);
	printf("PMCEID0: %"PRIx32"\n", pmceid0);
	printf("PMCEID1: %"PRIx32"\n", pmceid1);
	return 0;
}
