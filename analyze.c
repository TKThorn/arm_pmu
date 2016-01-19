#define _GNU_SOURCE
#include <inttypes.h>
#include <pthread.h>
#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "armpmu_lib.h"
#include "helpers.h"
#include "sensors.h"
#include "test_programs.h"

#define NUMBER_OF_CPUS sysconf(_SC_NPROCESSORS_ONLN)

static struct {
	char *test_program;
	int target_cpu;
	int monitor_cpu;
} opts;

static void * sensors_monitor(void *arg)
{
	setaff(opts.monitor_cpu);
	for (;;) {
		printf("A15: %f W / %iºC, A7: %f W\n", read_a15_power(), read_a15_temperature(), read_a7_power());
		usleep(200000);
	}
}

static pthread_t start_monitor()
{
	// The monitor is a separate thread, pinned to another CPU.
	pthread_t tid;
	pthread_create(&tid, NULL, sensors_monitor, NULL);
	return tid;
}

static void start_test(test_program p)
{
	setaff(opts.target_cpu);

	// Set up performance counters.
	set_pmn(0, L1D_CACHE_LD);
	set_pmn(1, L1D_CACHE_ST);
	set_pmn(2, BR_MIS_PRED);
	set_pmn(3, BR_PRED);
	reset_pmn();
	reset_ccnt();

	// Run the test program with performance counters!
	enable_pmn();
	p(1000000000);
	disable_pmn();

	// Read the performance counters.
	uint32_t cycles, l1d_cache_ld, l1d_cache_st, br_mis_pred, br_pred;
	MRC_PMU(cycles, PMCCNTR);
	l1d_cache_ld = read_pmn(0);
	l1d_cache_st = read_pmn(1);
	br_mis_pred = read_pmn(2);
	br_pred = read_pmn(3);
	printf("CPU cycles: %"PRIu32"\n", cycles);
	printf("Level 1 data cache access, read: %"PRIu32"\n", l1d_cache_ld);
	printf("Level 1 data cache access, write: %"PRIu32"\n", l1d_cache_st);
	printf("Mispredicted or not predicted branch speculatively executed: %"PRIu32"\n", br_mis_pred);
	printf("Predictable branch speculatively executed: %"PRIu32"\n", br_pred);
}

static void usage(char *name)
{
	fprintf(stderr, "Usage: %s [options] <test program>\n", name);
	fprintf(stderr, "Options:\n");
	fprintf(stderr, "\t-c <cpu>: CPU the analysis runs on, defaults to the currently executing CPU.\n");
	fprintf(stderr, "\t-m <cpu>: CPU the sensor monitoring runs on, defaults to one of 0-3 or 4-7.\n");
}

int main(int argc, char **argv)
{
	// Option defaults
	opts.target_cpu = getcpu();
	opts.monitor_cpu = -1;

	int opt;
	while ((opt = getopt(argc, argv, "c:m:h")) != -1) {
		switch (opt) {
		case 'c':
			opts.target_cpu = atoi(optarg);
			break;
		case 'm':
			opts.monitor_cpu = atoi(optarg);
			break;
		case 'h':
		default:
			usage(argv[0]);
			return 1;
		}
	}

	if (argc != 2) {
		usage(argv[0]);
		return 1;
	}
	opts.test_program = argv[1];
	test_program p = get_test_program(opts.test_program);
	if (p == NULL) {
		fprintf(stderr, "Invalid test program %s\n", opts.test_program);
		return 1;
	}
	fprintf(stderr, "Test program: %s\n", opts.test_program);

	// We have two CPUs with an equal number of cores.
	if (opts.monitor_cpu == -1) opts.monitor_cpu = (opts.target_cpu + (NUMBER_OF_CPUS / 2)) % NUMBER_OF_CPUS;
	fprintf(stderr, "CPU: %d, Monitoring CPU: %d\n", opts.target_cpu, opts.monitor_cpu);

	fprintf(stderr, "\n");

	start_monitor();
	sleep(1);
	printf("\n=== STARTING TEST ===\n");
	start_test(p);
	printf("=== TEST FINISHED ===\n\n");
	sleep(1);

	return 0;
}
