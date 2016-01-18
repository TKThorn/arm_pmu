#define _GNU_SOURCE
#include <sched.h>
#include <stdio.h>
#include <sys/syscall.h>
#include <unistd.h>

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

