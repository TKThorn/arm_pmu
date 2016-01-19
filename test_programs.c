#include "test_programs.h"

#include <string.h>
#include <stdio.h>

static void dummy()
{
	static unsigned int foo = 2;
	for (int i = 0; i < 10000000; i++) {
		foo *= 2;
	}
}

//generate a number of single mul instructions
void mul_generator(int cycles) {
    int var1 = 2;
    int var2 = 2;
    for (int i = 0; i < cycles; i++) {
        asm volatile ("mul %0, %1, %2" : "=r" (var1) : "0" (var1), "r" (var2));

    }
}

//generate a number of single mov instructions
void mov_generator(int cycles) {
    int src = 1;
    int dest;
    for (int i = 0; i < cycles; i++) {
        asm volatile ("mov %0, %1" : "=r" (dest) : "r" (src));
    }

}

//generate a number of branch instructions
void branch_generator(int cycles) {
    static int foo = 0;
    for (int i = 0; i < cycles; i++) {
        if ((i % 2) + (i % 3) + (i % 5)  == 2) {
            foo ++;
        } else {
            foo += 2;
        }
    }
}

//generate a number of single add instructions
void add_generator(int cycles) {
    int count = 1;
    int curr = 0;
    for (int i = 0; i < cycles / 12; i++) {
        asm volatile ("add %0, %1": "=r" (curr) : "r" (count) );
        asm volatile ("add %0, %1": "=r" (curr) : "r" (count) );
        asm volatile ("add %0, %1": "=r" (curr) : "r" (count) );
        asm volatile ("add %0, %1": "=r" (curr) : "r" (count) );
        asm volatile ("add %0, %1": "=r" (curr) : "r" (count) );
        asm volatile ("add %0, %1": "=r" (curr) : "r" (count) );
        asm volatile ("add %0, %1": "=r" (curr) : "r" (count) );
        asm volatile ("add %0, %1": "=r" (curr) : "r" (count) );
        asm volatile ("add %0, %1": "=r" (curr) : "r" (count) );
        asm volatile ("add %0, %1": "=r" (curr) : "r" (count) );
        asm volatile ("add %0, %1": "=r" (curr) : "r" (count) );
        asm volatile ("add %0, %1": "=r" (curr) : "r" (count) );
    }
}

// two parallel add instructions
void parallel_add_generator(int cycles) {
    int count = 1;
    int count2 = 1;
    int curr = 0;
    int curr2 = 0;
    for (int i = 0; i < cycles / 12; i++) {
        asm volatile ("add %0, %1": "=r" (curr) : "r" (count) );
        asm volatile ("add %0, %1": "=r" (curr2) : "r" (count2) );
        asm volatile ("add %0, %1": "=r" (curr) : "r" (count) );
        asm volatile ("add %0, %1": "=r" (curr2) : "r" (count2) );
        asm volatile ("add %0, %1": "=r" (curr) : "r" (count) );
        asm volatile ("add %0, %1": "=r" (curr2) : "r" (count2) );
        asm volatile ("add %0, %1": "=r" (curr) : "r" (count) );
        asm volatile ("add %0, %1": "=r" (curr2) : "r" (count2) );
        asm volatile ("add %0, %1": "=r" (curr) : "r" (count) );
        asm volatile ("add %0, %1": "=r" (curr2) : "r" (count2) );
        asm volatile ("add %0, %1": "=r" (curr) : "r" (count) );
        asm volatile ("add %0, %1": "=r" (curr2) : "r" (count2) );
    }
}




test_program get_test_program(char *name)
{
	if(strcmp(name, "add") == 0) {
		return add_generator;
	} else if(strcmp(name, "parallel_add") == 0) {
		return parallel_add_generator;
	} else if (strcmp(name, "mov") == 0) {
		return mov_generator;
	} else if (strcmp(name, "branch") == 0) {
		return branch_generator;
	} else if (strcmp(name, "mul") == 0){
		return mul_generator;
	}
	return 0;
}
