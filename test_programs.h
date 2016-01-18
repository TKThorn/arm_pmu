#ifndef TEST_PROGRAMS_H
#define TEST_PROGRAMS_H

typedef void (*test_program)();

// Returns a test program by name, or NULL if there is none.
test_program get_test_program(char *name);

#endif
