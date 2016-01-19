
#define _GNU_SOURCE
#include <stdio.h>
#include <inttypes.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <sched.h>
#include <cstdlib>


//choosing type of test
int main(int argc, char **argv) {
    if (argc < 2) {
        printf("set number of cycles\n");
        exit(-1);
    } else {
        double loop_limit = atoi(argv[2]);
        if(strcmp(argv[1], "add")) {
            add_generator(loop_limit);
        } else if (strcmp(argv[1], "mov")) {
            mov_generator(loop_limit);
        } else if (strcmp(argv[1], "branch")) {
            branch_generator(loop_limit);
        } else if (strcmp(argv[1], "mul")){
            mul_generator(loop_limit);
        }
    }
    return 0;
}

//from here on are the definitions of each test

//generate a number of single mul instructions
void mul_generator(double cycles) {
    double var1 = 2;
    double var2 = 2;
    printf("You are using dcache");
    for (int i = 0; i < cycles; i++) {
        asm volatile ("mul %0, %1, %2" : "=r" (var1) : "0" (var1), "r" (var2));

    }
}

//generate a number of single mov instructions
void mov_generator(double cycles) {
    printf("You are using cycles");
    int src = 1;
    int dest;
    for (int i = 0; i < cycles; i++) {
        asm volatile ("mov %0, %1" : "=r" (dest) : "r" (src));
    }

}

//generate a number of branch instructions
void branch_generator(double cycles) {
    printf("You are using branch");
    int foo = 0;
    for (int i = 0; i < cycles; i++) {
        if (i % 2 == 0) {
            foo ++;
        } else {
            foo += 2;
        }
    }
}

//generate a number of single add instructions
void add_generator(double cycles) {
    printf("You are using instruction counter");
    int count = 1;
    int curr = 0;
    for (int i = 0; i < cycles; i++) {
        asm volatile ("add %0, %1": "=r" (curr) : "r" (count) );
    }
}

