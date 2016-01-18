//
// Created by thomas on 17.01.16.
//

#define _GNU_SOURCE
#include <stdio.h>
#include <inttypes.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <sched.h>
#include <cstdlib>

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

int main(int argc, char **argv) {
    int cpu = getcpu();
    printf("Executing CPU: %d\n", cpu);
    // Set CPU affinity to make sure that the following code is executed on the
    // CPU printed above.
    setaff(cpu);

    if (argc < 2) {
        printf("set number of cycles\n");
        exit(-1);
    } else {
        double loop_limit = atoi(argv[2]);
        if(strcmp(argv[1], "dcache")) {
            dcache_misses_generator(loop_limit);
        } else if (strcmp(argv[1], "cycle")) {
            cycle_generator(loop_limit);
        } else if (strcmp(argv[1], "branch")) {
            branch_generator(loop_limit);
        } else {
            instruction_exec_generator(loop_limit);
        }
    }
    return 0;
}

//from here on are the definitions of each test

void dcache_misses_generator(double cycles) {
    printf("You are using dcache");
    for (int i = 0; i < cycles; i++) {

    }
}

void cycle_generator(double cycles) {
    printf("You are using cycles");
    for (int i = 0; i < cycles; i++) {

    }

}

void branch_generator(double cycles) {
    printf("You are using branch");
    for (int i = 0; i < cycles; i++) {

    }

}

void instruction_exec_generator(double cycles) {
    printf("You are using instruction counter");
    unsigned int var = 2;
    for (int i = 0; i < cycles; i++) {
        var *= 2;
    }
}

