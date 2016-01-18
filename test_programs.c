#include "test_programs.h"

static void dummy()
{
	static unsigned int foo = 2;
	for (int i = 0; i < 10000000; i++) {
		foo *= 2;
	}
}

test_program get_test_program(char *name)
{
	return dummy;
}
