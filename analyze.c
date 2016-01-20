#define _GNU_SOURCE
#include <inttypes.h>
#include <locale.h>
#include <pthread.h>
#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
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
	long long cycle_count;
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
	set_pmn(3, INST_RETIRED);
	reset_pmn();
	reset_ccnt();

	// Measure the time taken.
	struct timespec t1, t2;
	clock_gettime(CLOCK_MONOTONIC, &t1);

	// Run the test program with performance counters!
	enable_pmn();
	p(opts.cycle_count);
	disable_pmn();

	clock_gettime(CLOCK_MONOTONIC, &t2);
	printf("Time: %f s\n", (double)(t2.tv_sec - t1.tv_sec) + (double)(t2.tv_nsec - t1.tv_nsec) / 1e9);

	// Read the performance counters.
	uint32_t cycles, l1d_cache_ld, l1d_cache_st, br_mis_pred, inst_retired;
	MRC_PMU(cycles, PMCCNTR);
	l1d_cache_ld = read_pmn(0);
	l1d_cache_st = read_pmn(1);
	br_mis_pred = read_pmn(2);
	inst_retired = read_pmn(3);
	printf("CPU cycles: %'"PRIu32"\n", cycles);
	printf("Level 1 data cache access, read: %'"PRIu32"\n", l1d_cache_ld);
	printf("Level 1 data cache access, write: %'"PRIu32"\n", l1d_cache_st);
	printf("Mispredicted or not predicted branch speculatively executed: %'"PRIu32"\n", br_mis_pred);
	printf("Retired instruction: %'"PRIu32"\n", inst_retired);
}

static void usage(char *name)
{
	fprintf(stderr, "Usage: %s [options] <test program>\n", name);
	fprintf(stderr, "Options:\n");
	fprintf(stderr, "\t-c <cpu>: CPU the analysis runs on, defaults to the currently executing CPU.\n");
	fprintf(stderr, "\t-m <cpu>: CPU the sensor monitoring runs on, defaults to one of 0-3 or 4-7.\n");
	fprintf(stderr, "\t-n <cycles>: Number of cycles the test program does.\n");
}

int main(int argc, char **argv)
{
	// To make thousands separation in printf() work.
	setlocale(LC_ALL, "");

	// Option defaults
	opts.target_cpu = getcpu();
	opts.monitor_cpu = -1;
	opts.cycle_count = 1000000;

	int opt;
	while ((opt = getopt(argc, argv, "c:m:n:h")) != -1) {
		switch (opt) {
		case 'c':
			opts.target_cpu = atoi(optarg);
			break;
		case 'm':
			opts.monitor_cpu = atoi(optarg);
			break;
		case 'n':
			opts.cycle_count = atoll(optarg);
			break;
		case 'h':
		default:
			usage(argv[0]);
			return 1;
		}
	}

	if (argc - optind != 1) {
		usage(argv[0]);
		return 1;
	}
	opts.test_program = argv[optind];
	test_program p = get_test_program(opts.test_program);
	if (p == NULL) {
		fprintf(stderr, "Invalid test program %s\n", opts.test_program);
		return 1;
	}
	fprintf(stderr, "Test program: %s\n", opts.test_program);
	fprintf(stderr, "Number of cycles: %lld\n", opts.cycle_count);

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
