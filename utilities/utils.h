/*
 * utils.h
 *
 *	Contains helping functions that will be used across files
 */

#ifndef UTILS_H_
#define UTILS_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#define DEBUG 0
#define TEST 0

/*
 * debug_print(const char* format, ...)
 *
 * Functions prints to stderr together with valuable information only if DEBUG == 1 else it does nothing
 *
 */
void debug_print(const char* format, ...);

void info_print(const char* format, ...);

void results_print(const char* format, ...);

char *rand_string(char *str, size_t size);

#endif /* UTILS_H_ */
